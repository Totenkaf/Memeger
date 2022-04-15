//Copyright 2022 by Artem Ustsov

#include "models.h"


std::string User::get_login() const { return user_login_; }
std::string User::get_password() const { return user_password_; }
std::string User::get_token() const { return user_token_; }
int User::get_active_status() const { return active_status_; }

void User::set_password(const std::string &user_password) {
    user_password_ = user_password;
}

void User::set_token(const std::string &user_token) {
    user_token_ = user_token;
}

void User::set_active_status(int active_status) {
    active_status_ = active_status;
}



std::string TextMessage::get_message_id() const {
    return message_id_;
}

std::string TextMessage::get_parent_chat_id() const {
    return parent_chat_id_;
}

std::string TextMessage::get_sender_id() const {
    return sender_id_;
}

std::string TextMessage::get_message_text() const {
    return text_message_;
}

time_t TextMessage::get_time_sent() const {
    return time_sent_;
}

bool TextMessage::is_read() const {
    return is_read_;
}

std::vector<TextMessage> Chat::get_messages() const{
    return messages_;
}





TextMessage Chat::get_last_message() const {
    return *messages_.begin();
}

std::vector<std::string> Chat::get_participants() const {
    return participants_;
}

std::string Chat::get_chat_id() const {
    return chat_id_;
}

void Chat::push_new_message(const TextMessage &new_message) {
    messages_.insert(messages_.begin(), new_message);
}

void Chat::add_new_participant(std::string new_participant_id) {
    participants_.push_back(new_participant_id);
}

bool Chat::is_empty() const {
    return participants_.size() == 0;
}

bool Chat::is_monologue() const {
    return participants_.size() == 1;
}

bool Chat::is_dialogue() const {
    return participants_.size() == 2;
}

bool Chat::is_polilogue() const {
    return participants_.size() > 2;
}
