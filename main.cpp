#include "postgresql_db.h"

int main() {
    Postgre_DB db("127.0.0.1", "5432", "postgres", "postgres", "postgres");
    db.drop_tables();

    // User user_1("Artem", "adfadf");
    // User user_2("Maxim", "Qwerty1233");
    // User user_3("Yuri", "blablacar1230");
    // User user_4("Yaroslav", "merci");
    
    // std::cout << db.add_user(user_1) << std::endl;
    // std::cout << db.add_user(user_2) << std::endl;
    // std::cout << db.add_user(user_3) << std::endl;
    // std::cout << db.add_user(user_4) << std::endl;

    return 0;
}