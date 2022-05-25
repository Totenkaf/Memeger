#include "postgresql_db.h"

int main() {
    Postgre_DB db("127.0.0.1", "5432", "postgres", "postgres", "postgres");
    return 0;
}
