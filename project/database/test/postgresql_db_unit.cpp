// Copyright 2022 by Artem Ustsov

#include <gtest/gtest.h>
#include "models.h"
#include "postgresql_db.h"

// добавить в фикстуру базовых пользователей, чаты и сообщения
// почему-то течет память, 262 в конце любого теста, вне зависимости от содержимого
// ошибка с pqxx::connection, что-то с shared_ptr. как будто не отдает часть ресурсов
class DataBaseEnvironment : public ::testing::Test {
public:
    void TearDown() override {
      test_db.drop_tables();
    }
    Postgre_DB test_db = {"127.0.0.1", "5432", "test_db", "postgres", "postgres"};
    virtual ~DataBaseEnvironment() {}
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

TEST_F(DataBaseEnvironment, fing_chat_by_chat_name) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);
  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login()};
  Chat chat_1("Memeger", participants_1);
  test_db.add_chat(chat_1);

  bool status_find_chat = test_db.find_chat_by_chat_name("Memeger");
  EXPECT_TRUE(status_find_chat);
}

TEST_F(DataBaseEnvironment, change_chat_name) {
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

  int status_change_chat_name = test_db.change_chat_name(chat_from_db, "BigBrainTeam");
  EXPECT_EQ(status_change_chat_name, 0);
  EXPECT_EQ(chat_from_db.get_chat_name(), "BigBrainTeam");
}


TEST_F(DataBaseEnvironment, get_all_chats_by_user_login) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);
  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);
  User test_user_3("Yuri", "asdfas");
  test_db.add_user(test_user_3);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");
  User user_from_db_3 = test_db.get_user_by_login("Yuri");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login(), user_from_db_3.get_login()};
  Chat chat_1("Memeger", participants_1);
  test_db.add_chat(chat_1);
  Chat chat_from_db_1 = test_db.get_chat_by_chat_name("Memeger");

  std::vector<std::string> participants_2 = {user_from_db_1.get_login(), user_from_db_2.get_login()};
  Chat chat_2("Artem/Maxim", participants_2);
  test_db.add_chat(chat_2);
  Chat chat_from_db_2 = test_db.get_chat_by_chat_name("Artem/Maxim");

  std::vector<Chat> chats_by_user_login_1 = test_db.get_all_chats_by_user_login("Artem");
  EXPECT_EQ(chats_by_user_login_1.size(), 2);

  std::vector<std::string> true_chats_1 = {"Memeger", "Artem/Maxim"};
  for (size_t i = 0; i < chats_by_user_login_1.size(); ++i) {
    EXPECT_EQ(chats_by_user_login_1[i].get_chat_name(), true_chats_1[i]);
  }

  std::vector<Chat> chats_by_user_login_2 = test_db.get_all_chats_by_user_login("Yuri");
  EXPECT_EQ(chats_by_user_login_2.size(), 1);

  std::vector<std::string> true_chats_2 = {"Memeger"};
  for (size_t i = 0; i < chats_by_user_login_2.size(); ++i) {
    EXPECT_EQ(chats_by_user_login_2[i].get_chat_name(), true_chats_2[i]);
  }

}

TEST_F(DataBaseEnvironment, get_participants_from_chat) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);
  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);
  User test_user_3("Yuri", "asdfas");
  test_db.add_user(test_user_3);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");
  User user_from_db_3 = test_db.get_user_by_login("Yuri");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login(), user_from_db_3.get_login()};
  Chat chat_1("Memeger", participants_1);
  test_db.add_chat(chat_1);

  Chat chat_from_db = test_db.get_chat_by_chat_name("Memeger");
  EXPECT_EQ(chat_from_db.get_chat_name(), "Memeger");

  std::vector<std::string> true_participants = {"Artem", "Maxim", "Yuri"};
  std::vector<std::string> participants_from_chat = test_db.get_participants_from_chat(chat_from_db);
  for (size_t i = 0; i < participants_from_chat.size(); ++i) {
    EXPECT_EQ(participants_from_chat[i], true_participants[i]);
  }
}

