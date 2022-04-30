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
    std::string users = create_table + "USERS (id UUID NOT NULL PRIMARY KEY, login VARCHAR(50) NOT NULL, password VARCHAR(50) NOT NULL,\
                                               active_status VARCHAR(8) NOT NULL);";

    std::string messages = create_table + "MESSAGES (id UUID NOT NULL PRIMARY KEY, user_from VARCHAR(50) NOT NULL, user_to VARCHAR(50) NOT NULL,\
                                                    chat_id VARCHAR(50) NOT NULL, content VARCHAR(150), time_creation TIMESTAMP NOT NULL,\
                                                    time_sent TIMESTAMP NOT NULL, is_read BOOLEAN NOT NULL);";

    std::string chats = create_table + "CHATS (id UUID NOT NULL PRIMARY KEY, chat_name VARCHAR(50) NOT NULL, time_creation TIMESTAMP NOT NULL);";

    std::string users_chats_link = create_table + "USERS_CHATS_LINK (user_id VARCHAR(50), chat_id VARCHAR(50));";

    pqxx::work N(*PG_conn);
    try {
        N.exec(users);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE USERS already exists" << std::endl;
    }
    N.commit();

    pqxx::work N1(*PG_conn);
    try {
        N1.exec(messages);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE MESSAGES already exist" << std::endl;
    }
    N1.commit();

    pqxx::work N2(*PG_conn);
    try {
        N2.exec(chats);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE CHATS already exist" << std::endl;
    }
    N2.commit();

    pqxx::work N3(*PG_conn);
    try {
        N3.exec(users_chats_link);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE USERS_CHATS_LINK already exist" << std::endl;
    }
    N3.commit();

    std::cout << "All data tables have been initialized successfully" << std::endl;
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
        std::cout << "TABLE USERS does not exist" << std::endl;
    }
    N.commit();

    pqxx::work N1(*PG_conn);
    try {
        N1.exec(messages);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE MESSAGES does not exist" << std::endl;
    }
    N1.commit();

    pqxx::work N2(*PG_conn);
    try {
        N2.exec(chats);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE CHATS does not exist" << std::endl;
    }
    N2.commit();

    pqxx::work N3(*PG_conn);
    try {
 
        N3.exec(users_chats_link);
    }
    catch (const std::exception &e) {
        std::cout << "TABLE USERS_CHATS_LINK does not exist" << std::endl;
    }
    N3.commit();

    std::cout << "All data tables have been deleted successfully" << std::endl;
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

int Postgre_DB::insert(const std::string& table, std::vector<std::string> values) {
    pqxx::work N(*PG_conn);
    std::string request;
    try {
        request = "INSERT INTO " + table + " VALUES(uuid_generate_v4(), '";
        for (size_t i = 0; i < values.size(); ++i) {
            request += remove_danger_characters(values[i]) + "', '";
        } 
        request = request.substr(0, request.size() - 3);
        request += ");";
        N.exec(request);
        // pqxx::result::const_iterator c = res.begin();
        // std::cout << c[0].as<std::string>() << std::endl;
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

int Postgre_DB::update(const std::string& table, std::vector <std::string> values, std::string where = "") {
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
        request = "UPDATE " + table + " SET ";
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


int Postgre_DB::save(const std::string& table, std::vector <std::string> values, std::string where = "") {
    pqxx::result check = select(table, where);
    try {
        if ((where != "") and (check.begin() != check.end())) {
            check.clear();
            return update(table, values, where);
        }
        else {
            check.clear();
            return insert(table, values);
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

    if (user.get_id() != "") {
        data.push_back(user.get_id());
    }
    data.push_back(user.get_login());
    data.push_back(user.get_password());
    data.push_back(user.get_active_status());

    try {
        save("USERS", data);
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
        if (res.begin() != res.end()) {
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
    values.push_back(user.get_id());
    values.push_back(new_login);
    values.push_back(user.get_password());
    values.push_back(user.get_active_status());

    try {
        int update_status = update("USERS", values, where);
        if (!update_status) {
            user.set_login(new_login);
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

int Postgre_DB::change_user_password(User& user, const std::string& new_password) {
    std::string where = "password = '" + remove_danger_characters(user.get_password()) + "'";

    std::vector<std::string> values;
    values.push_back(user.get_id());
    values.push_back(user.get_login());
    values.push_back(new_password);
    values.push_back(user.get_active_status());

    try {
        int update_status = update("USERS", values, where);
        if (!update_status) {
            user.set_password(new_password);
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
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


int Postgre_DB::add_chat(const Chat& chat) {
    std::vector <std::string> data;

    if(chat.get_chat_id() != "") {
        data.push_back(chat.get_chat_id());
    }
    data.push_back(chat.get_chat_name());

    try {
        save("CHATS", data);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::add_message(const TextMessage& message) {
    std::vector <std::string> data;

    data.push_back(message.get_message_id());
    data.push_back(message.get_parent_chat_id());
    data.push_back(message.get_sender_id());
    data.push_back(message.get_address_id());
    data.push_back(message.get_message_text());
    if(message.is_read()) {
        data.push_back("TRUE");
    } else {
        data.push_back("FALSE");
    }

    try {
        save("MESSAGES", data);
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
        save("USERS_CHATS_LINK", data);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}
