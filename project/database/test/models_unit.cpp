// Copyright 2022 by Artem Ustsov

#include <gtest/gtest.h>
#include "models.h"

// добавить фикстуру

TEST(MODELS, add_test_user) {
  Postgre_DB test_db("127.0.0.1", "5432", "test_db", "postgres", "postgres");

  User test_user("Ivan", "Qwerty1235", "active");
  test_db.add_user(test_user);

  User user_from_db = test_db.get_user_by_login("Ivan");

  EXPECT_TRUE(user_from_db.get_id() != std::string());
  EXPECT_EQ(user_from_db.get_login(), "Ivan");
  EXPECT_EQ(user_from_db.get_password(), "Qwerty1235");
  EXPECT_EQ(user_from_db.get_active_status(), "active");

  test_db.drop_tables();
}

TEST(MODELS, change_test_user_login) {
  Postgre_DB test_db("127.0.0.1", "5432", "test_db", "postgres", "postgres");
  User test_user("Ivan", "Qwerty1235", "active");

  // CODE HERE

  test_db.drop_tables();
}

TEST(MODELS, change_test_user_password) {
  Postgre_DB test_db("127.0.0.1", "5432", "test_db", "postgres", "postgres");
  User test_user("Ivan", "Qwerty1235", "active");

  // CODE HERE

  test_db.drop_tables();
}

TEST(MODELS, find_test_user_by_login) {
  Postgre_DB test_db("127.0.0.1", "5432", "test_db", "postgres", "postgres");
  User test_user("Ivan", "Qwerty1235", "active");

  // CODE HERE

  test_db.drop_tables();
}

TEST(MODELS, delete_user) {
  Postgre_DB test_db("127.0.0.1", "5432", "test_db", "postgres", "postgres");
  User test_user("Ivan", "Qwerty1235", "active");

  // CODE HERE

  test_db.drop_tables();
}

TEST(MODELS, add_chat) {
  Postgre_DB test_db("127.0.0.1", "5432", "test_db", "postgres", "postgres");
  User test_user("Ivan", "Qwerty1235", "active");
  std::vector<std::string> participants = {test_user.get_login()};
  Chat test_chat("TestChat", participants);

  test_db.add_chat(test_chat);

  // User user_from_db = test_db.get_user_by_login("Ivan"); /* добавить методы
  // для получания чата из базы данных*/

  // EXPECT_TRUE(user_from_db.get_id() != std::string());
  // EXPECT_EQ(user_from_db.get_login(), "Ivan");
  // EXPECT_EQ(user_from_db.get_password(), "Qwerty1235");
  // EXPECT_EQ(user_from_db.get_active_status(), "active");

  test_db.drop_tables();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
