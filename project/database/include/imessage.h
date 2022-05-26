// Copyright 2022 by Artem Ustsov

#pragma once
#include <ctime>
#include <iostream>
#include <string>

class IMessage {
public:
  virtual ~IMessage() = default;
  [[nodiscard]] virtual auto get_message_id() const -> std::string = 0;
  [[nodiscard]] virtual auto set_message_id(const std::string& message_id) -> int = 0;

  [[nodiscard]] virtual auto get_parent_chat_id() const -> std::string = 0;
  [[nodiscard]] virtual auto set_parent_chat_id(const std::string& parent_chat_id) -> int = 0;

  [[nodiscard]] virtual auto get_sender_id() const -> std::string = 0;
  [[nodiscard]] virtual auto set_sender_id(const std::string& sender_id) -> int = 0;

  // [[nodiscard]] virtual auto::string get_address_id() const -> std::string = 0;
  // [[nodiscard]] virtual auto set_address_id(const std::string& address_id) -> int = 0;

  // [[nodiscard]] virtual auto get_time_sent() const -> time_t = 0;
  // [[nodiscard]] virtual auto set_time_sent(time_t time_sent) -> int = 0;

  [[nodiscard]] virtual auto get_read_status() const -> bool = 0;
  [[nodiscard]] virtual auto set_read_status(bool is_read) -> int = 0;

  virtual void clear_message() = 0;
};
