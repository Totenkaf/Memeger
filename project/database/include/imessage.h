// Copyright 2022 by Artem Ustsov

#pragma once
#include <ctime>
#include <iostream>
#include <string>

class IMessage {
 public:
  virtual ~IMessage() {}

  virtual std::string get_message_id() const = 0;
  virtual int set_message_id(const std::string& message_id) = 0;

  virtual std::string get_parent_chat_id() const = 0;
  virtual int set_parent_chat_id(const std::string& parent_chat_id) = 0;

  virtual std::string get_sender_id() const = 0;
  virtual int set_sender_id(const std::string& sender_id) = 0;

  // virtual std::string get_address_id() const = 0;
  // virtual int set_address_id(const std::string& address_id) = 0;

  // virtual time_t get_time_sent() const = 0;
  // virtual int set_time_sent(time_t time_sent) = 0;

  virtual bool get_read_status() const = 0;
  virtual int set_read_status(bool is_read) = 0;

  virtual void clear_message() = 0;
};
