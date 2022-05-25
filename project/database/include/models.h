// Copyright 2022 by Artem Ustsov

#pragma once
#include <string>
#include <utility>
#include <vector>

#include "imessage.h"

enum exit_status {
  _EXIT_SUCCESS = 0,
  _EXIT_FAULT,
  _TABLE_EXECUTION_FAULT,
  _TABLE_DELETION_FAULT,
  _INSERT_FAULT,
  _UPDATE_FAULT,
  _SAVE_FAULT,
  _DELETE_FAULT,
};

// Добавить работу со временем создания пользователя
class User {
 private:
  std::string user_login_;
  std::string user_password_;
  std::string user_id_;
  std::string active_status_;
  // time_t time_creation;

 public:
  User()
      : user_login_(std::string()),
        user_password_(std::string()),
        user_id_(std::string()),
        active_status_(std::string()) {}

  User(const std::string user_id, const std::string user_login,
       const std::string user_password, const std::string active_status)
      : user_login_(std::move(user_login)),
        user_password_(std::move(user_password)),
        user_id_(std::move(user_id)),
        active_status_(active_status) {}

  User(const std::string user_login, const std::string user_password)
      : user_login_(std::move(user_login)),
        user_password_(std::move(user_password)),
        user_id_(std::string()),
        active_status_(std::string()) {}

  ~User() = default;

  std::string get_id() const;
  std::string get_login() const;
  std::string get_password() const;
  std::string get_active_status() const;

  void set_id(const std::string& user_id);
  void set_login(const std::string& user_login);
  void set_password(const std::string& user_password);
  void set_active_status(const std::string& active_status);

  void clear_user();

  bool operator==(const User& user) const;
  bool operator!=(const User& user) const;
};

// Добавить работу со временем отправки сообщения
// Попробовать реализовать картинку

class TextMessage : public IMessage {
 public:
  TextMessage()
      : message_id_(std::string()),
        parent_chat_id_(std::string()),
        sender_id_(std::string()),
        text_message_(std::string()),
        is_read_(false) {}

  TextMessage(const std::string parent_chat_id, const std::string sender_id,
              const std::string text_message)
      : message_id_(std::string()),
        parent_chat_id_(parent_chat_id),
        sender_id_(sender_id),
        text_message_(std::move(text_message)),
        is_read_(false) {}

  ~TextMessage() = default;

  std::string get_message_id() const override;
  int set_message_id(const std::string& message_id) override;

  std::string get_parent_chat_id() const override;
  int set_parent_chat_id(const std::string& parent_chat_id) override;

  std::string get_sender_id() const override;
  int set_sender_id(const std::string& sender_id) override;

  bool get_read_status() const override;
  int set_read_status(bool is_read) override;

  std::string get_message_text() const;
  int set_message_text(const std::string& text_message);

  void clear_message() override;

  // std::string get_address_id() const override;
  // int set_address_id(const std::string& address_id) override;
  // time_t get_time_sent() const;
  // int set_time_sent(time_t time_sent);

 private:
  std::string message_id_;
  std::string parent_chat_id_;
  std::string sender_id_;
  std::string text_message_;
  bool is_read_;

  // std::string address_id_;
  // time_t time_sent_;
};

// Добавить работу со временем создания чата
class Chat {
 public:
  Chat() = default;

  Chat(const std::string chat_id, const std::string chat_name,
       std::vector<std::string>& participants,
       std::vector<TextMessage>& messages)
      : chat_id_(chat_id),
        chat_name_(chat_name),
        participants_(participants),
        messages_(messages) {}

  Chat(const std::string chat_name, std::vector<std::string>& participants)
      : chat_id_(std::string()),
        chat_name_(chat_name),
        participants_(participants),
        messages_(std::vector<TextMessage>()) {}

  Chat(const std::string chat_id, const std::string chat_name,
       std::vector<std::string>& participants)
      : chat_id_(chat_id),
        chat_name_(chat_name),
        participants_(participants),
        messages_(std::vector<TextMessage>()) {}

  ~Chat() = default;

  TextMessage get_last_message() const;

  std::string get_chat_name() const;
  int set_chat_name(const std::string& chat_name);

  std::string get_chat_id() const;
  int set_chat_id(const std::string chat_id);

  // time_t get_chat_time_creation() const;
  // int set_chat_time_creation(const time_t& time_creation);

  int set_participants(std::vector<std::string>& participants);
  std::vector<std::string> get_participants() const;

  int set_messages(const std::vector<TextMessage>& messages);
  std::vector<TextMessage> get_messages() const;

  // int push_new_message(TextMessage new_message);
  int set_chat_messages(const std::vector<TextMessage>& messages);

  bool is_empty() const;
  bool is_dialogue() const;
  bool is_monologue() const;
  bool is_polilogue() const;

  bool operator!=(const Chat& chat) const;
  bool operator==(const Chat& chat) const;

  void clear_chat();

 private:
  std::string chat_id_;
  std::string chat_name_;
  std::vector<std::string> participants_;
  std::vector<TextMessage> messages_;
  // time_t time_creation_;
};
