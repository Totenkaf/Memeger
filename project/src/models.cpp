//Copyright 2022 by Artem Ustsov

#include "models.h"
#include "imessage.h"

std::string User::get_login() const { return user_login_; }
std::string User::get_password() const { return user_password_; }
std::string User::get_token() const { return user_token_; }
int User::get_active_status() const { return active_status_; }

void User::set_login(const std::string &user_login) { user_login_ = user_login; }
void User::set_password(const std::string &user_password) { user_password_ = user_password; }
void User::set_token(const std::string &user_token) { user_token_ = user_token; }
void User::set_active_status(int active_status) { active_status_ = active_status; }

bool User::operator==(const User &user) const { return user_login_ == user.get_login(); }
bool User::operator!=(const User &user) const { return user_login_ != user.get_login(); }


std::string TextMessage::get_message_id() const { return message_id_; }
int TextMessage::set_message_id(std::string message_id) { message_id_ = message_id; }
std::string TextMessage::get_parent_chat_id() const { return parent_chat_id_; }
std::string TextMessage::get_sender_id() const { return sender_id_; }
std::string TextMessage::get_message_text() const { return text_message_; }
time_t TextMessage::get_time_sent() const { return time_sent_; }
bool TextMessage::is_read() const {return is_read_; }



std::vector<IMessage&> Chat::get_messages() const{
    return messages_;
}

IMessage& Chat::get_last_message() const {
    return messages_.back();
}

std::vector<std::string> Chat::get_participants() const {
    return participants_;
}

std::string Chat::get_chat_id() const {
    return chat_id_;
}

int Chat::set_chat_id(std::string chat_id) {
    chat_id_ = chat_id;
}

int Chat::push_new_message(IMessage& new_message) {
    messages_.push_back(new_message);
}

int Chat::add_new_participant(std::string& new_participant_id) {
    participants_.push_back(new_participant_id);
}


bool Chat::operator==(const Chat &chat) const {
    return chat_id_ == chat.get_chat_id();
}

bool Chat::operator!=(const Chat &chat) const {
    return chat_id_ != chat.get_chat_id();
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
