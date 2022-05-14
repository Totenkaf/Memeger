// Copyright 2022 by Artem Ustsov

#include <gtest/gtest.h>
#include "models.h"
#include "postgresql_db.h"

class DataBaseEnvironment : public ::testing::Test {
public:
    void TearDown() override {
      test_db.drop_tables();
    }
    Postgre_DB test_db = {"127.0.0.1", "5432", "test_db", "postgres", "postgres"};
};

TEST_F(DataBaseEnvironment, add_test_user) {
  User test_user("Ivan", "Qwerty1235");
  int status_add_user = test_db.add_user(test_user);
  EXPECT_EQ(status_add_user, 0);
}

TEST_F(DataBaseEnvironment, add_test_users) {
  User test_user_1("Artem", "qwwaqwaw");
  User test_user_2("Maxim", "Qwerty1235");
  User test_user_3("Yuri", "kigorg");
  User test_user_4("Yaroslav", "superduper");

  int status_add_user_1 = test_db.add_user(test_user_1);
  int status_add_user_2 = test_db.add_user(test_user_2);
  int status_add_user_3 = test_db.add_user(test_user_3);
  int status_add_user_4 = test_db.add_user(test_user_4);

  EXPECT_EQ(status_add_user_1, 0);
  EXPECT_EQ(status_add_user_2, 0);
  EXPECT_EQ(status_add_user_3, 0);
  EXPECT_EQ(status_add_user_4, 0);
}

TEST_F(DataBaseEnvironment, add_exist_user) {
  User test_user_1("Ivan", "Qwerty1235");
  test_db.add_user(test_user_1);

  User test_user_2("Ivan", "Qwerty1235");
  int status_add_exist_user = test_db.add_user(test_user_2);
  EXPECT_EQ(status_add_exist_user, 1);
}

TEST_F(DataBaseEnvironment, find_user_by_login) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  bool status_find_user = test_db.find_user_by_login("Ivan");
  EXPECT_TRUE(status_find_user);
}

TEST_F(DataBaseEnvironment, find_wrong_user_by_login) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  bool status_find_user = test_db.find_user_by_login("Artems");
  EXPECT_FALSE(status_find_user);
}

TEST_F(DataBaseEnvironment, get_user_by_login) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Ivan");

  EXPECT_TRUE(user_from_db.get_id() != std::string());
  EXPECT_EQ(user_from_db.get_login(), "Ivan");
  EXPECT_EQ(user_from_db.get_password(), "Qwerty1235");
  EXPECT_EQ(user_from_db.get_active_status(), "active");
}

TEST_F(DataBaseEnvironment, change_user_login) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Ivan");

  int status_change_user_login = test_db.change_user_login(user_from_db, "Vasiliy");
  EXPECT_EQ(status_change_user_login, 0);

  EXPECT_TRUE(user_from_db.get_id() != std::string());
  EXPECT_EQ(user_from_db.get_login(), "Vasiliy");
  EXPECT_EQ(user_from_db.get_password(), "Qwerty1235");
  EXPECT_EQ(user_from_db.get_active_status(), "active");
}

TEST_F(DataBaseEnvironment, change_wrong_user_login) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Victor");

  int status_change_user_login = test_db.change_user_login(user_from_db, "Vasiliy");
  EXPECT_EQ(status_change_user_login, 1);
}

TEST_F(DataBaseEnvironment, change_user_password) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Ivan");

  int status_change_user_login = test_db.change_user_password(user_from_db, "tiberius");
  EXPECT_EQ(status_change_user_login, 0);

  EXPECT_TRUE(user_from_db.get_id() != std::string());
  EXPECT_EQ(user_from_db.get_login(), "Ivan");
  EXPECT_EQ(user_from_db.get_password(), "tiberius");
  EXPECT_EQ(user_from_db.get_active_status(), "active");
}

TEST_F(DataBaseEnvironment, change_wrong_user_password) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Victor");

  int status_change_user_login = test_db.change_user_password(user_from_db, "Vasiliy");
  EXPECT_EQ(status_change_user_login, 1);
}

TEST_F(DataBaseEnvironment, change_user_status) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Ivan");

  int status_change_user_login = test_db.change_user_status(user_from_db, "inactive");
  EXPECT_EQ(status_change_user_login, 0);


  EXPECT_TRUE(user_from_db.get_id() != std::string());
  EXPECT_EQ(user_from_db.get_login(), "Ivan");
  EXPECT_EQ(user_from_db.get_password(), "Qwerty1235");
  EXPECT_EQ(user_from_db.get_active_status(), "inactive");
}

TEST_F(DataBaseEnvironment, change_wrong_user_status) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  User user_from_db = test_db.get_user_by_login("Victor");

  int status_change_user_login = test_db.change_user_status(user_from_db, "inactive");
  EXPECT_EQ(status_change_user_login, 1);
}

TEST_F(DataBaseEnvironment, get_user_id) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);
  std::string user_id = test_db.get_user_id("Ivan");
  EXPECT_TRUE(!user_id.empty());
}

TEST_F(DataBaseEnvironment, delete_user) {
  User test_user("Ivan", "Qwerty1235");
  test_db.add_user(test_user);

  User user_from_db = test_db.get_user_by_login("Ivan");
  int status_delete_user = test_db.delete_user(user_from_db);
  EXPECT_EQ(status_delete_user, 0);

  EXPECT_EQ(user_from_db.get_id(), std::string());
  EXPECT_EQ(user_from_db.get_login(), std::string());
  EXPECT_EQ(user_from_db.get_password(), std::string());
  EXPECT_EQ(user_from_db.get_active_status(), std::string());
}

TEST_F(DataBaseEnvironment, add_chat) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);

  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login()};
  Chat chat_1("Memeger", participants_1);

  EXPECT_EQ(test_db.add_chat(chat_1), 0);
}

TEST_F(DataBaseEnvironment, get_chat) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);

  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login()};

  Chat chat_1("Memeger", participants_1);
  test_db.add_chat(chat_1);

  Chat chat_from_db = test_db.get_chat_by_chat_name("Memeger");
  EXPECT_EQ(chat_from_db.get_chat_name(), "Memeger");
  EXPECT_TRUE(chat_from_db.get_chat_id() != std::string());
  EXPECT_TRUE(chat_from_db.is_dialogue());
}

TEST_F(DataBaseEnvironment, add_new_chat_participant) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);
  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login()};
  Chat chat_1("Memeger", participants_1);
  test_db.add_chat(chat_1);

  Chat chat_from_db = test_db.get_chat_by_chat_name("Memeger");
  User test_user_3("Yuir", "adsfads");
  test_db.add_user(test_user_3);

  int status_add_new_participant = test_db.add_new_participant(test_user_3, chat_from_db);
  EXPECT_EQ(status_add_new_participant, 0);

  Chat chat_from_db_new = test_db.get_chat_by_chat_name("Memeger");
  EXPECT_TRUE(chat_from_db_new.is_polilogue());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
