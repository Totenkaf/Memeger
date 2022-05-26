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
  User() = default;
  User(const User& other) = default;
  User(User&& other) = default;

  auto operator=(const User&) -> User& = default;
  auto operator=(User&& other) -> User& = default;

  User(std::string user_id, std::string user_login,
       std::string user_password, std::string active_status)
      : user_login_(std::move(user_login)),
        user_password_(std::move(user_password)),
        user_id_(std::move(user_id)),
        active_status_(std::move(active_status)) {}

  User(std::string user_login, std::string user_password)
      : user_login_(std::move(user_login)),
        user_password_(std::move(user_password)) {}

  ~User() = default;

  [[nodiscard]] auto get_id() const -> std::string;
  [[nodiscard]] auto get_login() const -> std::string;
  [[nodiscard]] auto get_password() const -> std::string;
  [[nodiscard]] auto get_active_status() const -> std::string;

  void set_id(const std::string& user_id);
  void set_login(const std::string& user_login);
  void set_password(const std::string& user_password);
  void set_active_status(const std::string& active_status);

  void clear_user();

  auto operator==(const User& user) const -> bool;
  auto operator!=(const User& user) const -> bool;
};

// Добавить работу со временем отправки сообщения
// Попробовать реализовать картинку

class TextMessage : public IMessage {
 public:
  TextMessage() = default;
  TextMessage(const TextMessage& other) = default;
  TextMessage(TextMessage&& other) = default;

  auto operator=(const TextMessage&) -> TextMessage& = default;
  auto operator=(TextMessage&& other) -> TextMessage& = default;

  ~TextMessage() override = default;

  TextMessage(std::string parent_chat_id, std::string sender_id,
              std::string text_message)
      : parent_chat_id_(std::move(parent_chat_id)),
        sender_id_(std::move(sender_id)),
        text_message_(std::move(text_message)) {}

  [[nodiscard]] auto get_message_id() const -> std::string override;
  auto set_message_id(const std::string& message_id) -> int override;

  [[nodiscard]] auto get_parent_chat_id() const -> std::string override;
  auto set_parent_chat_id(const std::string& parent_chat_id) -> int override;

  [[nodiscard]] auto get_sender_id() const -> std::string override;
  auto set_sender_id(const std::string& sender_id) -> int override;

  [[nodiscard]] auto get_read_status() const -> bool override;
  auto set_read_status(bool is_read) -> int override;

  [[nodiscard]] auto get_message_text() const -> std::string;
  auto set_message_text(const std::string& text_message) -> int;

  void clear_message() override;

  // [[nodiscard]] auto get_address_id() const -> std::string override;
  // auto set_address_id(const std::string& address_id) -> int override;
  // [[nodiscard]] auto get_time_sent() const -> time_t;
  // auto set_time_sent(time_t time_sent) -> int;

 private:
  std::string message_id_;
  std::string parent_chat_id_;
  std::string sender_id_;
  std::string text_message_;
  bool is_read_ = false;

  // std::string address_id_;
  // time_t time_sent_;
};

// Добавить работу со временем создания чата
class Chat {
 public:
  Chat() = default;
  Chat(const Chat& other) = default;
  Chat(Chat&& other) = default;

  auto operator=(const Chat&) -> Chat& = default;
  auto operator=(Chat&& other) -> Chat& = default;

  Chat(std::string chat_id, std::string chat_name,
       std::vector<std::string> participants,
       std::vector<TextMessage> messages)
      : chat_id_(std::move(chat_id)),
        chat_name_(std::move(chat_name)),
        participants_(std::move(participants)),
        messages_(std::move(messages)) {}

  Chat(std::string chat_name, std::vector<std::string> participants)
      : chat_name_(std::move(chat_name)),
        participants_(std::move(participants)) {}

  Chat(std::string chat_id, std::string chat_name,
       std::vector<std::string> participants)
      : chat_id_(std::move(chat_id)),
        chat_name_(std::move(chat_name)),
        participants_(std::move(participants)) {}

  ~Chat() = default;

  [[nodiscard]] auto get_last_message() const -> TextMessage;

  [[nodiscard]] auto get_chat_name() const -> std::string;
  auto set_chat_name(const std::string& chat_name) -> int;

  [[nodiscard]] auto get_chat_id() const -> std::string;
  auto set_chat_id(const std::string& chat_id) -> int;

  // [[nodiscard]] auto get_chat_time_creation() const -> time_t;
  // auto set_chat_time_creation(const time_t& time_creation) -> int;

  auto set_participants(std::vector<std::string>& participants) -> int;
  [[nodiscard]] auto get_participants() const -> std::vector<std::string>;

  auto set_messages(const std::vector<TextMessage>& messages) -> int;
  [[nodiscard]] auto get_messages() const -> std::vector<TextMessage>;

  auto set_chat_messages(const std::vector<TextMessage>& messages) -> int;

  [[nodiscard]] auto is_empty() const -> bool;
  [[nodiscard]] auto is_dialogue() const -> bool;
  [[nodiscard]] auto is_monologue() const -> bool;
  [[nodiscard]] auto is_polilogue() const -> bool;

  auto operator!=(const Chat& chat) const -> bool;
  auto operator==(const Chat& chat) const -> bool;

  void clear_chat();

 private:
  std::string chat_id_;
  std::string chat_name_;
  std::vector<std::string> participants_;
  std::vector<TextMessage> messages_;
  // time_t time_creation_;
};
