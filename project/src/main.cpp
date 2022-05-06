#include "postgresql_db.h"

int main() {
    Postgre_DB db;
    // db.drop_tables();

    // User user_1("Artem", "adfadf");
    // User user_2("Maxim", "Qwerty1233");
    // User user_3("Yuri", "blablacar1230");
    // User user_4("Yaroslav", "merci");
    
    // std::cout << db.add_user(user_1) << std::endl;
    // std::cout << db.add_user(user_2) << std::endl;
    // std::cout << db.add_user(user_3) << std::endl;
    // std::cout << db.add_user(user_4) << std::endl;

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
    // user_3 = db.get_user_by_login("Yu
    // User user_1;
    // user_1 = db.get_user_by_login("Artem");

    // User user_2;
    // user_2 = db.get_user_by_login("Maxim");

    // std::vector<std::string> participants_1 = {user_1.get_login(), user_2.get_login(), user_3.get_login()};
    // Chat chat_1("Memeger", participants_1);
    // std::cout << db.add_chat(chat_1) << std::endl;


    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");


    // TextMessage message(chat.get_chat_id(), user_1.get_id(), user_2.get_id(), "Hello, Maxim");
    // std::cout << db.add_message(message) << std::endl;



    // std::cout << chat_1.get_chat_id() << std::endl;
    // std::cout << chat_1.get_chat_name() << std::endl;

    // Chat chat_1;

    // std::vector<std::string> participants_1 = {user_1.get_login(), user_2.get_login()};
    // Chat chat_1("Artem/Maxim", participants_1);
    // std::cout << db.add_chat(chat_1) << std::endl;

    // chat_1 = db.get_chat_by_chat_name("Artem/Maxim");

    // TextMessage message_1(chat_1.get_chat_id(), user_1.get_id(), user_2.get_id(), "Hello, Maxim!");
    // std::cout << db.add_message(message_1) << std::endl;
    // TextMessage message_2(chat_1.get_chat_id(), user_2.get_id(), user_1.get_id(), "Hi!");
    // std::cout << db.add_message(message_2) << std::endl;
    // TextMessage message_3(chat_1.get_chat_id(), user_1.get_id(), user_2.get_id(), "I did almost all methods what we need");
    // std::cout << db.add_message(message_3) << std::endl;
    // TextMessage message_4(chat_1.get_chat_id(), user_2.get_id(), user_1.get_id(), "It's good news.");
    // std::cout << db.add_message(message_4) << std::endl;
    // TextMessage message_5(chat_1.get_chat_id(), user_2.get_id(), user_1.get_id(), "Let's call tonight and discuss?");
    // std::cout << db.add_message(message_5) << std::endl;


    // std::vector<std::string> chat_1_participants = db.get_participants_from_chat(chat_1);
    // std::cerr << "Вернули вектор participants" << std::endl;
    // for (auto participant : chat_1_participants)
    // {
    //     std::cout << participant << std::endl;
    // }



    // std::vector<Chat> chats = db.get_all_chats_by_user_login(user_3.get_login());
    // std::cout << "Вернули вектор chats" << std::endl;
    // for (auto chat : chats)
    // {
    //     std::cout << chat.get_chat_name() << std::endl;
    // }

    // size_t num_of_message = 1;
    // std::vector<std::string> messages = db.get_last_N_messages_from_chat(chat_1.get_chat_id(), num_of_message);
    // std::cout << "Вернули вектор messages" << std::endl;
    // for (auto message : messages)
    // {
    //     std::cout << message << std::endl;
    // }

    // std::cout << db.change_chat_name(chat_1, "Maxim/Artem") << std::endl;

    // Chat chat_1 = db.get_chat_by_chat_name("Memeger");
    // db.change_chat_name(chat_1, "BigBrainTeam");
    // db.add_new_participant(user_4, chat_1);

    return 0;
}
