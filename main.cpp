#include "postgresql_db.h"

auto main(int argc, char* argv[]) -> int {
    if(argc < 2) {
        std::cerr << "EMPTY COMMAND" << std::endl;
        return _EXIT_FAULT;
    }

    std::string request = argv[1];

    if(request == "init_only") {
        Postgre_DB db("127.0.0.1", "5432", "postgres", "postgres", "postgres");

        User user_1("Artem", "adfadf");
        User user_2("Maxim", "Qwerty1233");
        User user_3("Yuri", "blablacar1230");
        User user_4("Yaroslav", "merci");

        db.add_user(user_1);
        db.add_user(user_2);
        db.add_user(user_3);
        db.add_user(user_4);

        std::vector<std::string> participants_1 = {user_1.get_login(), user_2.get_login(), user_3.get_login()};
        Chat chat_1("Memeger", participants_1);
        db.add_chat(chat_1);

        std::cout << "DB TABLES HAVE BEEN INITIALIZED" << std::endl;
    } 
    else if(request == "init_drop") {
        Postgre_DB db("127.0.0.1", "5432", "postgres", "postgres", "postgres");
        std::cout << "DB TABLES HAVE BEEN INITIALIZED" << std::endl;
        db.drop_tables();
        std::cout << "DB TABLES HAVE BEEN DELETED" << std::endl;
    } 
    return _EXIT_SUCCESS;
}
