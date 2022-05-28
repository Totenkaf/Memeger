// Copyright 2022 by Artem Ustsov

#include <algorithm>

#include "models.h"
#include "postgresql_db.h"

// Конструктор с уставкой. Подлкючается к БД на указанном порте, хосте
Postgre_DB::Postgre_DB(std::string db_host, std::string db_port,
                       std::string db_name, std::string db_user,
                       std::string db_password)
    : db_host_(std::move(db_host)), db_port_(std::move(db_port)),
      db_name_(std::move(db_name)), db_user_(std::move(db_user)),
      db_password_(std::move(db_password)) {
  std::string request = "dbname = " + db_name_ + " user = " + db_user_ +
                        " password = " + db_password_ +
                        " hostaddr = " + db_host_ + " port = " + db_port_;
  try {
    PG_conn = std::make_shared<pqxx::connection>(request);
    if (PG_conn->is_open()) {
      init_tables();
    } else {
      std::cerr << "Can't open database" << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

Postgre_DB::~Postgre_DB() {
  if (!PG_conn->is_open()) {
    std::cerr << "Can't open database" << std::endl;
  } else {
    PG_conn->disconnect();
  }
}

// Создание воркеров для отправки запросов в БД
auto Postgre_DB::execution_table(const std::string &table_name,
                                 std::shared_ptr<pqxx::connection> &PG_conn)
    -> int {
  pqxx::work N(*PG_conn);
  try {
    N.exec(table_name);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "TABLE " << table_name << " execution fault" << std::endl;
    return _TABLE_EXECUTION_FAULT;
  }
  N.commit();
  return _EXIT_SUCCESS;
}

// Инициализация пустых таблиц для хранения информации о сущностях
auto Postgre_DB::init_tables() -> int {
  std::string create_table = "CREATE TABLE IF NOT EXISTS ";

  std::string users =
      create_table +
      "USERS (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY, \
          login VARCHAR(50) NOT NULL, \
          CONSTRAINT unique_login_users UNIQUE(login), \
          password VARCHAR(50) NOT NULL, \
          active_status VARCHAR(8) NOT NULL DEFAULT 'active', \
          CONSTRAINT real_active_statuses CHECK(active_status = 'active' \
          OR active_status = 'inactive'), \
          time_creation TIMESTAMP NOT NULL DEFAULT NOW());";
  if (execution_table(users, PG_conn) != 0) {
    return _TABLE_EXECUTION_FAULT;
  }

  std::string chats =
      create_table +
      "CHATS (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY, \
          chat_name VARCHAR(50) NOT NULL DEFAULT 'with_yourself', \
          CONSTRAINT unique_chat_name UNIQUE (chat_name), \
          time_creation TIMESTAMP NOT NULL DEFAULT NOW());";
  if (execution_table(chats, PG_conn) != 0) {
    return _TABLE_EXECUTION_FAULT;
  }

  std::string messages =
      create_table +
      "MESSAGES (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY, \
        user_from UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE, \
        time_sent TIMESTAMP NOT NULL DEFAULT NOW(), \
        chat_id UUID NOT NULL REFERENCES CHATS(id) ON DELETE CASCADE, \
        content VARCHAR(150), \
        is_read BOOLEAN NOT NULL DEFAULT false);";
  if (execution_table(messages, PG_conn) != 0) {
    return _TABLE_EXECUTION_FAULT;
  }

  std::string users_chats_link =
      create_table + "USERS_CHATS_LINK (id UUID NOT NULL DEFAULT\
                     uuid_generate_v4() PRIMARY KEY,\
        user_id UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE, \
        chat_id UUID NOT NULL REFERENCES CHATS(id) ON DELETE CASCADE);";
  if (execution_table(users_chats_link, PG_conn) != 0) {
    return _TABLE_EXECUTION_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Удаление таблиц из БД
auto Postgre_DB::drop_tables() -> int {
  std::string drop_table = "DROP TABLE ";

  std::string users = drop_table + "USERS CASCADE;";
  if (execution_table(users, PG_conn) != 0) {
    return _TABLE_DELETION_FAULT;
  }

  std::string messages = drop_table + "MESSAGES CASCADE;";
  if (execution_table(messages, PG_conn) != 0) {
    return _TABLE_DELETION_FAULT;
  }

  std::string chats = drop_table + "CHATS CASCADE;";
  if (execution_table(chats, PG_conn) != 0) {
    return _TABLE_DELETION_FAULT;
  }

  std::string users_chats_link = drop_table + "USERS_CHATS_LINK CASCADE;";
  if (execution_table(users_chats_link, PG_conn) != 0) {
    return _TABLE_DELETION_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Удаление лишних символов, которые могут помешать парсингу (/, '', ;)
auto Postgre_DB::remove_danger_characters(const std::string &row_column)
    -> std::string {
  std::string prep_column = std::string();
  for (const auto &i : row_column) {
    if (i != '\'' && i != '\"') {
      prep_column += i;
    }
  }
  return prep_column;
}

// Парсинг имен таблиц в запросах
auto Postgre_DB::parse_table_fields(const std::vector<std::string> &fields)
    -> std::string {
  std::string request = " (";
  for (size_t i = 0; i < fields.size(); ++i) {
    if (i == fields.size() - 1) {
      request += remove_danger_characters(fields[i]) + ")";
      continue;
    }
    request += remove_danger_characters(fields[i]) + ", ";
  }
  return request;
}

// Парсинг значений полей таблиц в запросах
auto Postgre_DB::parse_table_values(const std::vector<std::string> &values)
    -> std::string {
  std::string request = " VALUES('";
  for (size_t i = 0; i < values.size(); ++i) {
    if (i == values.size() - 1) {
      request += remove_danger_characters(values[i]) + "') ";
      continue;
    }
    request += remove_danger_characters(values[i]) + "', '";
  }
  return request;
}

// Парсинг выходных значений
auto Postgre_DB::parse_output_params(
    const std::vector<std::string> &output_params) -> std::string {
  std::string request = " ";
  for (size_t i = 0; i < output_params.size(); ++i) {
    if (i == output_params.size() - 1) {
      request += remove_danger_characters(output_params[i]);
      continue;
    }
    request += remove_danger_characters(output_params[i]) + ",";
  }
  return request;
}

// Обертка над запросом на вставку данных в БД
auto Postgre_DB::insert(const std::string &table,
                        const std::vector<std::string> &table_fields,
                        const std::vector<std::string> &values,
                        std::vector<std::string> &output_params) -> int {
  pqxx::work N(*PG_conn);
  try {
    std::string request = "INSERT INTO " + table +
                          parse_table_fields(table_fields) +
                          parse_table_values(values);
    if (!output_params.empty()) {
      request += "RETURNING" + parse_output_params(output_params) + ";";
    }

    pqxx::result res = N.exec(request);
    if (!output_params.empty()) {
      pqxx::result::const_iterator c = res.begin();
      for (int i = 0; i < static_cast<int>(output_params.size()); ++i) {
        output_params.at(i) = c.at(i).as<std::string>();
      }
    }
    N.commit();
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _INSERT_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Обертка над запросом на выбор данных из БД
auto Postgre_DB::select(
    const std::string &table, const std::string &where = std::string(),
    const std::vector<std::string> &what = std::vector<std::string>(),
    int limit = -1) -> pqxx::result {
  std::string request = "SELECT ";
  if (!what.empty()) {
    for (const auto &item : what) {
      request += remove_danger_characters(item) + ", ";
    }
    request = request.substr(0, request.size() - 2);
  } else {
    request += "*";
  }
  request += " FROM " + table;
  if (!where.empty()) {
    request += " WHERE " + where;
  }
  if (limit > 0) {
    request += " ORDER BY time_sent DESC LIMIT " + std::to_string(limit);
  }
  request += ";";
  pqxx::nontransaction N(*PG_conn);
  pqxx::result res = N.exec(request);
  N.commit();
  return res;
}

// Обертка над запросом на обновление существующих данных в БД
auto Postgre_DB::update(const std::string &table,
                        const std::vector<std::string> &table_fields,
                        const std::vector<std::string> &values,
                        const std::string &where = std::string()) -> int {
  pqxx::work N(*PG_conn);
  try {
    std::string request = "UPDATE " + table + " SET ";
    for (size_t i = 0; i < table_fields.size(); ++i) {
      request += table_fields[i] + " = '" +
                 remove_danger_characters(values[i]) + "', ";
    }
    request = request.substr(0, request.size() - 2);
    if (!where.empty()) {
      request += " WHERE " + where;
    }
    request += ";";
    N.exec(request);
    N.commit();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _UPDATE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Обертка над сохранением записи в БД
auto Postgre_DB::save(const std::string &table,
                      const std::vector<std::string> &table_fields,
                      const std::vector<std::string> &values,
                      std::vector<std::string> &output_params,
                      const std::string &where = std::string()) -> int {
  pqxx::result check = select(table, where);
  try {
    if ((!where.empty()) && (!check.empty())) {
      check.clear();
      return update(table, table_fields, values, where);
    }
    check.clear();
    return insert(table, table_fields, values, output_params);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    check.clear();
    return _SAVE_FAULT;
  }
}

// Обертка над удалением записи из конкретной таблицы БД
auto Postgre_DB::delete_(const std::string &table,
                         const std::string &where = std::string()) -> int {
  pqxx::work N(*PG_conn);
  std::string request = "DELETE FROM " + table;
  if (!where.empty()) {
    request += " WHERE " + where;
  }
  request += ";";
  try {
    N.exec(request);
    N.commit();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _DELETE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Добавление нового пользователя в таблицу USERS в БД
auto Postgre_DB::add_user(User &user) -> int {
  std::vector<std::string> data = {user.get_login(), user.get_password()};
  std::vector<std::string> table_fields = {"login", "password"};
  std::vector<std::string> output_params = {"id", "active_status"};
  try {
    save("USERS", table_fields, data, output_params);
    if (output_params[0] == "id" || output_params[1] == "active_status") {
      return _SAVE_FAULT;
    }
    user.set_id(output_params[0]);
    user.set_active_status(output_params[1]);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _SAVE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Получение ID пользователя из БД
auto Postgre_DB::get_user_id(const std::string &login) -> std::string {
  std::string where = "login = '" + login + "'";
  std::vector<std::string> what = {"id"};
  std::string user_id;

  try {
    pqxx::result res = select("USERS", where, what);
    pqxx::result::const_iterator c = res.begin();
    user_id = c[0].as<std::string>();
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return user_id;
  }
  return user_id;
}

// Получение логина пользователя из БД
auto Postgre_DB::get_user_login(const std::string &id) -> std::string {
  std::string where = "id = '" + id + "'";
  std::vector<std::string> what = {"login"};
  std::string user_login;
  try {
    pqxx::result res = select("USERS", where, what);
    pqxx::result::const_iterator c = res.begin();
    user_login = c.at(0).as<std::string>();
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG USER" << std::endl;
  }
  return user_login;
}

// Получение сущности пользователя по логину
auto Postgre_DB::get_user_by_login(const std::string &login) -> User {
  std::string where = "login = '" + login + "'";
  User user;
  try {
    pqxx::result res = select("USERS", where);
    if (!res.empty()) {
      pqxx::result::const_iterator c = res.begin();
      user.set_id(c.at(0).as<std::string>());
      user.set_login(c.at(1).as<std::string>());
      user.set_password(c.at(2).as<std::string>());
      user.set_active_status(c.at(3).as<std::string>());
    }
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG USER" << std::endl;
  }
  return user;
}

// Поиск сущности пользователя по логину в БД
auto Postgre_DB::find_user_by_login(const std::string &login) -> bool {
  std::string where = "login = '" + login + "'";
  try {
    pqxx::result res = select("USERS", where);
    if (!res.empty()) {
      return true;
    }
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return false;
}

// Замена логина сущности пользователя в БД
auto Postgre_DB::change_user_login(User &user, const std::string &new_login)
    -> int {
  if (!find_user_by_login(user.get_login())) {
    return _UPDATE_FAULT;
  }
  std::string where =
      "login = '" + remove_danger_characters(user.get_login()) + "'";
  std::vector<std::string> values = {new_login};
  std::vector<std::string> table_fields = {"login"};
  try {
    int update_status = update("USERS", table_fields, values, where);
    if (update_status == 0) {
      user.set_login(new_login);
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _UPDATE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Замена статуса работы пользователя
auto Postgre_DB::change_user_status(User &user, const std::string &new_status)
    -> int {
  if (!find_user_by_login(user.get_login())) {
    return _UPDATE_FAULT;
  }
  std::string where =
      "login = '" + remove_danger_characters(user.get_login()) + "'";
  std::vector<std::string> values = {new_status};
  std::vector<std::string> table_fields = {"active_status"};
  try {
    int update_status = update("USERS", table_fields, values, where);
    if (update_status == 0) {
      user.set_active_status(new_status);
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _UPDATE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Замена пароля пользователя в БД
auto Postgre_DB::change_user_password(User &user,
                                      const std::string &new_password) -> int {
  if (!find_user_by_login(user.get_login())) {
    return _UPDATE_FAULT;
  }
  std::string where =
      "login = '" + remove_danger_characters(user.get_login()) + "'";
  std::vector<std::string> values = {new_password};
  std::vector<std::string> table_fields = {"password"};
  try {
    int update_status = update("USERS", table_fields, values, where);
    if (update_status == 0) {
      user.set_password(new_password);
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _UPDATE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Удаление пользователя из БД, удаляются все связанные линки
auto Postgre_DB::delete_user(User &user) -> int {
  if (!user.get_id().empty()) {
    std::string where =
        "id = '" + remove_danger_characters(user.get_id()) + "'";
    int delete_status = delete_("USERS", where);
    if (delete_status == 0) {
      user.clear_user();
      return _EXIT_SUCCESS;
    }
  }
  return _DELETE_FAULT;
}

// Удаление сообщения из БД
auto Postgre_DB::delete_message(IMessage &message) -> int {
  if (!message.get_message_id().empty()) {
    std::string where =
        "id = '" + remove_danger_characters(message.get_message_id()) + "'";
    return delete_("MESSAGES", where);
  }
  return _DELETE_FAULT;
}

// Добавление сообщения в БД
auto Postgre_DB::add_message(IMessage &message) -> int {
  std::vector<std::string> data = {message.get_sender_id(),
                                   message.get_parent_chat_id(),
                                   message.get_message_content()};
  std::vector<std::string> table_fields = {"user_from", "chat_id", "content"};
  try {
    std::vector<std::string> output_params = {"id"};
    save("MESSAGES", table_fields, data, output_params);
    message.set_message_id(output_params[0]);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG ADD USER" << std::endl;
    return _SAVE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Добавление связи конкретного пользователя с конкретным чатом
auto Postgre_DB::add_user_chat_link(const std::string &user_id,
                                    const std::string &chat_id) -> int {
  std::vector<std::string> data = {user_id, chat_id};
  std::vector<std::string> table_fields = {"user_id", "chat_id"};
  std::vector<std::string> output_params = {};
  try {
    save("USERS_CHATS_LINK", table_fields, data, output_params);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG USERS CHATS linking" << std::endl;
    return _SAVE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Добавление сущности чата в БД
auto Postgre_DB::add_chat(Chat &chat) -> int {
  std::vector<std::string> data = {chat.get_chat_name()};
  std::vector<std::string> table_fields = {"chat_name"};
  std::vector<std::string> output_params = {"id"};
  try {
    save("CHATS", table_fields, data, output_params);
    if (output_params.at(0) == "id") {
      return _SAVE_FAULT;
    }
    chat.set_chat_id(output_params.at(0));
    std::vector<std::string> participants = chat.get_participants();
    for (const auto &participant : participants) {
      add_user_chat_link(get_user_id(participant), chat.get_chat_id());
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _SAVE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Получение сущности чата из БД
auto Postgre_DB::get_chat_by_chat_name(const std::string &chat_name) -> Chat {
  static constexpr size_t NUM_OF_LAST_MESSAGES = 25;
  std::string where = "chat_name = '" + chat_name + "'";
  pqxx::result res = select("CHATS", where);
  Chat chat;
  try {
    pqxx::result::const_iterator c = res.begin();
    chat.set_chat_id(c.at(0).as<std::string>());
    chat.set_chat_name(c.at(1).as<std::string>());
    std::vector<std::string> participants;
    participants = get_participants_from_chat(chat);
    chat.set_participants(participants);
    std::vector<std::shared_ptr<IMessage>> messages;
    messages = get_last_N_messages_from_chat(chat, NUM_OF_LAST_MESSAGES);
    chat.set_chat_messages(messages);
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG CHAT_NAME" << std::endl;
  }
  return chat;
}

// Удаление сущности чата из БД, удалятся все связанные линки
auto Postgre_DB::delete_chat(Chat &chat) -> int {
  if (!chat.get_chat_id().empty()) {
    std::string where =
        "id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
    int delete_status = delete_("CHATS", where);
    if (delete_status == 0) {
      chat.clear_chat();
      return _EXIT_SUCCESS;
    }
  }
  return _DELETE_FAULT;
}

// Поиск сущности чата в БД
auto Postgre_DB::find_chat_by_chat_name(const std::string &chat_name) -> bool {
  std::string where =
      "chat_name = '" + remove_danger_characters(chat_name) + "'";
  try {
    pqxx::result res = select("CHATS", where);
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

// Получение списка имен пользователей из чата
auto Postgre_DB::get_participants_from_chat(const Chat &chat)
    -> std::vector<std::string> {
  std::string where =
      "chat_id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
  std::vector<std::string> what = {"user_id"};
  pqxx::result res = select("USERS_CHATS_LINK", where, what);
  std::vector<std::string> participants;
  try {
    for (const auto &el : res) {
      std::string user_login = get_user_login(el.at(0).as<std::string>());
      participants.push_back(user_login);
    }
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG PARTICIPANTS" << std::endl;
  }
  return participants;
}

// Получение сущности чата по ID
auto Postgre_DB::get_chat_by_id(const std::string &chat_id) -> Chat {
  static constexpr size_t NUM_OF_LAST_MESSAGES = 25;
  std::string where = "id = '" + remove_danger_characters(chat_id) + "'";
  Chat chat;
  try {
    pqxx::result res = select("CHATS", where);
    std::vector<std::string> participants;
    std::vector<std::shared_ptr<IMessage>> messages;
    pqxx::result::const_iterator c = res.begin();
    chat.set_chat_id(c.at(0).as<std::string>());
    chat.set_chat_name(c.at(1).as<std::string>());
    participants = get_participants_from_chat(chat);
    chat.set_participants(participants);
    messages = get_last_N_messages_from_chat(chat, NUM_OF_LAST_MESSAGES);
    chat.set_chat_messages(messages);
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG CHAT" << std::endl;
  }
  return chat;
}

// Получение всех сущностей чатов из БД
auto Postgre_DB::get_all_chats_by_user_login(const std::string &login)
    -> std::vector<Chat> {
  std::vector<Chat> chats;
  std::string where =
      "user_id = '" + remove_danger_characters(get_user_id(login)) + "'";
  std::vector<std::string> what = {"chat_id"};
  pqxx::result res = select("USERS_CHATS_LINK", where, what);
  try {
    for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
      Chat chat = get_chat_by_id(c.at(0).as<std::string>());
      chats.push_back(chat);
    }
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "wrong get chats" << std::endl;
  }
  return chats;
}

// Получение последних N (если указано) сообщений
auto Postgre_DB::get_last_N_messages_from_chat(const Chat &chat,
                                               int num_of_messages = -1)
    -> std::vector<std::shared_ptr<IMessage>> {
  std::vector<std::shared_ptr<IMessage>> messages;
  std::string where =
      "chat_id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
  std::vector<std::string> what = {"id", "user_from", "chat_id", "content",
                                   "is_read"};
  pqxx::result res = select("MESSAGES", where, what, num_of_messages);

  try {
    for (const auto &el : res) {
      std::shared_ptr<IMessage> message = std::make_shared<TextMessage>();
      message->set_message_id(el.at(0).as<std::string>());
      message->set_sender_id(el.at(1).as<std::string>());
      message->set_parent_chat_id(el.at(2).as<std::string>());
      message->set_message_content(el.at(3).as<std::string>());
      message->set_read_status(false);
      messages.emplace_back(message);
    }
    if (num_of_messages > 0) {
      std::reverse(messages.begin(), messages.end());
    }
    res.clear();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Wrong get chats" << std::endl;
  }
  return messages;
}

// Изменение имени сущности чата в БД
auto Postgre_DB::change_chat_name(Chat &chat, const std::string &new_chat_name)
    -> int {
  if (!find_chat_by_chat_name(chat.get_chat_name())) {
    return _UPDATE_FAULT;
  }
  std::string where =
      "chat_name = '" + remove_danger_characters(chat.get_chat_name()) + "'";
  std::vector<std::string> values = {new_chat_name};
  std::vector<std::string> table_fields = {"chat_name"};
  try {
    update("CHATS", table_fields, values, where);
    chat.set_chat_name(new_chat_name);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return _UPDATE_FAULT;
  }
  return _EXIT_SUCCESS;
}

// Добавление нового пользователя в чат
auto Postgre_DB::add_new_participant(const User &user, const Chat &chat)
    -> int {
  return add_user_chat_link(user.get_id(), chat.get_chat_id());
}
