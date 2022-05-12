// Copyright 2022 by Artem Ustsov

#pragma once
#include <ctime>
#include <iostream>
#include <string>

class IMessage {
 private:
  virtual std::string get_message_id() const = 0;
  virtual int set_message_id(std::string message_id) = 0;

  virtual std::string get_parent_chat_id() const = 0;
  virtual int set_parent_chat_id(std::string chat_id) = 0;

  virtual std::string get_sender_login() const = 0;
  virtual int set_sender_login(std::string sender_login) = 0;

  virtual time_t get_time_sent() const = 0;
  virtual int set_time_sent(time_t) = 0;

  virtual bool is_read() const = 0;
};
