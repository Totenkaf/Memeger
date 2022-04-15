//Copyright 2022 by Artem Ustsov

#pragma once
#include <iostream>
#include <string>
#include <ctime>


class IMessage {
private:
    virtual std::string get_message_id() const = 0;
    virtual std::string get_parent_chat_id() const = 0;
    virtual std::string get_sender_id() const = 0;
    virtual time_t get_time_sent() const = 0;
    virtual bool is_read() const = 0;
};
