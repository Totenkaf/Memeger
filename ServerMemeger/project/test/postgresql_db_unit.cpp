//Copyright 2022 by Artem Ustsov
#include <gtest/gtest.h>
#include "postgresql_db.h"
#include "models.h"


TEST(POSTGRE_DB, test_user_login) {
  Postgre_DB pg("127.0.0.1", "5432", "test_db", "tester", "test_password");
  User user;
  user.user_id = pg.user_register("mail.ru", "1049284876844");
  pg.save_user(user);
  int new_user_id = pg.user_id("mail.ru");
  string user_login = pg.user_login(0);
  USERS_INFO user2 = pg.user_info("mail.ru");
  string name = user2.name;
  EXPECT_EQ(new_user_id, 0);
  EXPECT_EQ(user_login, "mail.ru");
  EXPECT_EQ(name, "USER1");
  EXPECT_EQ(pg.user_exist("mail.ru"), 1);
  EXPECT_EQ(pg.user_exist("mail.ru", "1049284876844"), 1);
  EXPECT_EQ(pg.user_exist("mail.ru", "1049284876843"), 0);
  EXPECT_EQ(pg.user_exist("vk.ru"), 0);
  EXPECT_EQ(pg.user_exist("vk.ru", "1049284876844"), 0);
}

TEST(POSTGRE_DB, test_set_mark) {
  Postgre_DB pg("127.0.0.1", "5432", "test_db", "tester", "test_password");
  User user;
  user.user_id = pg.user_register("vk.ru", "efefegr34t34");
  user.name = "USER2";
  pg.save_user(user);
  pg.set_mark("mail.ru", "vk.ru", 1);
  pg.set_mark("vk.ru", "mail.ru", 1);
  int new_user_id = pg.user_id("vk.ru");
  EXPECT_EQ(new_user_id, 1);
  EXPECT_EQ(pg.gender_is_different(pg.user_id("vk.ru"), pg.user_id("mail.ru")), 1);
  EXPECT_EQ(pg.is_pair(pg.user_id("vk.ru"), pg.user_id("mail.ru")), 1);
}

TEST(POSTGRE_DB, test_drop_tables) {
  Postgre_DB pg("127.0.0.1", "5432", "test_db", "tester", "test_password");
  pg.drop_tables();
  EXPECT_EQ(pg.user_exist("vk.ru"), 0);
  EXPECT_EQ(pg.user_exist("mail.ru"), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}