//Copyright 2022 by Artem Ustsov

#include "postgresql_db.h"

Postgre_DB::Postgre_DB(std::string host, std::string port, std::string db_name) {
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

int Postgre_DB::max_id(const std::string & table, std::string name_id) {
    result res = select(table);
    if (res.begin() == res.end()) {
        res.clear();
        return -1;
    }
    else {
        res.clear();
        std::vector <std::string> column;
        column.push_back("MAX(" + name_id + ")");
        result res = select(table, "", column);
        result::const_iterator c = res.begin();
        int m = c[0].as<int>();
        res.clear();
        return m;
    }
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
        cout << "TABLE USERS_INFO already exists" << endl;
    }
    N.commit();
    work N1(*PG_conn);
    try {
        N1.exec(login);
    }
    catch (const std::exception &e) {
        cout << "TABLE LOGIN already exist" << endl;
    }
    N1.commit();
    work N2(*PG_conn);
    try {
        N2.exec(users_rec);
    }
    catch (const std::exception &e) {
        cout << "TABLE USERS already exist" << endl;
    }
    N2.commit();
    work N3(*PG_conn);
    try {
        N3.exec(marks);
    }
    catch (const std::exception &e) {
        cout << "TABLE MARKS already exist" << endl;
    }
    N3.commit();
    work N4(*PG_conn);
    try {
        N4.exec(images);
        N4.commit();
    }
    catch (const std::exception &e) {
        cout << "TABLE IMAGES already exist" << endl;
    }
    return 0;
}

int Postgre_DB::drop_tables() {
    std::string drop_table = "DROP TABLE ";
    std::string users_info = drop_table + "USERS_INFO;";
    std::string login = drop_table + "LOGIN;";
    std::string  users_rec = drop_table + "USERS_REC;";
    std::string marks = drop_table + "MARKS;";
    std::string images = drop_table + "IMAGES;";
    work N(*PG_conn);
    try {
        N.exec(users_info);
    }
    catch (const std::exception &e) {
        cout << "TABLE USERS_INFO does not exist" << endl;
    }
    N.commit();
    work N1(*PG_conn);
    try {
        N1.exec(login);
    }
    catch (const std::exception &e) {
        cout << "TABLE LOGIN does not exist" << endl;
    }
    N1.commit();
    work N2(*PG_conn);
    try {
        N2.exec(users_rec);
    }
    catch (const std::exception &e) {
        cout << "TABLE USERS_REC does not exist" << endl;
    }
    N2.commit();
    work N3(*PG_conn);
    try {
        N3.exec(marks);
    }
    catch (const std::exception &e) {
        cout << "TABLE MARKS does not exist" << endl;
    }
    N3.commit();
    work N4(*PG_conn);
    try {
        N4.exec(images);
        N4.commit();
    }
    catch (const std::exception &e) {
        cout << "TABLE IMAGES does not exist" << endl;
    }
    return 0;
}

int Postgre_DB::user_exist(std::string login, std::string password) {
    std::string where = "login = '" + remove_danger_characters(login) + "'";
    if (password != "") {
        where += " AND password = '" + password + "'";
    }
    try {
        result res = select("LOGIN", where);
        if (res.begin() != res.end()) {
            res.clear();
            return 1;
        }
    }
    catch (const std::exception &e) {
        return 0;
    }
    
    return 0;
}

USERS_INFO Postgre_DB::user_info(std::string login) {
    std::string where = "(SELECT user_id FROM LOGIN WHERE login = '" + login + "') = user_id"; 
    result res = select("USERS_INFO", where);
    USERS_INFO user;
    if (res.begin() != res.end()) {
        result::const_iterator c = res.begin();
        user.user_id = c[0].as<int>();
        user.age = c[1].as<int>();
        user.course_number = c[2].as<int>();
        user.num_pairs = c[3].as<int>();
        user.name = c[4].as<std::string>();
        user.surname = c[5].as<std::string>();
        user.gender = c[6].as<std::string>();
        user.faculty = c[7].as<std::string>();
        user.vk_link = c[8].as<std::string>();
        user.telegram_link = c[9].as<std::string>();
        user.description = c[10].as<std::string>();
        user.deleted = c[11].as<bool>();
    }
    res.clear();
    return user;
}

int Postgre_DB::user_register(std::string login, std::string password) {
    if (user_exist(login)) {
        return -1;
    }
    else {
        int id = max_id("LOGIN", "user_id") + 1;
        std::vector <string> user_vec(3);
        user_vec[0] = std::to_string(id);
        user_vec[1] = login;
        user_vec[2] = password;
        save("login", user_vec);
        return id;
    }
}

