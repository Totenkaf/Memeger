//Copyright 2022 by Artem Ustsov

#include "models.h"
#include "postgresql_db.h"
#include <algorithm>

Postgre_DB::Postgre_DB() 
: db_host_("127.0.0.1"), db_port_("5432"), db_name_("postgres"), db_user_("postgres"), db_password_("postgres")
{
    std::string request = "dbname = " + db_name_ + " user = " + db_user_ + " password = " + db_password_ + " hostaddr = " + db_host_ + " port = " + db_port_;
    try {
        PG_conn = std::make_shared <pqxx::connection>(request);

        if (!PG_conn->is_open()) {
            std::cerr << "Can't open database" << std::endl;        
        } else {
            std::cout << "Database has been successfully opened" << std::endl;
        }
    }
    catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    init_tables();
}

Postgre_DB::Postgre_DB(std::string db_host, std::string db_port, std::string db_name, 
                       std::string db_user, std::string db_password) {

    std::string request = "dbname = " + db_name + " user = " + db_user + " password = " + db_password + " hostaddr = " + db_host + " port = " + db_port;
    try {
        PG_conn = std::make_shared <pqxx::connection>(request);
        
        if (!PG_conn->is_open()) {
            std::cerr << "Can't open database" << std::endl;        
        } else {
            std::cout << "Database has been successfully opened" << std::endl;
        }
    }
    catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    init_tables();
}

Postgre_DB::~Postgre_DB() {
    if (PG_conn->is_open()) {
        PG_conn->disconnect();
    }
}


int Postgre_DB::init_tables() {
    std::string create_table = "CREATE TABLE IF NOT EXISTS ";
    
    std::string users = create_table + "USERS (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                               login VARCHAR(50) NOT NULL,\
                                               CONSTRAINT unique_login_users UNIQUE(login),\
                                               password VARCHAR(50) NOT NULL,\
                                               active_status VARCHAR(8) NOT NULL DEFAULT 'active',\
                                               CONSTRAINT real_active_statuses CHECK(active_status = 'active' OR active_status = 'inactive'),\
                                               time_creation TIMESTAMP NOT NULL DEFAULT NOW()\
                                               );";

    std::string chats = create_table + "CHATS (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                               chat_name VARCHAR(50) NOT NULL DEFAULT 'with_yourself',\
                                               CONSTRAINT unique_chat_name UNIQUE (chat_name),\
                                               time_creation TIMESTAMP NOT NULL DEFAULT NOW()\
                                               );";


    std::string messages = create_table + "MESSAGES (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                                     user_from UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE,\
                                                     user_to UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE,\
                                                     time_sent TIMESTAMP NOT NULL DEFAULT NOW(),\
                                                     chat_id UUID NOT NULL REFERENCES CHATS(id) ON DELETE CASCADE,\
                                                     content VARCHAR(150),\
                                                     is_read BOOLEAN NOT NULL DEFAULT false\
                                                     );";

    std::string users_chats_link = create_table + "USERS_CHATS_LINK (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                                                     user_id UUID NOT NULL REFERENCES USERS(id) ON DELETE CASCADE,\
                                                                     chat_id UUID NOT NULL REFERENCES CHATS(id) ON DELETE CASCADE\
                                                                     );";

    pqxx::work N(*PG_conn);
    try {
        N.exec(users);
    }
    catch (const std::exception &e) {
        std::cerr << "TABLE USERS execution fault" << std::endl;
        return 1;
    }
    N.commit();

    pqxx::work N1(*PG_conn);
    try {
        N1.exec(chats);
    }
    catch (const std::exception &e) {
        std::cerr << "TABLE CHATS execution fault" << std::endl;
        return 1;
    }
    N1.commit();

    pqxx::work N2(*PG_conn);
    try {
        N2.exec(messages);
    }
    catch (const std::exception &e) {
        std::cerr << "TABLE MESSAGES execution fault" << std::endl;
        return 1;
    }
    N2.commit();

    pqxx::work N3(*PG_conn);
    try {
        N3.exec(users_chats_link);
    }
    catch (const std::exception &e) {
        std::cerr << "TABLE USERS_CHATS_LINK execution fault" << std::endl;
        return 1;
    }
    N3.commit();

    std::cout << "All data tables have been initialized successfully" << std::endl;
    return 0;
}

