// Copyright 2022 by Artem Ustsov

#include <gtest/gtest.h>
#include "models.h"

class ModelsTest : public ::testing::Test {
public:
    User user;
    Chat chat;
    TextMessage message;
    virtual ~ModelsTest() {}
};

TEST(MODELS, add_test_user) {
}

TEST(MODELS, change_test_user_login) {
}

TEST(MODELS, change_test_user_password) {
}

TEST(MODELS, find_test_user_by_login) {
}

TEST(MODELS, delete_user) {
}

TEST(MODELS, add_chat) {
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
