// Copyright 2022 by Artem Ustsov

#include <gtest/gtest.h>
#include <memory>

#include "models.h"

class ModelsTest : public ::testing::Test {
public:
  User user;
  Chat chat;
  std::shared_ptr<IMessage> message = std::make_shared<TextMessage>();
  virtual ~ModelsTest() {}
};

TEST_F(ModelsTest, test_user) {
  user.set_id("ajdfa-adsf64-6a46sdf6-adsf46");
  user.set_login("Artem");
  user.set_password("Qwerty12345");
  user.set_active_status("active");

  EXPECT_EQ(user.get_id(), "ajdfa-adsf64-6a46sdf6-adsf46");
  EXPECT_EQ(user.get_login(), "Artem");
  EXPECT_EQ(user.get_password(), "Qwerty12345");
  EXPECT_EQ(user.get_active_status(), "active");
}

TEST_F(ModelsTest, test_chat) {
  chat.set_chat_id("ajdfa-adsf64-6a46sdf6-adsf46");
  chat.set_chat_name("Memeger");

  std::vector<std::string> participants = {"Artem", "Maxim", "Yuri",
                                           "Yaroslav"};
  chat.set_participants(participants);
  EXPECT_TRUE(participants.empty());

  std::vector<std::shared_ptr<IMessage>> messages;
  message->set_message_id("lkaslma-65a16sd1f-a6ds1f6");
  message->set_message_content("Hello");
  messages.emplace_back(message);

  chat.set_chat_messages(messages);

  EXPECT_EQ(chat.get_chat_id(), "ajdfa-adsf64-6a46sdf6-adsf46");
  EXPECT_EQ(chat.get_chat_name(), "Memeger");
  EXPECT_TRUE(chat.is_polilogue());

  std::vector<std::string> participants_1 = {"Artem", "Maxim", "Yuri",
                                             "Yaroslav"};
  for (size_t i = 0; i < chat.get_participants().size(); ++i) {
    EXPECT_EQ(chat.get_participants()[i], participants_1[i]);
  }
  EXPECT_EQ(chat.get_messages()[0]->get_message_content(),
            message->get_message_content());

  chat.clear_chat();
  EXPECT_TRUE(chat.get_chat_id().empty());
  EXPECT_TRUE(chat.get_messages().empty());
  EXPECT_TRUE(chat.get_participants().empty());
  EXPECT_TRUE(chat.get_chat_name().empty());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
