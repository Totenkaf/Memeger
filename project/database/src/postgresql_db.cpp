// Copyright 2022 by Artem Ustsov

#include "models.h"
#include "postgresql_db.h"
#include <algorithm>

// Конструктор с уставкой. Подлкючается к БД на указанном порте, хосте
Postgre_DB::Postgre_DB(const std::string db_host, const std::string db_port,
                       const std::string db_name, const std::string db_user,
                       const std::string db_password)
: db_host_(db_host), db_port_(db_port), db_name_(db_name), db_user_(db_user), db_password_(db_password)
{
  std::string request = "dbname = " + db_name_ + " user = " + db_user_ +
                        " password = " + db_password_ +
                        " hostaddr = " + db_host_ + " port = " + db_port_;
  try {
    PG_conn = std::make_shared<pqxx::connection>(request);
    if (!PG_conn->is_open()) {
      std::cerr << "Can't open database" << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  init_tables();
}

Postgre_DB::~Postgre_DB() {
  if (PG_conn->is_open()) {
    PG_conn->disconnect();
  }
}

// Создание воркеров для отправки запросов в БД
int Postgre_DB::execution_table(const std::string& table_name, std::shared_ptr<pqxx::connection>& PG_conn) {
  pqxx::work N(*PG_conn);
  try {
    N.exec(table_name);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "TABLE " << table_name << " execution fault" << std::endl;
    return 1;
  }
  N.commit();
  return 0;
}

// Инициализация пустых таблиц для хранения информации о сущностях
int Postgre_DB::init_tables() {
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
  if(execution_table(users, PG_conn)) {
    return 1;
  }
  
  std::string chats =
      create_table +
      "CHATS (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY, \
                                               chat_name VARCHAR(50) NOT NULL DEFAULT 'with_yourself', \
                                               CONSTRAINT unique_chat_name UNIQUE (chat_name), \
                                               time_creation TIMESTAMP NOT NULL DEFAULT NOW());";
  if(execution_table(chats, PG_conn)) {
    return 1;
  }

  std::string messages =
      create_table +
      "MESSAGES (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY, \
                                                     user_from UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE, \
                                                     time_sent TIMESTAMP NOT NULL DEFAULT NOW(), \
                                                     chat_id UUID NOT NULL REFERENCES CHATS(id) ON DELETE CASCADE, \
                                                     content VARCHAR(150), \
                                                     is_read BOOLEAN NOT NULL DEFAULT false);";
  if(execution_table(messages, PG_conn)) {
    return 1;
  }

  std::string users_chats_link =
      create_table +
      "USERS_CHATS_LINK (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY, \
                                                                     user_id UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE, \
                                                                     chat_id UUID NOT NULL REFERENCES CHATS(id) ON DELETE CASCADE);";
  if(execution_table(users_chats_link, PG_conn)) {
    return 1;
  }
  return 0;
}

// Удаление таблиц из БД
int Postgre_DB::drop_tables() {
  std::string drop_table = "DROP TABLE ";

  std::string users = drop_table + "USERS CASCADE;";
  if(execution_table(users, PG_conn)) {
    return 1;
  }

  std::string messages = drop_table + "MESSAGES CASCADE;";
  if(execution_table(messages, PG_conn)) {
    return 1;
  }

  std::string chats = drop_table + "CHATS CASCADE;";
  if(execution_table(chats, PG_conn)) {
    return 1;
  }

  std::string users_chats_link = drop_table + "USERS_CHATS_LINK CASCADE;";
  if(execution_table(users_chats_link, PG_conn)) {
    return 1;
  }
  return 0;
}

// Удаление лишних символов, которые могут помешать парсингу (/, '', ;)
std::string Postgre_DB::remove_danger_characters(
    const std::string& row_column) {
  std::string prep_column = "";
  for (auto i : row_column) {
    if (i != '\'' && i != '\"') {
      prep_column += i;
    }
  }
  return prep_column;
}

// Парсинг имен таблиц в запросах
std::string Postgre_DB::parse_table_fields(
    const std::vector<std::string>& fields) {
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
std::string Postgre_DB::parse_table_values(
    const std::vector<std::string>& values) {
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
std::string Postgre_DB::parse_output_params(
    const std::vector<std::string>& output_params) {
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
int Postgre_DB::insert(const std::string& table,
                       const std::vector<std::string>& table_fields,
                       const std::vector<std::string>& values,
                       std::vector<std::string>& output_params) {
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
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Обертка над запросом на выбор данных из БД
pqxx::result Postgre_DB::select(
    const std::string& table, const std::string where = std::string(),
    const std::vector<std::string> what = std::vector<std::string>(),
    int limit = -1) {
  std::string request = "SELECT ";
  if (!what.empty()) {
    for (std::size_t i = 0; i < what.size(); ++i) {
      request += remove_danger_characters(what[i]) + ", ";
    }
    request =
        request.substr(0, request.size() - 2);  // MAGIC NUMBER. TO DO SMART
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
int Postgre_DB::update(const std::string& table,
                       const std::vector<std::string>& table_fields,
                       const std::vector<std::string>& values,
                       const std::string where = std::string()) {
  pqxx::work N(*PG_conn);
  try {
    std::string request = "UPDATE " + table + " SET ";
    for (size_t i = 0; i < table_fields.size(); ++i) {
      request += table_fields[i] + " = '" +
                 remove_danger_characters(values[i]) + "', ";
    }
    request =
        request.substr(0, request.size() - 2);  // MAGIC NUMBER. TO DO SMART
    if (!where.empty()) {
      request += " WHERE " + where;
    }
    request += ";";
    N.exec(request);
    N.commit();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Обертка над сохранением записи в БД
int Postgre_DB::save(const std::string& table,
                     const std::vector<std::string>& table_fields,
                     const std::vector<std::string>& values,
                     std::vector<std::string>& output_params,
                     const std::string where = std::string()) {
  pqxx::result check = select(table, where);
  try {
    if ((!where.empty()) && (!check.empty())) { 
      check.clear();
      return update(table, table_fields, values, where);
    } else {
      check.clear();
      return insert(table, table_fields, values, output_params);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    check.clear();
    return 1;
  }
}

// Обертка над удалением записи из конкретной таблицы БД
int Postgre_DB::delete_(const std::string& table,
                        const std::string where = std::string()) {
  pqxx::work N(*PG_conn);
  std::string request = "DELETE FROM " + table;
  if (!where.empty()) {
    request += " WHERE " + where;
  }
  request += ";";
  try {
    N.exec(request);
    N.commit();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Добавление нового пользователя в таблицу USERS в БД
int Postgre_DB::add_user(User& user) {
  std::vector<std::string> data = {user.get_login(), user.get_password()};
  std::vector<std::string> table_fields = {"login", "password"};
  std::vector<std::string> output_params = {"id", "active_status"};
  try {
    save("USERS", table_fields, data, output_params);
    if (output_params[0] == "id" || output_params[1] == "active_status") {
      return 1;
    }
    user.set_id(output_params[0]);
    user.set_active_status(output_params[1]);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Получение ID пользователя из БД
std::string Postgre_DB::get_user_id(const std::string& login) {
  std::string where = "login = '" + login + "'";
  std::vector<std::string> what = {"id"};
  std::string user_id;

  try {
    pqxx::result res = select("USERS", where, what);
    pqxx::result::const_iterator c = res.begin();
    user_id = c[0].as<std::string>();
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return user_id;
  }
  return user_id;
}

// Получение логина пользователя из БД
std::string Postgre_DB::get_user_login(const std::string& id) {
  std::string where = "id = '" + id + "'";
  std::vector<std::string> what = {"login"};
  std::string user_login;
  try {
      pqxx::result res = select("USERS", where, what);
      pqxx::result::const_iterator c = res.begin();
      user_login = c.at(0).as<std::string>();
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG USER" << std::endl;
  }
  return user_login;
}

// Получение сущности пользователя по логину
User Postgre_DB::get_user_by_login(const std::string& login) {
  std::string where = "login = '" + login + "'";
  User user;
  try {
    pqxx::result res = select("USERS", where);
    if(!res.empty()) {
      pqxx::result::const_iterator c = res.begin();
      user.set_id(c.at(0).as<std::string>());
      user.set_login(c.at(1).as<std::string>());
      user.set_password(c.at(2).as<std::string>());
      user.set_active_status(c.at(3).as<std::string>());
    }
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG USER" << std::endl;
  }
  return user;
}

// Поиск сущности пользователя по логину в БД
bool Postgre_DB::find_user_by_login(const std::string& login) {
  std::string where = "login = '" + login + "'";
  try {
    pqxx::result res = select("USERS", where);
    if(!res.empty()) {
      return true;
    }
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return false;
}

// Замена логина сущности пользователя в БД
int Postgre_DB::change_user_login(User& user, const std::string& new_login) {
  if (!find_user_by_login(user.get_login())) {
    return 1;
  }
  std::string where =
      "login = '" + remove_danger_characters(user.get_login()) + "'";
  std::vector<std::string> values = {new_login};
  std::vector<std::string> table_fields = {"login"};
  try {
    int update_status = update("USERS", table_fields, values, where);
    if (!update_status) {
      user.set_login(new_login);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Замена статуса работы пользователя
int Postgre_DB::change_user_status(User& user, const std::string& new_status) {
  if (!find_user_by_login(user.get_login())) {
    return 1;
  }
  std::string where =
      "login = '" + remove_danger_characters(user.get_login()) + "'";
  std::vector<std::string> values = {new_status};
  std::vector<std::string> table_fields = {"active_status"};
  try {
    int update_status = update("USERS", table_fields, values, where);
    if (!update_status) {
      user.set_active_status(new_status);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Замена пароля пользователя в БД
int Postgre_DB::change_user_password(User& user,
                                     const std::string& new_password) {
  if (!find_user_by_login(user.get_login())) {
    return 1;
  }
  std::string where =
      "login = '" + remove_danger_characters(user.get_login()) + "'";
  std::vector<std::string> values = {new_password};
  std::vector<std::string> table_fields = {"password"};
  try {
    int update_status = update("USERS", table_fields, values, where);
    if (!update_status) {
      user.set_password(new_password);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Удаление пользователя из БД, удаляются все связанные линки
int Postgre_DB::delete_user(User& user) {
  if (!user.get_id().empty()) {
    std::string where =
        "id = '" + remove_danger_characters(user.get_id()) + "'";
        int delete_status = delete_("USERS", where);
        if(!delete_status) {
          user.clear_user();
          return 0;
        }
  }
    return 1;
}

// Удаление сообщения из БД
int Postgre_DB::delete_message(TextMessage& message) {
  if (!message.get_message_id().empty()) {
    std::string where =
        "id = '" + remove_danger_characters(message.get_message_id()) + "'";
    return delete_("MESSAGES", where);
  }
  return 1;
}

// Добавление сообщения в БД
int Postgre_DB::add_message(TextMessage& message) {
  std::vector<std::string> data = {message.get_sender_id(),
                                   message.get_parent_chat_id(),
                                   message.get_message_text()};
  std::vector<std::string> table_fields = {"user_from", "chat_id", "content"};
  try {
    std::vector<std::string> output_params = {"id"};
    save("MESSAGES", table_fields, data, output_params);
    message.set_message_id(output_params[0]);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG ADD USER" << std::endl;
    return 1;
  }
  return 0;
}

// Добавление связи конкретного пользователя с конкретным чатом
int Postgre_DB::add_user_chat_link(const std::string& user_id,
                                   const std::string& chat_id) {
  std::vector<std::string> data = {user_id, chat_id};
  std::vector<std::string> table_fields = {"user_id", "chat_id"};
  std::vector<std::string> output_params = {};
  try {
    save("USERS_CHATS_LINK", table_fields, data, output_params);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG USERS CHATS linking" << std::endl;
    return 1;
  }
  return 0;
}

// Добавление сущности чата в БД
int Postgre_DB::add_chat(Chat& chat) {
  std::vector<std::string> data = {chat.get_chat_name()};
  std::vector<std::string> table_fields = {"chat_name"};
  std::vector<std::string> output_params = {"id"};
  try {
    save("CHATS", table_fields, data, output_params);
    if (output_params[0] == "id") {
      return 1;
    }
    chat.set_chat_id(output_params[0]);
    std::vector<std::string> participants = chat.get_participants();
    for (const auto& participant : participants) {
      add_user_chat_link(get_user_id(participant), chat.get_chat_id());
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Получение сущности чата из БД
Chat Postgre_DB::get_chat_by_chat_name(const std::string& chat_name) {
  static constexpr size_t NUM_OF_LAST_MESSAGES = 25;
  std::string where =
      "chat_name = '" + chat_name + "'";
  pqxx::result res = select("CHATS", where);
  Chat chat;
  try {
    pqxx::result::const_iterator c = res.begin();
    chat.set_chat_id(c.at(0).as<std::string>());
    chat.set_chat_name(c.at(1).as<std::string>());
    std::vector<std::string> participants;
    participants = get_participants_from_chat(chat);
    chat.set_participants(participants);

    std::vector<TextMessage> messages;
    messages = get_last_N_messages_from_chat(chat, NUM_OF_LAST_MESSAGES);
    chat.set_chat_messages(messages);
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG CHAT_NAME" << std::endl;
  }
  return chat;
}

// Удаление сущности чата из БД, удалятся все связанные линки
int Postgre_DB::delete_chat(Chat& chat) {
  if (!chat.get_chat_id().empty()) {
    std::string where =
        "id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
      int delete_status = delete_("CHATS", where); // возможно, стоит избавиться от статуса удаления
        if(!delete_status) {
          chat.clear_chat();
          return 0;
        }
  }
    return 1;
}

// Поиск сущности чата в БД
bool Postgre_DB::find_chat_by_chat_name(const std::string& chat_name) {
  std::string where =
      "chat_name = '" + remove_danger_characters(chat_name) + "'";
  try {
    pqxx::result res = select("CHATS", where);
    res.clear();
  } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return false;
  }
  return true;
}

// Получение списка имен пользователей из чата
std::vector<std::string> Postgre_DB::get_participants_from_chat(
    const Chat& chat) {
  std::string where =
      "chat_id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
  std::vector<std::string> what = {"user_id"};
  pqxx::result res = select("USERS_CHATS_LINK", where, what);
  std::vector<std::string> participants;
  try {
    for (const auto& el : res) {
      std::string user_login = get_user_login(el.at(0).as<std::string>());
      participants.push_back(user_login);
    }
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG PARTICIPANTS" << std::endl;
  }
  return participants;
}

// Получение сущности чата по ID
Chat Postgre_DB::get_chat_by_id(const std::string& chat_id) {
  static constexpr size_t NUM_OF_LAST_MESSAGES = 25;
  std::string where = "id = '" + remove_danger_characters(chat_id) + "'";
  Chat chat;
  try {
    pqxx::result res = select("CHATS", where);
    std::vector<std::string> participants;
    std::vector<TextMessage> messages;
    pqxx::result::const_iterator c = res.begin();
    chat.set_chat_id(c.at(0).as<std::string>());
    chat.set_chat_name(c.at(1).as<std::string>());
    participants = get_participants_from_chat(chat);
    chat.set_participants(participants);
    messages = get_last_N_messages_from_chat(chat, NUM_OF_LAST_MESSAGES);
    chat.set_chat_messages(messages);
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "WRONG CHAT" << std::endl;
  }
  return chat;
}

// Получение всех сущностей чатов из БД
std::vector<Chat> Postgre_DB::get_all_chats_by_user_login(
    const std::string& login) {
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
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "wrong get chats" << std::endl;
  }
  return chats;
}

// Получение последних N (если указано) сообщений
std::vector<TextMessage> Postgre_DB::get_last_N_messages_from_chat(
    const Chat& chat, int num_of_messages = -1) {
  std::vector<TextMessage> messages;
  std::string where =
      "chat_id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
  std::vector<std::string> what = {"id", "user_from", "chat_id", "content",
                                   "is_read"};
  pqxx::result res = select("MESSAGES", where, what, num_of_messages);

  try {
    for (const auto& el : res) {
      TextMessage message;
      message.set_message_id(el.at(0).as<std::string>());
      message.set_sender_id(el.at(1).as<std::string>());
      message.set_parent_chat_id(el.at(2).as<std::string>());
      message.set_message_text(el.at(3).as<std::string>());
      message.set_read_status(false);
      messages.push_back(message);
    }
    if (num_of_messages > 0) {
      std::reverse(messages.begin(), messages.end());
    }
    res.clear();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Wrong get chats" << std::endl;
  }
  return messages;
}

// Изменение имени сущности чата в БД
int Postgre_DB::change_chat_name(Chat& chat, const std::string& new_chat_name) {
  if (!find_chat_by_chat_name(chat.get_chat_name())) {
    return 1;
  }
  std::string where =
      "chat_name = '" + remove_danger_characters(chat.get_chat_name()) + "'";
  std::vector<std::string> values = {new_chat_name};
  std::vector<std::string> table_fields = {"chat_name"};
  try {
    update("CHATS", table_fields, values, where);
    chat.set_chat_name(new_chat_name);
  } 
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

// Добавление нового пользователя в чат
int Postgre_DB::add_new_participant(const User& user, const Chat& chat) {
  return add_user_chat_link(user.get_id(), chat.get_chat_id());
}