int Postgre_DB::drop_tables() {
    std::string drop_table = "DROP TABLE ";

    std::string users = drop_table + "USERS CASCADE;";
    std::string messages = drop_table + "MESSAGES CASCADE;";
    std::string chats = drop_table + "CHATS CASCADE;";
    std::string users_chats_link = drop_table + "USERS_CHATS_LINK CASCADE;";

    pqxx::work N(*PG_conn);
    try {
        N.exec(users);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE USERS execution fault" << std::endl;
    }
    N.commit();

    pqxx::work N1(*PG_conn);
    try {
        N1.exec(messages);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE MESSAGES execution fault" << std::endl;
    }
    N1.commit();

    pqxx::work N2(*PG_conn);
    try {
        N2.exec(chats);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE CHATS execution fault" << std::endl;
    }
    N2.commit();

    pqxx::work N3(*PG_conn);
    try {
 
        N3.exec(users_chats_link);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE USERS_CHATS_LINK execution fault" << std::endl;
    }
    N3.commit();

    std::cout << "All data tables have been deleted successfully" << std::endl; /*сделать callback функцию*/
    return 0;
}

std::string Postgre_DB::remove_danger_characters(const std::string& row_column) {
    std::string prep_column = "";
    for (auto i : row_column) {
        if (i != '\'' and i != '\"') {
            prep_column += i;
        }
    }
    return prep_column;
}

std::string Postgre_DB::parse_table_fields(const std::vector<std::string>& fields) {
    std::string request = " (";
    for (size_t i = 0; i < fields.size(); ++i) {
        if(i == fields.size() - 1) {
            request += remove_danger_characters(fields[i]) + ")";
            continue;
        }
        request += remove_danger_characters(fields[i]) + ", ";
    }
    return request;
}

std::string Postgre_DB::parse_table_values(const std::vector<std::string>& values) {
    std::string request = " VALUES('";
    for (size_t i = 0; i < values.size(); ++i) {
        if(i == values.size() - 1) {
            request += remove_danger_characters(values[i]) + "') ";
            continue;
        }
        request += remove_danger_characters(values[i]) + "', '";
    }
    return request;
}

std::string Postgre_DB::parse_output_params(const std::vector<std::string>& output_params) {
    std::string request = " ";
    for (size_t i = 0; i < output_params.size(); ++i) {
        if(i == output_params.size() - 1) {
            request += remove_danger_characters(output_params[i]);
            continue;
        }
        request += remove_danger_characters(output_params[i]) + ",";
    }
    return request;
}

