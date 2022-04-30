#include "postgresql_db.h"

int main() {
    Postgre_DB db;
    // db.drop_tables();

    User user("Artem", "Totenkaf074", "active");
    // db.add_user(user);
    std::vector<std::string> participants = {user.get_login()};
    Chat chat("aipbjomq5161-1198874-asdfadf", "WithYourself", participants);
    // db.add_chat(chat);
    return 0;
}