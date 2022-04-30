#include <iostream>
#include <pqxx/pqxx>


int init_tables(std::shared_ptr <pqxx::connection> PG_conn) {

    std::string create_table = "CREATE TABLE ";
    /*время создания и обновления через триггеры*/
    std::string users = create_table + "USERS (id VARCHAR(50) NOT NULL PRIMARY KEY, login VARCHAR(50) NOT NULL, password VARCHAR(50) NOT NULL,\
                                               active_status VARCHAR(8) NOT NULL);";

    std::string messages = create_table + "MESSAGES (id VARCHAR(50) NOT NULL PRIMARY KEY, user_from VARCHAR(50) NOT NULL, user_to VARCHAR(50) NOT NULL,\
                                                    chat_id VARCHAR(50) NOT NULL, content VARCHAR(150), is_read BOOLEAN NOT NULL);";

    std::string chats = create_table + "CHATS (id VARCHAR(50) NOT NULL PRIMARY KEY, chat_name VARCHAR(50) NOT NULL);";

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

int drop_tables(std::shared_ptr <pqxx::connection> PG_conn) {
    std::string drop_table = "DROP TABLE ";

    std::string users = drop_table + "USERS;";
    std::string messages = drop_table + "MESSAGES;";
    std::string chats = drop_table + "CHATS ;";
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


std::string remove_danger_characters(const std::string& row_column) {
    std::string prep_column = "";
    for (auto i : row_column) {
        if (i != '\'' and i != '\"') {
            prep_column += i;
        }
    }
    return prep_column;
}


int insert(const std::string& table, std::vector<std::string> values, std::shared_ptr<pqxx::connection> PG_conn) {
    pqxx::work N(*PG_conn);
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


pqxx::result select(const std::string& table, std::shared_ptr<pqxx::connection> PG_conn, std::string where = "", std::vector <std::string> what = std::vector <std::string>()) {
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


int update(const std::string& table, std::vector <std::string> values, std::shared_ptr<pqxx::connection> PG_conn, std::string where = "") {
    std::string request;
    std::string lower_table = "";

    for (auto sym : table) {
        lower_table += tolower(sym);
    }

    std::vector <std::string> column;
    column.push_back("column_name");

    std::string wh = "TABLE_NAME = '" + lower_table + "'";

    pqxx::result columns = select("INFORMATION_SCHEMA.COLUMNS", PG_conn, wh, column);
    
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



int save(const std::string& table, std::vector <std::string> values, std::shared_ptr<pqxx::connection> PG_conn, std::string where = "") {
    pqxx::result check = select(table, PG_conn, where);
    try {
        if ((where != "") and (check.begin() != check.end())) {
            check.clear();
            return update(table, values, PG_conn, where);
        }
        else {
            check.clear();
            return insert(table, values, PG_conn);
        }
    }
    catch (const std::exception &e) {
        check.clear();
        return 1;
    }
}


class User {
private:
    std::string user_login_;
    std::string user_password_;
    std::string user_id_;
    std::string active_status_;

public:
    User()
    : user_login_(""), user_password_(""), user_id_(""), active_status_("") {}
    ~User() = default;

    User(std::string user_id, std::string user_login, 
        std::string user_password, std::string active_status) :
        user_login_(std::move(user_login)), user_password_(std::move(user_password)),
        user_id_(std::move(user_id)), active_status_(active_status) {
    }

    std::string get_id() const;
    std::string get_login() const;
    std::string get_password() const;
    std::string get_active_status() const;

    void set_id(const std::string &user_id);
    void set_login(const std::string &user_login);
    void set_password(const std::string &user_password);
    void set_active_status(const std::string &active_status);

    bool operator==(const User &user) const;
    bool operator!=(const User &user) const;
};

std::string User::get_login() const { return user_login_; }
std::string User::get_password() const { return user_password_; }
std::string User::get_id() const { return user_id_; }
std::string User::get_active_status() const { return active_status_; }

void User::set_login(const std::string &user_login) { user_login_ = user_login; }
void User::set_password(const std::string &user_password) { user_password_ = user_password; }
void User::set_id(const std::string &user_id) { user_id_ = user_id; }
void User::set_active_status(const std::string& active_status) { active_status_ = active_status; }

bool User::operator==(const User &user) const { return user_login_ == user.get_login(); }
bool User::operator!=(const User &user) const { return user_login_ != user.get_login(); }



int add_user(const User& user, std::shared_ptr<pqxx::connection> PG_conn) {
    std::vector <std::string> data;

    data.push_back(user.get_id());
    data.push_back(user.get_login());
    data.push_back(user.get_password());
    data.push_back(user.get_active_status());

    try {
        save("USERS", data, PG_conn);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


User get_user_by_login(const std::string& login, std::shared_ptr<pqxx::connection> PG_conn) {
    std::string where = "login = '" + remove_danger_characters(login) + "'"; 
    pqxx::result res = select("USERS", PG_conn, where);
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

bool find_user_by_login(const std::string& login, std::shared_ptr<pqxx::connection> PG_conn) {
    std::string where = "login = '" + remove_danger_characters(login) + "'";
    try {
        pqxx::result res = select("USERS", PG_conn, where);
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

int change_user_login(User& user, const std::string& new_login, std::shared_ptr<pqxx::connection> PG_conn) {
    std::string where = "login = '" + remove_danger_characters(user.get_login()) + "'";

    std::vector<std::string> values;
    values.push_back(user.get_id());
    values.push_back(new_login);
    values.push_back(user.get_password());
    values.push_back(user.get_active_status());

    try {
        int update_status = update("USERS", values, PG_conn, where);
        if (!update_status) {
            user.set_login(new_login);
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}

int change_user_password(User& user, const std::string& new_password, std::shared_ptr<pqxx::connection> PG_conn) {
    std::string where = "password = '" + remove_danger_characters(user.get_password()) + "'";

    std::vector<std::string> values;
    values.push_back(user.get_id());
    values.push_back(user.get_login());
    values.push_back(new_password);
    values.push_back(user.get_active_status());

    try {
        int update_status = update("USERS", values, PG_conn, where);
        if (!update_status) {
            user.set_password(new_password);
        }
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int delete_(const std::string& table, std::string& where, std::shared_ptr<pqxx::connection> PG_conn) {
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


int delete_user(User& user, std::shared_ptr<pqxx::connection> PG_conn) {
    if (user.get_id() == "") {
        return 1;
    }
    else {  
        std::string where = "id = '" + remove_danger_characters(user.get_id()) + "'";
        return delete_("USERS", where, PG_conn);
    }
}

class TextMessage {
public:
    TextMessage()
    : message_id_(std::string()), parent_chat_id_(std::string()),
    sender_id_(std::string()), address_id_(std::string()),
    text_message_(std::string()), is_read_(false) {}

    TextMessage(std::string message_id, std::string parent_chat_id, 
                std::string sender_id, std::string address_id,
                std::string text_message, bool is_read) :
                message_id_(message_id), parent_chat_id_(parent_chat_id), 
                sender_id_(sender_id), address_id_(address_id),
                text_message_(std::move(text_message)), 
                is_read_(is_read) 
    {}

    ~TextMessage() = default;

    std::string get_message_id() const;
    int set_message_id(const std::string& message_id);
    
    std::string get_parent_chat_id() const;
    int set_parent_chat_id(const std::string& parent_chat_id);

    std::string get_sender_id() const;
    int set_sender_id(const std::string& sender_id);

    std::string get_address_id() const;
    int set_address_id(const std::string& address_id);

    // time_t get_time_sent() const;
    // int set_time_sent(time_t time_sent);
    bool is_read() const;

    std::string get_message_text() const;

private:
    std::string message_id_;
    std::string parent_chat_id_;
    std::string sender_id_;
    std::string address_id_; /* пока непонятно, нужно ли это держать */
    std::string text_message_;
    // time_t time_sent_;
    bool is_read_;
};


std::string TextMessage::get_message_id() const { 
    return message_id_; 
}

int TextMessage::set_message_id(const std::string& message_id) { 
    message_id_ = message_id; 
    return 0;
}

std::string TextMessage::get_parent_chat_id() const { 
    return parent_chat_id_; 
}

int TextMessage::set_parent_chat_id(const std::string& parent_chat_id) {
    parent_chat_id_ = parent_chat_id;
    return 0;
}

std::string TextMessage::get_sender_id() const { 
    return sender_id_; 
}

std::string TextMessage::get_address_id() const { 
    return address_id_; 
}

std::string TextMessage::get_message_text() const { 
    return text_message_; 
}

// time_t TextMessage::get_time_sent() const { 
//     return time_sent_; 
// }

bool TextMessage::is_read() const {
    return is_read_; 
}



class Chat {
public:
    Chat() = default;

    Chat(std::string chat_id, std::string chat_name,
            std::vector<std::string>& participants,
            std::vector<TextMessage>& messages)
            :
            chat_id_(chat_id),
            chat_name_(chat_name),
            participants_(participants),
            messages_(messages)
    {}

    Chat(std::string chat_id, std::string chat_name,
        std::vector<std::string>& participants)
        :
        chat_id_(chat_id),
        chat_name_(chat_name),
        participants_(participants),
        messages_(std::vector<TextMessage>())
    {}

    ~Chat() = default;

    TextMessage get_last_message() const;

    std::string get_chat_name() const;
    int set_chat_name(const std::string& chat_name);

    std::string get_chat_id() const;
    int set_chat_id(std::string chat_id);

    time_t get_chat_time_creation() const;
    int set_chat_time_creation(const time_t& date_creation);
    
    std::vector<std::string> get_participants() const;
    std::vector<TextMessage> get_messages() const;


    int push_new_message(TextMessage new_message);
    int add_new_participant(std::string& new_participant_id);

    bool is_empty() const;
    bool is_dialogue() const;
    bool is_monologue() const;
    bool is_polilogue() const;

    bool operator!=(const Chat &chat) const;
    bool operator==(const Chat &chat) const;

private:
    std::string chat_id_;
    std::string chat_name_;
    std::vector<std::string> participants_;
    std::vector<TextMessage> messages_;
    // time_t date_creation_;
};


std::vector<TextMessage> Chat::get_messages() const {
    return messages_;
}

TextMessage Chat::get_last_message() const {
    return messages_.back();
}

std::vector<std::string> Chat::get_participants() const {
    return participants_;
}

std::string Chat::get_chat_id() const {
    return chat_id_;
}

int Chat::set_chat_id(std::string chat_id) {
    chat_id_ = chat_id;
    return 0;
}

std::string Chat::get_chat_name() const {
    return chat_name_;
}

int Chat::set_chat_name(const std::string& chat_name) {
    chat_name_ = chat_name;
    return 0;
}

// time_t Chat::get_chat_time_creation() const {
//     return date_creation_;
// }

// int Chat::set_chat_time_creation(const time_t& date_creation) {
//     date_creation_ = date_creation;
//     return 0;
// }

int Chat::push_new_message(TextMessage new_message) {
    messages_.push_back(new_message);
    return 0;
}

int Chat::add_new_participant(std::string& new_participant_id) {
    participants_.push_back(new_participant_id);
    return 0;
}


bool Chat::operator==(const Chat &chat) const {
    return chat_id_ == chat.get_chat_id();
}

bool Chat::operator!=(const Chat &chat) const {
    return chat_id_ != chat.get_chat_id();
}


bool Chat::is_empty() const {
    return participants_.size() == 0;
}

bool Chat::is_monologue() const {
    return participants_.size() == 1;
}

bool Chat::is_dialogue() const {
    return participants_.size() == 2;
}

bool Chat::is_polilogue() const {
    return participants_.size() > 2;
}


int add_chat(const Chat& chat, std::shared_ptr<pqxx::connection> PG_conn) {
    std::vector <std::string> data;

    data.push_back(chat.get_chat_id());
    data.push_back(chat.get_chat_name());

    try {
        save("CHATS", data, PG_conn);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int add_message(const TextMessage& message, std::shared_ptr<pqxx::connection> PG_conn) {
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
        save("MESSAGES", data, PG_conn);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int add_user_chat_link(const User& user, const Chat& chat, std::shared_ptr<pqxx::connection> PG_conn) {
    std::vector <std::string> data;

    data.push_back(user.get_id());
    data.push_back(chat.get_chat_id());

    try {
        save("USERS_CHATS_LINK", data, PG_conn);
    }
    catch (const std::exception &e) {
        return 1;
    }
    return 0;
}


int main(int argc, char* argv[]) {
    std::string db_name = "postgres";
    std::string db_user = "postgres";
    std::string db_password = "postgres";
    std::string db_hostaddr = "127.0.0.1";
    std::string db_port = "5432";
    
    
    std::string request = "dbname = " + db_name + " user = " + db_user + " password = " + db_password + " hostaddr = " + db_hostaddr + " port = " + db_port;


    std::shared_ptr <pqxx::connection> PG_conn;
    PG_conn = std::make_shared <pqxx::connection>(request);
    std::cout << "Database has been successfully connected" << std::endl;

   try {
        if (!PG_conn->is_open()) {
            std::cerr << "Can't open database" << std::endl;
            
        } else {
            std::cout << "Database has been successfully opened" << std::endl;
        }
   }
    catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      return 1;
   }

//    init_tables(PG_conn);

//    User user;
//    user.set_login("Timur");
//    user.set_password("12345678");
//    user.set_active_status("active");
//    user.set_id("abcd91e9-9060-408a-97ee-b587fc5efff"); //генерить на стороне сервера BOOST

//    add_user(user, PG_conn);

//    User user_1;
//    user_1.set_login("Artem");
//    user_1.set_password("0000");
//    user_1.set_active_status("active");
//    user_1.set_id("7feb24af-fc38-44de-bc38-04defc3804de"); //генерить на стороне сервера BOOST

//    add_user(user_1, PG_conn);


//    User user_2;
//    user_2 = get_user_by_login("Artem", PG_conn);
//    std::cout << "UUID пользователя: " << user_2.get_id() << std::endl;
//    std::cout << "Логин пользователя: " << user_2.get_login() << std::endl;
//    std::cout << "Пароль пользователя: " << user_2.get_password() << std::endl;
//    std::cout << "Статус пользователя: " << user_2.get_active_status() << std::endl;

//    std::string user_name_to_find = "Maxim";
//    bool answer = find_user_by_login(user_name_to_find, PG_conn);   std::vector<TextMessage> messages = {text_message_1};
//    answer = find_user_by_login(user_name_to_find, PG_conn);
//    std::cout << "Пользователь с именем " << user_name_to_find << (answer == true ? " cуществует в БД" : " не существует в БД") << std::endl;

//    std::vector<std::string> values_1 = {"Artem", "1234578", "active"};
//    std::vector<std::string> values_2 = {"Maxim", "0000", "active"};
//    insert("USERS", values_1, PG_conn);
//    insert("USERS", values_2, PG_conn);


//    std::vector<std::string> values_3 = {"Artem/Maxim", "NOW()"};
//    insert("CHATS", values_3, PG_conn);

//    std::vector<std::string> values_4 = {"Artem/Maxim", "NOW()"};
//    insert("MESSAGES", values_4, PG_conn);


    // change_user_login(user_3, "Maximus", PG_conn);
    // std::cout << "Пользователь с именем " << user_3.get_login() << std::endl;

    // std::cout << "Пользователь с именем " << user_3.get_login() << " Имееет пароль " << user_3.get_password() << std::endl;
    // change_user_password(user_3, "0000000", PG_conn);
    // std::cout << "Пользователь с именем " << user_3.get_login() << " Имееет нвоый пароль " << user_3.get_password() << std::endl;


    // if(!delete_user(user_3, PG_conn)) {
    //     std::cout << "User has been deleted successfully" << std::endl;
    // } else {
    //     std::cout << "User hasn't been deleted" << std::endl;
    // }

   User user_1("7feb24af-fc38-44de-bc38-04defc3804de", "Artem", "Totenkaf07$", "active");
   add_user(user_1, PG_conn);
   User user_2("9859asd-dsfsd6549-99asd161-sad", "Maxim", "123456", "active");
   add_user(user_2, PG_conn);


   std::vector<std::string> participants = {user_1.get_login(), user_2.get_login()};

   Chat chat("sfsd6549-99-fc38-5622300", "Artem/Maxim", participants);
   add_chat(chat, PG_conn);

   TextMessage text_message_1("9859asd-dsfsd6549-99asd161-sad", chat.get_chat_id(), user_1.get_id(), user_2.get_id(), "Hello, Maxim", false);
   add_message(text_message_1, PG_conn);

   add_user_chat_link(user_1, chat, PG_conn);
   add_user_chat_link(user_2, chat, PG_conn);
   

   User user_3("0000-0000-0000", "Timur", "00000", "inactive");
   add_user(user_3, PG_conn);
   std::vector<std::string> participants_1 = {user_1.get_login(), user_2.get_login(), user_3.get_login()};
   Chat chat_1("1111-2222-3333", "Memeger", participants_1);
   add_chat(chat_1, PG_conn);

   TextMessage text_message_2("998794d-ddafsd9f8161-zusfjf", chat_1.get_chat_id(), user_1.get_id(), chat_1.get_chat_id(), "Hi, guys!", false);
   add_message(text_message_2, PG_conn);
   add_user_chat_link(user_1, chat_1, PG_conn);
   add_user_chat_link(user_2, chat_1, PG_conn);
   add_user_chat_link(user_3, chat_1, PG_conn);


//    drop_tables(PG_conn);
   PG_conn->disconnect();
   std::cout << "Database has been successfully closed" << std::endl;
   std::cout << "Database has been successfully disconnected" << std::endl;

   return 0;
}
