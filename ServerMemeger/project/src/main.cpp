#include "postgresql_db.h"


int main() {
    Postgre_DB db("127.0.0.1", "5432", "postgres", "postgres", "postgres");
    // db.init_tables();

    User user("lsjfih-656sdfg-s65fdg", "Artem", "Totenkaf074", "active");
    // db.add_user(user);
    std::vector<std::string> participants = {user.get_login()};
    Chat chat("aipbjomq5161-1198874-asdfadf", "WithYourself", participants);
    // db.add_chat(chat);
    return 0;
}