int Postgre_DB::save_user(USERS_INFO user_info) {
    std::vector <std::string> user;
    user.push_back(std::to_string(user_info.user_id));
    user.push_back(std::to_string(user_info.age));
    user.push_back(std::to_string(user_info.course_number));
    user.push_back(std::to_string(user_info.num_pairs));
    user.push_back(user_info.name);
    user.push_back(user_info.surname);
    user.push_back(user_info.gender);
    user.push_back(user_info.faculty);
    user.push_back(user_info.vk_link);
    user.push_back(user_info.telegram_link);
    user.push_back(user_info.description);
    user.push_back(std::to_string(user_info.deleted));
    std::string request = "user_id = " + user[0];
    try {
        std::string users_info = "USERS_INFO";
        save(users_info, user, request);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int Postgre_DB::user_id(std::string login) {
    std::string where = "login = '" + login + "'"; 
    result res = select("LOGIN", where);
    if (res.begin() == res.end()) {
        res.clear();
        return -1;
    }
    else {
         result::const_iterator c = res.begin();
         int m = c[0].as<int>();
         res.clear();
         return m;
    }
}

std::string Postgre_DB::user_login(int id) {
    std::string where = "user_id = '" + std::to_string(id) + "'"; 
    result res = select("LOGIN", where);
    if (res.begin() == res.end()) {
        res.clear();
        return "";
    }
    else {
        result::const_iterator c = res.begin();
        std::string s = c[1].as<std::string>();
        res.clear();
        return s;
    }
}

std::vector <std::vector<float>> Postgre_DB::users_params() {
    std::vector <std::vector<float>> users_params;
    std::vector <std::string> columns;
    std::vector <std::string> faculty;
    columns.push_back("age");
    columns.push_back("course_number");
    columns.push_back("num_pairs");
    columns.push_back("faculty");
    result res = select("USERS_INFO", "", columns);

    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        std::vector <float> temp;
        temp.push_back(c[0].as<float>());
        temp.push_back(c[1].as<float>());
        temp.push_back(c[2].as<float>());
        faculty.push_back(c[3].as<std::string>());
        users_params.push_back(temp);
    }
    std::vector <float> faculty_labels = Utility::LabelEncoder<float>(faculty);
    for (size_t i = 0; i < users_params.size(); ++i) {
        users_params[i].push_back(faculty_labels[i]);
    }
    res.clear();
    return users_params;
}  


int Postgre_DB::save_image(std::string path_to_file, int user_id, std::string name) {
    int image_id;
    std::string images = "IMAGES";
    std::string where = "";
    if (name != "") {
        where = "image_name = " + name;
    }
    image_id = max_id(images, "image_id") + 1;
    std::vector <std::string> values;
    values.push_back(std::to_string(image_id));
    values.push_back(std::to_string(user_id));
    values.push_back(name);
    values.push_back(path_to_file);
    try {
        save(images, values);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

std::vector <std::string> Postgre_DB::user_image(int user_id, std::string image_name) {
    std::vector <std::string> paths;
    std::string where = "user_id = " + std::to_string(user_id);
    if (image_name != "") {
        where += " AND image_name = '" + image_name + "'";
    }
    where += " ORDER BY image_id";
    result res = select("IMAGES", where);
    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        paths.push_back(c[3].as<std::string>());
    }
    res.clear();
    return paths;
}

int Postgre_DB::delete_image(int user_id, std::string image_name) {
    std::string img_req = "user_id = " + std::to_string(user_id) + " AND ";
    std::string table = "IMAGES";
    if (image_name != "") {
        img_req += "image_name = '" + image_name + "'";
    }
    else {
        img_req += "image_id = (SELECT MAX(image_id) FROM IMAGES WHERE user_id = " + std::to_string(user_id) + ")";
    }
    return delete_(table, img_req);
}

int Postgre_DB::delete_user(std::string login) {
    USERS_INFO user;
    user = user_info(login);
    if (user.user_id == -1) {
        return 1;
    }
    else {
        user.deleted = true;
        save_user(user);
        return 0;
    }
}

int Postgre_DB::user_deleted(int id) {
    USERS_INFO user;
    user = user_info(user_login(id));
    if (user.user_id == -1 or user.deleted) {
        return 1;
    }
    else {
        return 0;
    }
}
