//Copyright 2022 by Artem Ustsov

#include "postgresql_db.h"
#include "models.h"

Postgre_DB::Postgre_DB(std::string host, std::string port) {
    init_tables();
}

Postgre_DB::~Postgre_DB() {
    if (PG_conn->is_open()) {
        PG_conn->close();
    }
}

std::string Postgre_DB::remove_danger_characters(const std::string & row_column) {
    std::string prep_column = "";
    for (auto i : row_column) {
        if (i != '\'' and i != '\"') prep_column += i;
    }
    return prep_column;
}

result Postgre_DB::select(const std::string table, std::string where, std::vector <std::string> what) {
    std::string request = "SELECT ";
    if (what.size() > 0) {
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
    nontransaction N(*PG_conn);
    result res = N.exec(request);
    N.commit();
    return res;
}

int Postgre_DB::insert(const std::string & table, std::vector <std::string> values) {
    work N(*PG_conn);
    std::string request;
    try {
        request = "INSERT INTO " + table + " VALUES('";
        for (size_t i = 0; i < values.size(); ++i) {
            request += remove_danger_characters(values[i]) + "', '";
        } 
        request = request.substr(0, request.size() - 3);
        request += ");";
        N.exec(request);
        N.commit();
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

int Postgre_DB::update(const std::string & table, std::vector <std::string> values, std::string where) {
    std::string request;
    std::string lower_table = "";
    for (auto sym : table) {
        lower_table += tolower(sym);
    }
    std::vector <std::string> column;
    column.push_back("column_name");
    std::string wh = "TABLE_NAME = '" + lower_table + "'";
    result columns = select("INFORMATION_SCHEMA.COLUMNS", wh, column);
    work N(*PG_conn);
    try {
        request = "UPDATE " + table + " SET ";
        std::size_t i = 0;
        for (result::const_iterator c = columns.begin(); c != columns.end(); ++c) {
            request += c[0].as<std::string>() + " = '" + remove_danger_characters(values[i++]) + "', ";
        }
        if (i < values.size()) return 1;

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

int Postgre_DB::save(const std::string & table, std::vector <std::string> values, std::string where) {
    result check = select(table, where);
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

int Postgre_DB::delete_(const std::string & table, std::string where) {
    work N(*PG_conn);
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

int Postgre_DB::init_tables() {
    std::string create_table = "CREATE TABLE ";
    std::string users = create_table + "USERS ();";
    std::string messages = create_table + "MESSAGES ();";
    std::string chats = create_table + "CHATS ();";
    work N(*PG_conn);
    try {
        N.exec(users_info);
    }
    catch (const std::exception &e) {
        cout << "TABLE USERS already exists" << endl;
    }
    N.commit();
    work N1(*PG_conn);
    try {
        N1.exec(login);
    }
    catch (const std::exception &e) {
        cout << "TABLE MESSAGES already exist" << endl;
    }
    N1.commit();
    work N2(*PG_conn);
    try {
        N2.exec(users_rec);
    }
    catch (const std::exception &e) {
        cout << "TABLE CHATS already exist" << endl;
    }
    N2.commit();
    return 0;
}

int Postgre_DB::drop_tables() {
    std::string drop_table = "DROP TABLE ";
    std::string users_info = drop_table + "USERS;";
    std::string login = drop_table + "CHATS;";
    std::string  users_rec = drop_table + "MESSAGES;";
    work N(*PG_conn);
    try {
        N.exec(users_info);
    }
    catch (const std::exception &e) {
        cout << "TABLE USERS does not exist" << endl;
    }
    N.commit();
    work N1(*PG_conn);
    try {
        N1.exec(login);
    }
    catch (const std::exception &e) {
        cout << "TABLE CHATS does not exist" << endl;
    }
    N1.commit();
    work N2(*PG_conn);
    try {
        N2.exec(users_rec);
    }
    catch (const std::exception &e) {
        cout << "TABLE MESSAGES does not exist" << endl;
    }
    N2.commit();
    return 0;
}




int Postgre_DB::add_user(const User& user);
User Postgre_DB::get_user_by_login(std::string login) const;
bool Postgre_DB::find_user_by_login(std::string login) const;
int Postgre_DB::delete_user(User& user);
int Postgre_DB::save_user(USERS_INFO user_info);

int Postgre_DB::change_user_login(User& user, string new_login);
int Postgre_DB::change_user_password(User& user, string new_password);


int Postgre_DB::add_message(TextMessage& message);
int Postgre_DB::save_message(TextMessage& message);
std::vector<std::string> Postgre_DB::get_last_N_messages(TextMessage& message) const;
int Postgre_DB::delete_message(TextMessage& message);


int Postgre_DB::add_chat(Chat& chat);
int Postgre_DB::save_chat(Chat& chat);
int Postgre_DB::delete_chat(Chat& chat);

std::string Postgre_DB::find_chat_by_participants(Chat& chat);
std::vector<std::string> Postgre_DB::get_participants_from_chat(const Chat& chat) const;
std::vector<Chat> Postgre_DB::get_all_chats_by_login(std::string login) const;
