//Copyright 2022 by Artem Ustsov

#include "models.h"
#include "postgresql_db.h"


Postgre_DB::Postgre_DB() 
: db_host_("127.0.0.1"), db_port_("5432"), db_name_("postgres"), db_user_("postgres"), db_password_("postgres")
{
    std::string request = "dbname = " + db_name_ + " user = " + db_user_ + " password = " + db_password_ + " hostaddr = " + db_host_ + " port = " + db_port_;
    try {
        PG_conn = std::make_shared <pqxx::connection>(request);

        if (!PG_conn->is_open()) {
            std::cerr << "Can't open database" << std::endl;        
        } else {
            std::cout << "Database has been successfully opened" << std::endl; /*сделать callback функцию*/
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
            std::cout << "Database has been successfully opened" << std::endl; /*сделать callback функцию*/
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
                                               time_creation TIMESTAMP NOT NULL DEFAULT NOW());";

    std::string messages = create_table + "MESSAGES (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                                     user_from VARCHAR(50) NOT NULL,\
                                                     user_to VARCHAR(50) NOT NULL,\
                                                     time_sent TIMESTAMP NOT NULL DEFAULT NOW(),\
                                                     chat_id VARCHAR(50) NOT NULL,\
                                                     content VARCHAR(150),\
                                                     is_read BOOLEAN NOT NULL DEFAULT false);";

    std::string chats = create_table + "CHATS (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                               chat_name VARCHAR(50) NOT NULL DEFAULT 'with_yourself',\
                                               CONSTRAINT unique_chat_name UNIQUE (chat_name),\
                                               time_creation TIMESTAMP NOT NULL DEFAULT NOW());";

    std::string users_chats_link = create_table + "USERS_CHATS_LINK (id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,\
                                                                     user_id VARCHAR(50),\
                                                                     chat_id VARCHAR(50));";

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
        N1.exec(messages);
    }
    catch (const std::exception &e) {
        std::cerr << "TABLE MESSAGES execution fault" << std::endl;
        return 1;
    }
    N1.commit();

    pqxx::work N2(*PG_conn);
    try {
        N2.exec(chats);
    }
    catch (const std::exception &e) {
        std::cerr << "TABLE CHATS execution fault" << std::endl;
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

    std::cout << "All data tables have been initialized successfully" << std::endl; /*сделать callback функцию*/
    return 0;
}

int Postgre_DB::drop_tables() {
    std::string drop_table = "DROP TABLE ";

    std::string users = drop_table + "USERS;";
    std::string messages = drop_table + "MESSAGES;";
    std::string chats = drop_table + "CHATS;";
    std::string users_chats_link = drop_table + "USERS_CHATS_LINK;";

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

int Postgre_DB::insert(const std::string& table, const std::string& table_fields, std::vector<std::string> values, std::string& output_res) {
    pqxx::work N(*PG_conn);
    std::string request;
    try {
        request = "INSERT INTO " + table + table_fields + " VALUES('";
        for (size_t i = 0; i < values.size(); ++i) {
            request += remove_danger_characters(values[i]) + "', '";
        } 
        request = request.substr(0, request.size() - 3);
        request += ") RETURNING id;"; /* возврат id только что добавленной записи в таблицу */
        pqxx::result res = N.exec(request);
        pqxx::result::const_iterator c = res.begin();
        output_res =  c[0].as<std::string>();
        N.commit();
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

pqxx::result Postgre_DB::select(const std::string& table, std::string where = "", std::vector <std::string> what = std::vector <std::string>()) {
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
    request += ";";
    pqxx::nontransaction N(*PG_conn);
    pqxx::result res = N.exec(request);
    N.commit();
    return res;
}

int Postgre_DB::update(const std::string& table, const std::string& table_fields, std::vector <std::string> values, std::string where = std::string()) {
    std::cout << "Зашли в UPDATE " << std::endl;
    std::string request;
    std::string lower_table = "";

    for (auto sym : table) {
        lower_table += tolower(sym);
    }

    std::vector <std::string> column;
    column.push_back("column_name");

    std::string wh = "TABLE_NAME = '" + lower_table + "'";

    pqxx::result columns = select("INFORMATION_SCHEMA.COLUMNS", wh, column);
    
    pqxx::work N(*PG_conn);

    try {
        request = "UPDATE " + table + table_fields + " SET ";
        std::size_t i = 0;
        for (pqxx::result::const_iterator c = columns.begin(); c != columns.end(); ++c) {
            request += c[0].as<std::string>() + " = '" + remove_danger_characters(values[i++]) + "', ";
        }
        if (i < values.size()) {
            return 1;
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
        columns.clear();
        return 1;
    }
    columns.clear();
    return 0;
}


int Postgre_DB::save(const std::string& table, const std::string& table_fiels, std::vector <std::string> values, std::string& output_res, std::string where = "") {
    pqxx::result check = select(table, where);
    try {
        if ((where != "") and (check.begin() != check.end())) {
            check.clear();
            return update(table, table_fiels, values, where);
        }
        else {
            check.clear();
            return insert(table, table_fiels, values, output_res);
        }
    }
    catch (const std::exception &e) {
        check.clear();
        return 1;
    }
}

int Postgre_DB::delete_(const std::string& table, std::string where = "") {
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
    std::vector <std::string> data;

    data.push_back(user.get_login());
    data.push_back(user.get_password());

    try {
        std::string new_id;
        save("USERS", "(login, password)", data, new_id);
        if(new_id.empty()) {
            return 1;
        }
        user.set_id(new_id);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


User Postgre_DB::get_user_by_login(const std::string& login) {
    std::string where = "login = '" + remove_danger_characters(login) + "'"; 
    pqxx::result res = select("USERS", where);
    User user;
    try {
        if (res.begin() != res.end()) { /* придумать посимпатичнее итераирование по объеку, а не перебирать все поля*/
            pqxx::result::const_iterator c = res.begin();
            user.set_id( c[0].as<std::string>() );
            user.set_login( c[1].as<std::string>() );
            user.set_password( c[2].as<std::string>() );
            user.set_active_status( c[3].as<std::string>() );
        }
        res.clear();
    }
    catch (const std::exception &e) {
        return user;
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

    std::vector<std::string> values;
    values.push_back(new_login);
    values.push_back(user.get_password());
    values.push_back(user.get_active_status());

    try {
        int update_status = update("USERS", "(login, password, active_status)", values, where); /* to prettify */
        if (!update_status) {
            user.set_login(new_login);
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

/*аккуратно, работает некорректно */
int Postgre_DB::change_user_status(User& user, const std::string& new_status) {
    /* проверка на равенство логина входящего пользователя и пользователя, который записан в БД */
    std::cout << "Зашли внутрь change_user_status " << std::endl;
    if(find_user_by_login(user.get_login())) {
        std::cout << "Нашли пользователя " << std::endl;
        std::string where = "login = '" + remove_danger_characters(user.get_login()) + "'";

        std::cout << "Собрали where" << where << std::endl;
        std::vector<std::string> values;
        values.push_back(user.get_login());
        values.push_back(user.get_password());
        values.push_back(new_status);

        try {
            std::cout << "Пробуем UPDATE " << std::endl;
            int update_status = update("USERS", "(login, password, active_status)", values, where); /* to prettify */
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
    /* проверка на равенство логина входящего пользователя и пользователя, который записан в БД */
    if(find_user_by_login(user.get_login())) {
        std::string where = "login = '" + remove_danger_characters(user.get_login()) + "'";

        std::vector<std::string> values;
        values.push_back(user.get_login());
        values.push_back(new_password);
        values.push_back(user.get_active_status());

        try {
            int update_status = update("USERS", "(login, password, active_status)", values, where); /* to prettify */
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
    std::vector <std::string> data;

    data.push_back(message.get_sender_id());
    data.push_back(message.get_address_id());
    data.push_back(message.get_parent_chat_id());
    data.push_back(message.get_message_text());

    try {
        std::string new_id;
        save("MESSAGES", "(user_from, user_to, chat_id, content)", data, new_id);
        if(new_id.empty()) {
            return 1;
        }
        message.set_message_id(new_id);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::add_user_chat_link(const User& user, const Chat& chat) {
    std::vector <std::string> data;

    data.push_back(user.get_id());
    data.push_back(chat.get_chat_id());

    try {
        std::string link_id;
        save("USERS_CHATS_LINK", "(user_id, chat_id)", data, link_id);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::add_chat(Chat& chat) {
    std::vector <std::string> data;
    data.push_back(chat.get_chat_name());

    try {
        std::string new_id;
        save("CHATS", "(chat_name)", data, new_id);
        if(new_id.empty()) {
            return 1;
        }
        chat.set_chat_id(new_id);
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
            chat.set_chat_id( c[0].as<std::string>() );
            chat.set_chat_name( c[1].as<std::string>() );
        }
        res.clear();
    }
    catch (const std::exception &e) {
        return chat;
    }
    return chat;
}


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

std::vector<std::string> Postgre_DB::get_participants_from_chat(const Chat& chat) {
    std::vector<std::string> participants;
    std::string where = "chat_id = '" + remove_danger_characters(chat.get_chat_id()) + "'";
    std::vector<std::string> what = {"user_id"}; /* целый вектор для одного значения - жирновато будет */
    pqxx::result res = select("USERS_CHATS_LINK", where, what);

    try {
        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
           participants.push_back(c[0].as<std::string>());
        }
        res.clear();
    }
    catch (const std::exception &e) {
        std::cerr << "wrong get participants" << std::endl; /* как обрабатывать исключения */
    }
    return participants;
}