TEST_F(DataBaseEnvironment, delete_chat) {
  User test_user_1("Artem", "missing");
  test_db.add_user(test_user_1);
  User test_user_2("Maxim", "finding");
  test_db.add_user(test_user_2);
  User test_user_3("Yuri", "asdfas");
  test_db.add_user(test_user_3);

  User user_from_db_1 = test_db.get_user_by_login("Artem");
  User user_from_db_2 = test_db.get_user_by_login("Maxim");
  User user_from_db_3 = test_db.get_user_by_login("Yuri");

  std::vector<std::string> participants_1 = {user_from_db_1.get_login(), user_from_db_2.get_login(), user_from_db_3.get_login()};
  Chat chat_1("Memeger", participants_1);
  test_db.add_chat(chat_1);

  Chat chat_from_db = test_db.get_chat_by_chat_name("Memeger");
  EXPECT_EQ(chat_from_db.get_chat_name(), "Memeger");
  EXPECT_TRUE(!chat_from_db.get_chat_id().empty());
  EXPECT_TRUE(!chat_from_db.get_participants().empty());

  int status_delete_chat = test_db.delete_chat(chat_from_db);
  EXPECT_EQ(status_delete_chat, 0);

  EXPECT_TRUE(chat_from_db.get_chat_id().empty());
  EXPECT_TRUE(chat_from_db.get_chat_name().empty());
  EXPECT_TRUE(chat_from_db.get_messages().empty());
  EXPECT_TRUE(chat_from_db.get_participants().empty());
}

TEST_F(DataBaseEnvironment, add_message) {
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

  TextMessage message(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "Hello, Maxim");
  EXPECT_EQ(test_db.add_message(message), 0);
  EXPECT_TRUE(!message.get_message_id().empty());
  EXPECT_EQ(message.get_parent_chat_id(), chat_from_db.get_chat_id());
  EXPECT_EQ(message.get_sender_id(), user_from_db_1.get_id());
  EXPECT_EQ(message.get_read_status(), false);
  EXPECT_EQ(message.get_message_text(), "Hello, Maxim");
}

TEST_F(DataBaseEnvironment, get_messages_from_chat) {
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

  TextMessage message_1(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "Hello, Maxim");
  EXPECT_EQ(test_db.add_message(message_1), 0);
  TextMessage message_2(chat_from_db.get_chat_id(), user_from_db_2.get_id(), "Hi");
  EXPECT_EQ(test_db.add_message(message_2), 0);
  TextMessage message_3(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "I have already done all my tests!");
  EXPECT_EQ(test_db.add_message(message_3), 0);
  TextMessage message_4(chat_from_db.get_chat_id(), user_from_db_2.get_id(), "Great news. Proud of you");
  EXPECT_EQ(test_db.add_message(message_4), 0);
  TextMessage message_5(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "Thanks! Let is discuss and integrate the project tonight?");
  EXPECT_EQ(test_db.add_message(message_5), 0);
  TextMessage message_6(chat_from_db.get_chat_id(), user_from_db_2.get_id(), "Sure, deal. See you");
  EXPECT_EQ(test_db.add_message(message_6), 0);

  std::vector<std::string> true_messages_text = {
                                                  "Hello, Maxim",
                                                  "Hi",
                                                  "I have already done all my tests!",
                                                  "Great news. Proud of you",
                                                  "Thanks! Let is discuss and integrate the project tonight?",
                                                  "Sure, deal. See you"
                                                };

  std::vector<TextMessage> messages_from_db = test_db.get_last_N_messages_from_chat(chat_from_db, -1);
  for (size_t i = 0; i < messages_from_db.size(); ++i) {
    EXPECT_EQ(messages_from_db[i].get_message_text(), true_messages_text[i]);
  }
}


TEST_F(DataBaseEnvironment, get_last_3_messages_from_chat) {
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

  TextMessage message_1(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "Hello, Maxim");
  EXPECT_EQ(test_db.add_message(message_1), 0);
  TextMessage message_2(chat_from_db.get_chat_id(), user_from_db_2.get_id(), "Hi");
  EXPECT_EQ(test_db.add_message(message_2), 0);
  TextMessage message_3(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "I have already done all my tests!");
  EXPECT_EQ(test_db.add_message(message_3), 0);
  TextMessage message_4(chat_from_db.get_chat_id(), user_from_db_2.get_id(), "Great news. Proud of you");
  EXPECT_EQ(test_db.add_message(message_4), 0);
  TextMessage message_5(chat_from_db.get_chat_id(), user_from_db_1.get_id(), "Thanks! Let is discuss and integrate the project tonight?");
  EXPECT_EQ(test_db.add_message(message_5), 0);
  TextMessage message_6(chat_from_db.get_chat_id(), user_from_db_2.get_id(), "Sure, deal. See you");
  EXPECT_EQ(test_db.add_message(message_6), 0);

  std::vector<std::string> true_messages_text = {
                                                  "Great news. Proud of you",
                                                  "Thanks! Let is discuss and integrate the project tonight?",
                                                  "Sure, deal. See you"
                                                };

  std::vector<TextMessage> messages_from_db = test_db.get_last_N_messages_from_chat(chat_from_db, 3);
  for (size_t i = 0; i < messages_from_db.size(); ++i) {
    EXPECT_EQ(messages_from_db[i].get_message_text(), true_messages_text[i]);
  }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
