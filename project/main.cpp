#include "database/include/postgresql_db.h"

int main() {
    Postgre_DB db;
    // db.drop_tables();


/* ADDING NW USERS TO DB */

    // User user_1("Artem", "adfadf");
    // User user_2("Maxim", "Qwerty1233");
    // User user_3("Yuri", "blablacar1230");
    // User user_4("Yaroslav", "merci");
    
    // std::cout << db.add_user(user_1) << std::endl;
    // std::cout << db.add_user(user_2) << std::endl;
    // std::cout << db.add_user(user_3) << std::endl;
    // std::cout << db.add_user(user_4) << std::endl;

/*****************************/

/*GET CURRENTS USERS FROM DB*/
    // User user_1;
    // user_1 = db.get_user_by_login("Maxim");

    // std::cout << user_1.get_active_status() << std::endl;
    // std::cout << user_1.get_id() << std::endl;
    // std::cout << user_1.get_login() << std::endl;
    // std::cout << user_1.get_password() << std::endl;

/******************/



/*CHANGE USER ACTIVE STATUS*/
    // db.change_user_status(user_1, "active");
    // std::cout << user_1.get_active_status() << std::endl;

    // db.change_user_login(user_1, "Maximus");
    // std::cout << user_1.get_login() << std::endl;

    // db.change_user_password(user_1, "qwerty");
    // std::cout << user_1.get_password() << std::endl;

    /* вернет 1, если пользователь не был добавлен*/
    // User user_3("Artem", "adfadf");
    // std::cout << db.add_user(user_3) << std::endl;
    // std::cout << user_3.get_id() << std::endl;
/********************************/


/*ADD NEW CHAT TO DB*/
    // User user_1;
    // user_1 = db.get_user_by_login("Artem");

    // User user_2;
    // user_2 = db.get_user_by_login("Maxim");

    // User user_3;
    // user_3 = db.get_user_by_login("Yuri");

    // std::vector<std::string> participants_1 = {user_1.get_login(), user_2.get_login(), user_3.get_login()};
    // Chat chat_1("Memeger", participants_1);
    // std::cout << db.add_chat(chat_1) << std::endl;
/*****************************/

/*GET CURRENT CHAT FROM DB*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // std::cout << chat_1.get_chat_id() << std::endl;
    // std::cout << chat_1.get_chat_name() << std::endl;
    // for(auto participant : chat_1.get_participants()) {
    //     std::cout << participant << std::endl;
    // }

/*GET PARTICIPANTS FROM CHAT*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // std::vector<std::string> chat_1_participants = chat_1.get_participants();
    // std::cerr << "Вернули вектор participants" << std::endl;
    // for (auto participant : chat_1_participants)
    // {
    //     std::cout << participant << std::endl;
    // }


/*ADD NEW PARTICIPANT TO THE CURRENT CHAT*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // User user_4;
    // user_4 = db.get_user_by_login("Yaroslav");
    // db.add_new_participant(user_4, chat_1);
/********************************************/


/*ADD NEW MESSAGE TO DATABASE*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // User user_1;
    // user_1 = db.get_user_by_login("Artem");
    // User user_2;
    // user_2 = db.get_user_by_login("Maxim");

    // TextMessage message(chat_1.get_chat_id(), user_1.get_id(), "Hello, Maxim");
    // std::cout << db.add_message(message) << std::endl;
/************************/


/*ADD MANE MESSAGES TO THE CURRENT CHAT*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");

    // User user_1;
    // user_1 = db.get_user_by_login("Artem");
    // User user_2;
    // user_2 = db.get_user_by_login("Maximus");

    // TextMessage message_1(chat_1.get_chat_id(), user_1.get_id(), "Hello, Maxim!");
    // std::cout << db.add_message(message_1) << std::endl;
    // TextMessage message_2(chat_1.get_chat_id(), user_2.get_id(), "Hi!");
    // std::cout << db.add_message(message_2) << std::endl;
    // TextMessage message_3(chat_1.get_chat_id(), user_1.get_id(), "I did almost all methods what we need");
    // std::cout << db.add_message(message_3) << std::endl;
    // TextMessage message_4(chat_1.get_chat_id(), user_2.get_id(), "It's good news.");
    // std::cout << db.add_message(message_4) << std::endl;
    // TextMessage message_5(chat_1.get_chat_id(), user_2.get_id(), "Let's call tonight and discuss?");
    // std::cout << db.add_message(message_5) << std::endl;

/***************************/


/*GET ALL CHATS BY PARTICIPANT LOGIN*/
    // User user_1;
    // user_1 = db.get_user_by_login("Artem");
    // std::vector<Chat> chats = db.get_all_chats_by_user_login(user_1.get_login());
    // std::cout << "Вернули вектор chats" << std::endl;
    // for (auto chat : chats)
    // {
    //     std::cout << chat.get_chat_name() << std::endl;
    // }
/**********************/


/*GET ALL MESSAGES FROM CHAT*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // std::vector<TextMessage> messages = chat_1.get_messages();
    // std::cout << "Вернули вектор messages" << std::endl;
    // for (auto message : messages)
    // {
    //     std::cout << message.get_message_text() << std::endl;
    // }
/**********************/

/*DELETE CHAT*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // std::cout << db.delete_chat(chat_1) << std::endl;
/**********************/


/*DELETE DELETED CHAT*/
    // Chat chat_1;
    // chat_1 = db.get_chat_by_chat_name("Memeger");
    // std::cout << db.delete_chat(chat_1) << std::endl;
/**********************/

/*DELETE USER*/
    // User user_1;
    // user_1 = db.get_user_by_login("Artem");
    // std::cout << db.delete_user(user_1) << std::endl;
/**********************/

/*DELETE DELETED USER*/
    // User user_1;
    // user_1 = db.get_user_by_login("Artem");
    // std::cout << db.delete_user(user_1) << std::endl;
/**********************/

    return 0;
}
