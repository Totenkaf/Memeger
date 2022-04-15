//Copyright 2022 by Artem Ustsov

#include <string>
#include <set>
#include <utility>
#include <vector>

class User {
private:
    std::string user_login_;
    std::string user_password_;
    std::string user_token_;
    int active_status_;

public:
    User() = default;
    ~User() = default;

    User(std::string user_login, std::string user_password,
         std::string user_token, std::vector<std::string> chat_list_, int active_status) :
            user_login_(std::move(user_login)), user_password_(std::move(user_password)),
            user_token_(std::move(user_token)), active_status_(active_status) {
    }

    User(std::string user_login, std::string user_password,
         std::string user_token, int active_status) :
            user_login_(std::move(user_login)), user_password_(std::move(user_password)),
            user_token_(std::move(user_token)), active_status_(active_status) {
    }

    std::string get_login() const;
    std::string get_password() const;
    std::string get_token() const;


    int get_active_status() const;

    void set_login(const std::string &user_login);
    void set_password(const std::string &user_password);

    void set_token(const std::string &user_token);
    void set_active_status(int active_status);
};


class TextMessage {
public:
    TextMessage() = default;
    TextMessage(std::string message_id, std::string parent_chat_id, std::string sender_id,
            std::string text_message, time_t time_sent, bool is_read) :
            message_id_(message_id), parent_chat_id_(parent_chat_id), sender_id_(sender_id),
            text_message_(std::move(text_message)), 
            time_sent_(time_sent), is_read_(is_read) {
    }
    ~TextMessage() = default;

    std::string get_message_id() const;
    std::string get_parent_chat_id() const;
    std::string get_sender_id() const;
    std::string get_message_text() const;
    time_t get_time_sent() const;
    bool is_read() const;

private:
    std::string message_id_;
    std::string parent_chat_id_;
    std::string sender_id_;
    std::string text_message_;
    time_t time_sent_;
    bool is_read_;
};


class Chat {
public:
    Chat() = default;
    Chat(std::string chat_id,
            std::vector<std::string> participants,
            std::vector<TextMessage> messages) :
            chat_id_(chat_id),
            participants_(participants),
            messages_(messages) {}

    ~Chat() = default;

    TextMessage get_last_message() const;
    time_t get_chat_time_creation() const;
    std::string get_chat_id() const;

    std::vector<std::string> get_participants() const;
    std::vector<TextMessage> get_messages() const;


    void push_new_message(const TextMessage& new_message);
    void add_new_participant(std::string new_participant_id);


    bool is_empty() const;
    bool is_dialogue() const;
    bool is_monologue() const;
    bool is_polilogue() const;

private:
    std::string chat_id_;
    std::vector<TextMessage> messages_;
    std::vector<std::string> participants_;
    time_t date_creation;
};


struct ComparatorChat {
    bool operator()(const Chat &lhs, const Chat &rhs) {
        return lhs.get_last_message().get_time_sent() > rhs.get_last_message().get_time_sent();
    }
};

typedef std::multiset<Chat, ComparatorChat> chat_list;