int Postgre_DB::insert(const std::string& table, const std::vector<std::string>& table_fields, 
                       const std::vector<std::string>& values, std::vector<std::string>& output_params) {
    std::cout << "Зашли в insert" << std::endl;
    pqxx::work N(*PG_conn);
    try {
        std::string request = "INSERT INTO " + table + parse_table_fields(table_fields) + parse_table_values(values);
        std::cout << "Собираем запрос: " << request << std::endl;

        if (!output_params.empty()) {
            request += "RETURNING" + parse_output_params(output_params) + ";";
        }
        std::cout << "Собрали запрос: " << request << std::endl;

        pqxx::result res = N.exec(request);
        if (!output_params.empty()) {
            pqxx::result::const_iterator c = res.begin();
            for (int i = 0; i < static_cast<int>(output_params.size()); ++i) {
                output_params.at(i) = c.at(i).as<std::string>();
            }
        }
        N.commit();
        res.clear();
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

pqxx::result Postgre_DB::select(const std::string& table, std::string where = std::string(), 
                                std::vector<std::string> what = std::vector <std::string>(),
                                int limit = -1) {
    std::string request = "SELECT ";
    if (what.size() > 0) {
        for (std::size_t i = 0; i < what.size(); ++i) {
            request += remove_danger_characters(what[i]) + ", ";
        }
        request = request.substr(0, request.size() - 2);
    }
    else {
        request += "*";
    }
    request += " FROM " + table;
    if (where != "") {
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

int Postgre_DB::update(const std::string& table, const std::vector<std::string>& table_fields, 
                       const std::vector<std::string>& values, std::string where = std::string()) {
    std::cout << "Зашли в UPDATE " << std::endl;

    pqxx::work N(*PG_conn);
    std::cout << "Сделали соединение к БД " << std::endl;

    try {
        std::cout << "Собираем UPDATE " << std::endl;
        std::string request = "UPDATE " + table + " SET ";
        std::cout << "Запрос " << request << std::endl;

        for (size_t i = 0; i < table_fields.size(); ++i) {
            request += table_fields[i] + " = '" + remove_danger_characters(values[i]) + "', ";
            std::cout << "Итерация " << i;
            std::cout << ": Запрос после итерации " << request << std::endl;
        }
        request = request.substr(0, request.size() - 2);

        if (where != "") {
            request += " WHERE " + where;
            
        }
        request += ";";
        std::cout << "Собрали request " << request << std::endl;
        N.exec(request);
        N.commit();
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::save(const std::string& table, const std::vector<std::string>& table_fields, const std::vector<std::string>& values, 
                     std::vector<std::string>& output_params, std::string where = std::string()) {

    std::cout << "Зашли в save" << std::endl;
    pqxx::result check = select(table, where);
    try {
        if((where != "") && (check.begin() != check.end())) {
            check.clear();
            return update(table, table_fields, values, where);
        }
        else {
            check.clear();
            return insert(table, table_fields, values, output_params);
        }
    }
    catch (const std::exception &e) {
        check.clear();
        return 1;
    }
}

int Postgre_DB::delete_(const std::string& table, std::string where = std::string()) {
    pqxx::work N(*PG_conn);
    std::string request;
    request = "DELETE FROM " + table;
    if (where != "") {
        request += " WHERE " + where;
    }
    request += ";";
    try {
        N.exec(request);
        N.commit();
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

int Postgre_DB::add_user(User& user) {
    std::cout << "Зашли в add_user" << std::endl;
    std::vector<std::string> data = {user.get_login(), user.get_password()};
    std::vector<std::string> table_fields = {"login", "password"};
    std::vector<std::string> output_params = {"id", "active_status"};

    try {
        if(save("USERS", table_fields, data, output_params)) {
            return 1;
        }
        user.set_id(output_params[0]);
        user.set_active_status(output_params[1]);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

std::string Postgre_DB::get_user_id(const std::string& login) {
    std::string where = "login = '" + remove_danger_characters(login) + "'";
    std::vector<std::string> what = {"id"};
    pqxx::result res = select("USERS", where, what);
    std::string user_id;

    try {
        if (res.begin() != res.end()) {
            pqxx::result::const_iterator c = res.begin();
            user_id =  c[0].as<std::string>();
        }
        res.clear();
    }
    catch (const std::exception &e) {
        return user_id;
    }
    return user_id;
}

std::string Postgre_DB::get_user_login(const std::string& id) {
    std::cout << "Зашли в get_user_login" << std::endl;
    std::string where = "id = '" + remove_danger_characters(id) + "'"; 
    std::vector<std::string> what = {"login"};
    pqxx::result res = select("USERS", where, what);
    std::string user_login;

    try {
        if (res.begin() != res.end()) {
            pqxx::result::const_iterator c = res.begin();
            user_login =  c.at(0).as<std::string>();
            std::cout << "Получили логин " << user_login << std::endl;
        }
        res.clear();
    }
    catch (const std::exception &e) {
    }
    return user_login;
}


User Postgre_DB::get_user_by_login(const std::string& login) {
    std::string where = "login = '" + remove_danger_characters(login) + "'"; 
    pqxx::result res = select("USERS", where);
    User user;
    try {
        if (res.begin() != res.end()) {
            pqxx::result::const_iterator c = res.begin();
            user.set_id( c.at(0).as<std::string>() );
            user.set_login( c.at(1).as<std::string>() );
            user.set_password( c.at(2).as<std::string>() );
            user.set_active_status( c.at(3).as<std::string>() );
        }
        res.clear();
    }
    catch (const std::exception &e) {
    }
    return user;
}


bool Postgre_DB::find_user_by_login(const std::string& login) {
    std::string where = "login = '" + remove_danger_characters(login) + "'";
    try {
        pqxx::result res = select("USERS", where);
        if (res.begin() != res.end()) {
            res.clear();
            return true;
        }
    }
    catch (const std::exception &e) {
        return false;
    }
    return false;
}

int Postgre_DB::change_user_login(User& user, const std::string& new_login) {
    std::string where = "login = '" + remove_danger_characters(user.get_login()) + "'";

    std::vector<std::string> values = {new_login};
    std::vector<std::string> table_fields = {"login"};

    try {
        int update_status = update("USERS", table_fields, values, where);
        if (!update_status) {
            user.set_login(new_login);
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::change_user_status(User& user, const std::string& new_status) {
    std::cout << "Зашли внутрь change_user_status " << std::endl;
    if(find_user_by_login(user.get_login())) {
        std::cout << "Нашли пользователя " << std::endl;
        std::string where = "login = '" + remove_danger_characters(user.get_login()) + "'";

        std::cout << "Собрали where " << where << std::endl;
        std::vector<std::string> values = {new_status};
        std::vector<std::string> table_fields = {"active_status"};
        
        try {
            std::cout << "Пробуем UPDATE " << std::endl;
            int update_status = update("USERS", table_fields, values, where);
            if (!update_status) {
                std::cout << "Установили новый статус " << new_status << std::endl;
                user.set_active_status(new_status);
            }
        }
        catch (const std::exception &e) {
            return 1;
        }
        return 0;
    } else {
        return 1;
    }
}


int Postgre_DB::change_user_password(User& user, const std::string& new_password) {
    if(find_user_by_login(user.get_login())) {
        std::string where = "login = '" + remove_danger_characters(user.get_login()) + "'";

        std::vector<std::string> values = {new_password};
        std::vector<std::string> table_fields = {"password"};

        try {
            int update_status = update("USERS", table_fields, values, where);
            if (!update_status) {
                user.set_password(new_password);
            }
        }
        catch (const std::exception &e) {
            return 1;
        }
        return 0;
    } else {
        return 1;
    }
}

/* добавить удаление из таблиц USERS_CHATS_LINK*/
int Postgre_DB::delete_user(User& user) {
    if (user.get_id() == "") {
        return 1;
    }
    else {  
        std::string where = "id = '" + remove_danger_characters(user.get_id()) + "'";
        return delete_("USERS", where);
    }
}


int Postgre_DB::delete_message(TextMessage& message) {
    if (message.get_message_id() == "") {
        return 1;
    }
    else {  
        std::string where = "id = '" + remove_danger_characters(message.get_message_id()) + "'";
        return delete_("MESSAGES", where);
    }
}


int Postgre_DB::add_message(TextMessage& message) {
    std::vector <std::string> data = {message.get_sender_id(),
                                      message.get_address_id(),
                                      message.get_parent_chat_id(),
                                      message.get_message_text()};
    std::vector <std::string> table_fields = {"user_from", "user_to", "chat_id", "content"};

    try {
        std::vector<std::string> output_params = {"id"};
        if(save("MESSAGES", table_fields, data, output_params)) {
            return 1;
        }
        message.set_message_id(output_params[0]);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::add_user_chat_link(const std::string& user_id, const std::string& chat_id) {
    std::vector <std::string> data = {user_id, chat_id};
    std::vector <std::string> table_fields = {"user_id", "chat_id"};
    std::vector<std::string> output_params = {};
    try {
        if(save("USERS_CHATS_LINK", table_fields, data, output_params)) {
            return 1;
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::add_chat(Chat& chat) {
    std::vector <std::string> data = {chat.get_chat_name()};
    std::vector <std::string> table_fields = {"chat_name"};
    std::vector<std::string> output_params = {"id"};

    try {
        if(save("CHATS", table_fields, data, output_params)) {
            return 1;
        }
        chat.set_chat_id(output_params[0]);
        std::vector<std::string> participants = chat.get_participants();
        for (auto participant : participants) {
            if(add_user_chat_link(get_user_id(participant), chat.get_chat_id())) {
                return 1;
            }
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}



Chat Postgre_DB::get_chat_by_chat_name(const std::string& chat_name) {
    std::string where = "chat_name = '" + remove_danger_characters(chat_name) + "'"; 
    pqxx::result res = select("CHATS", where);
    Chat chat;

    try {
        if (res.begin() != res.end()) {
            pqxx::result::const_iterator c = res.begin();
            chat.set_chat_id( c.at(0).as<std::string>() );
            chat.set_chat_name( c.at(1).as<std::string>() );

        }
        res.clear();
    }
    catch (const std::exception &e) {
        std::cerr << "WRONG CHAT_NAME" << std::endl;
    }

    std::vector<std::string> participants = get_participants_from_chat(chat);
    chat.set_participants(participants);
    return chat;
}


/* добавить удаление из таблиц USERS_CHATS_LINK*/
int Postgre_DB::delete_chat(Chat& chat) {
    if (chat.get_chat_id() == "") {
        return 1;
    }
    else {  
        std::string where = "id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
        return delete_("CHATS", where);
    }
}

bool Postgre_DB::find_chat_by_chat_name(const std::string& chat_name) {
    std::string where = "chat_name = '" + remove_danger_characters(chat_name) + "'";
    try {
        pqxx::result res = select("CHATS", where);
        if (res.begin() != res.end()) {
            res.clear();
            return true;
        }
    }
    catch (const std::exception &e) {
        return false;
    }
    return false;
}

std::vector<std::string> Postgre_DB::get_participants_from_chat(const Chat& chat) {
    std::vector<std::string> participants;
    std::string where = "chat_id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
    std::vector<std::string> what = {"user_id"};
    pqxx::result res = select("USERS_CHATS_LINK", where, what);

    try {
        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            std::string user_login = get_user_login(c.at(0).as<std::string>());
            std::cerr << "Вернули логин " << user_login << std::endl;
            participants.push_back(user_login);
        }
        res.clear();
    }
    catch (const std::exception &e) {
        std::cerr << "WRONG PARTICIPANTS" << std::endl;
    }
    return participants;
}


Chat Postgre_DB::get_chat_by_id(const std::string& chat_id) {
    std::cerr << "Зашли в  get_chat_by_id" << std::endl;
    std::string where = "id = '" + remove_danger_characters(chat_id) + "'"; 
    std::cerr << "Запрос where " << where << std::endl;
    pqxx::result res = select("CHATS", where);
    Chat chat;
    try {
        if (res.begin() != res.end()) {
            pqxx::result::const_iterator c = res.begin();
            std::cerr << c.at(0).as<std::string>() << std::endl;
            chat.set_chat_id( c.at(0).as<std::string>() );

            std::cerr << c.at(1).as<std::string>() << std::endl;
            chat.set_chat_name( c.at(1).as<std::string>() );
        }
        res.clear();
    }
    catch (const std::exception &e) {
        std::cerr << "WRONG CHAT" << std::endl;
    }
    return chat;
}


std::vector<Chat> Postgre_DB::get_all_chats_by_user_login(const std::string& login) {
    std::cout << "Зашли в  get_all_chats_by_user_login" << std::endl;
    std::vector<Chat> chats;
    std::string where = "user_id = '" + remove_danger_characters(get_user_id(login)) + "'";
    std::cout << "Запрос where " << where << std::endl;
    std::vector<std::string> what = {"chat_id"};
    pqxx::result res = select("USERS_CHATS_LINK", where, what);


    try {
        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            Chat chat = get_chat_by_id(c.at(0).as<std::string>());
            chats.push_back(chat);
        }
    res.clear();
    }
    catch (const std::exception &e) {
        std::cerr << "wrong get chats" << std::endl;
    }


    return chats;
}

std::vector<std::string> Postgre_DB::get_last_N_messages_from_chat(const std::string chat_id, int num_of_messages = -1) {
    std::vector<std::string> messages;
    std::string where = "chat_id = '" + remove_danger_characters(chat_id) + "'";
    std::vector<std::string> what = {"content"};
    pqxx::result res = select("MESSAGES", where, what, num_of_messages);

    try {
        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            std::string message = c.at(0).as<std::string>();
            messages.push_back(message);
        }
        if(num_of_messages > 0) {
            std::reverse(messages.begin(), messages.end());
        }
        res.clear();
    }
    catch (const std::exception &e) {
        std::cerr << "wrong get chats" << std::endl;
    }
    return messages;
}


int Postgre_DB::change_chat_name(Chat& chat, const std::string& new_chat_name) {
    std::cout << "Зашли внутрь change_chat_name " << std::endl;
    if(find_chat_by_chat_name(chat.get_chat_name())) {
        std::cout << "Нашли чат " << std::endl;
        std::string where = "chat_name = '" + remove_danger_characters(chat.get_chat_name()) + "'";

        std::cout << "Собрали where " << where << std::endl;
        std::vector<std::string> values = {new_chat_name};
        std::vector<std::string> table_fields = {"chat_name"};

        try {
            std::cout << "Пробуем UPDATE " << std::endl;
            int update_status = update("CHATS", table_fields, values, where);
            if (!update_status) {
                std::cout << "Установили новое имя чата " << new_chat_name << std::endl;
                chat.set_chat_name(new_chat_name);
            }
        }
        catch (const std::exception &e) {
            return 1;
        }
        return 0;
    } else {
        return 1;
    }
}

int Postgre_DB::add_new_participant(const User& user, const Chat& chat) {
    return add_user_chat_link(user.get_id(), chat.get_chat_id());
}
