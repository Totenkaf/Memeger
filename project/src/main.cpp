#include "postgresql_db.h"

int main() {
    Postgre_DB db;
    db.drop_tables();

    // User user_1("Artem", "adfadf");
    // User user_2("Maxim", "Qwerty1233");
    // User user_3("Yuri", "blablacar1230");
    
    // std::cout << db.add_user(user_1) << std::endl;
    // std::cout << db.add_user(user_2) << std::endl;
    // std::cout << db.add_user(user_3) << std::endl;

    // std::cout << user_1.get_id() << std::endl;
    // std::cout << user_1.get_login() << std::endl;
    // std::cout << user_1.get_password() << std::endl;
    // std::cout << user_1.get_active_status() << std::endl;



    // User user_1;
    // user_1 = db.get_user_by_login("Maxim");
    // std::cout << user_1.get_active_status() << std::endl;

    // std::cout << user_1.get_id() << std::endl;
    // std::cout << user_1.get_login() << std::endl;
    // std::cout << user_1.get_password() << std::endl;

    // db.change_user_status(user_1, "inactive");
    // std::cout << user_1.get_active_status() << std::endl;

    /* вернет 1, если пользователь не был добавлен*/
    // User user_3("Artem", "adfadf", "active");
    // db.add_user(user_3);
    // std::cout << user_3.get_id() << std::endl;


    // User user_1;
    // user_1 = db.get_user_by_login("Artem");

    // User user_2;
    // user_2 = db.get_user_by_login("Maxim");

    // User user_3;
    // user_3 = db.get_user_by_login("Yuri");

    // std::vector<std::string> participants_1 = {user_1.get_login(), user_2.get_login()};
    // Chat chat_1("Artem/Maxim", participants_1);
    // std::cout << db.add_chat(chat_1) << std::endl;

    // std::vector<std::string> participants_2 = {user_1.get_login(), user_2.get_login(), user_3.get_login()};
    // Chat chat_2("Memeger", participants_2);
    // std::cout << db.add_chat(chat_2) << std::endl;


    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");


    // std::cout << db.add_user_chat_link(user_1, chat_1) << std::endl;
    // std::cout << db.add_user_chat_link(user_2, chat_1) << std::endl;



    // TextMessage message(chat.get_chat_id(), user_1.get_id(), user_2.get_id(), "Hello, Maxim");
    // std::cout << db.add_message(message) << std::endl;



    // std::cout << chat_1.get_chat_id() << std::endl;
    // std::cout << chat_1.get_chat_name() << std::endl;

    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");

    // std::cout << db.add_user_chat_link(user_1, chat_1) << std::endl;
    // std::cout << db.add_user_chat_link(user_2, chat_1) << std::endl;
    // std::cout << db.add_user_chat_link(user_3, chat_1) << std::endl;

    // std::vector<std::string> chat_1_participants = db.get_participants_from_chat(chat_1);
    // for (auto participant : chat_1_participants)
    // {
    //     std::cout << participant << std::endl;
    // }
    return 0;
}
