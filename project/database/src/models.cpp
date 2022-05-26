// Copyright 2022 by Artem Ustsov

#include "models.h"

auto User::get_login() const -> std::string { return user_login_; }

auto User::get_password() const -> std::string { return user_password_; }

auto User::get_id() const -> std::string { return user_id_; }

auto User::get_active_status() const -> std::string { return active_status_; }

void User::set_login(const std::string& user_login) {
  user_login_ = user_login;
}

void User::set_password(const std::string& user_password) {
  user_password_ = user_password;
}

void User::set_id(const std::string& user_id) { user_id_ = user_id; }

void User::set_active_status(const std::string& active_status) {
  active_status_ = active_status;
}

auto User::operator==(const User& user) const -> bool {
  return user_login_ == user.get_login();
}

auto User::operator!=(const User& user) const -> bool {
  return user_login_ != user.get_login();
}

void User::clear_user() {
  user_login_.clear();
  user_password_.clear();
  user_id_.clear();
  active_status_.clear();
}

auto TextMessage::get_message_id() const -> std::string { return message_id_; }

auto TextMessage::set_message_id(const std::string& message_id) -> int {
  message_id_ = message_id;
  return _EXIT_SUCCESS;
}

auto TextMessage::get_parent_chat_id() const -> std::string { return parent_chat_id_; }

auto TextMessage::set_parent_chat_id(const std::string& parent_chat_id) -> int {
  parent_chat_id_ = parent_chat_id;
  return _EXIT_SUCCESS;
}

auto TextMessage::get_sender_id() const -> std::string { return sender_id_; }

auto TextMessage::set_sender_id(const std::string& sender_id) -> int {
  sender_id_ = sender_id;
  return _EXIT_SUCCESS;
}

auto TextMessage::get_message_text() const -> std::string { return text_message_; }

auto TextMessage::set_message_text(const std::string& text_message) -> int {
  text_message_ = text_message;
  return _EXIT_SUCCESS;
}

// time_t TextMessage::get_time_sent() const {
//     return time_sent_;
// }

auto TextMessage::get_read_status() const -> bool { return is_read_; }

auto TextMessage::set_read_status(bool is_read) -> int {
  is_read_ = is_read;
  return _EXIT_SUCCESS;
}

auto Chat::get_messages() const -> std::vector<TextMessage> { return messages_; }

void TextMessage::clear_message() {
  message_id_.clear();
  parent_chat_id_.clear();
  sender_id_.clear();
  text_message_.clear();
  is_read_ = false;
}

auto Chat::get_last_message() const -> TextMessage { return messages_.back(); }

auto Chat::get_participants() const -> std::vector<std::string> {
  return participants_;
}

auto Chat::set_participants(std::vector<std::string>& participants) -> int {
  participants_ = std::move(participants);
  return _EXIT_SUCCESS;
}

auto Chat::get_chat_id() const -> std::string { return chat_id_; }

auto Chat::set_chat_id(const std::string& chat_id) -> int {
  chat_id_ = chat_id;
  return _EXIT_SUCCESS;
}

auto Chat::get_chat_name() const -> std::string { return chat_name_; }

auto Chat::set_chat_name(const std::string& chat_name) -> int {
  chat_name_ = chat_name;
  return _EXIT_SUCCESS;
}

// time_t Chat::get_chat_time_creation() const {
//     return date_creation_;
// }

// int Chat::set_chat_time_creation(const time_t& date_creation) {
//     date_creation_ = date_creation;
//     return 0;
// }

auto Chat::set_chat_messages(const std::vector<TextMessage>& messages) -> int {
  messages_ = messages;
  return _EXIT_SUCCESS;
}

auto Chat::operator==(const Chat& chat) const -> bool {
  return chat_id_ == chat.get_chat_id();
}

auto Chat::operator!=(const Chat& chat) const -> bool {
  return chat_id_ != chat.get_chat_id();
}

auto Chat::is_empty() const -> bool { return participants_.empty(); }

auto Chat::is_monologue() const -> bool { return participants_.size() == 1; }

auto Chat::is_dialogue() const -> bool { return participants_.size() == 2; }

auto Chat::is_polilogue() const -> bool { return participants_.size() > 2; }

void Chat::clear_chat() {
  chat_id_.clear();
  chat_name_.clear();
  participants_.clear();
  messages_.clear();
}
