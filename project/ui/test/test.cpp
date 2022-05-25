// Copyright 2022 Yaroslav Zhovtyak

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <locale>
#include <codecvt>
/*
 using ::testing::Return;
 class MockDb : public IMainDb {
 public:
     MOCK_METHOD(void, sendMessage, (std::string message), (override));
     MOCK_METHOD(vector<string>, findQuery, (std::string query), (override));
 };

 TEST(searchUserLogin, InCorrectData) {
     MockDb mock;
     EXPECT_CALL(mock, searchUserLogin(std::string("log"), std::string("pas"))).WillOnce(Return(nullptr));
     EXPECT_EQ(mock.searchUserLogin(std::string("log"), std::string("pas")), nullptr);
 }

 TEST(getCodeFromMessage, CorrectData) {
     MockDb mock;
     Chat test_chat;
     vector<string> test_participants;
     EXPECT_CALL(mock, get_chat_participants()).WillOnce(Return(test_participants));
     EXPECT_CALL(mock, add_user()).WillOnce(Return(""));
 }

 TEST(Search_Memeger, CorrectData) {
     MockDb mock;
     Search search;
     vector<string> answer;
     EXPECT_CALL(mock, search_by_query(query)).WillOnce(Return(answer));
     EXPECT_EQ(search.get_query(), "test");
 }

 TEST(User_Info, CorrectData) {
     UserInfo user_info;
     Picture test_picture;
     EXPECT_EQ(userInfo.get_name, "test");
     EXPECT_EQ(userInfo.get_id, "111");
     EXPECT_EQ(userInfo.get_profile_picture, test_picture);
 }
*/
