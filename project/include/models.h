//Copyright 2022 by Artem Ustsov

#pragma once
#include <string>
#include <set>
#include <utility>
#include <vector>


/* Добавить работу со временем создания пользователя */
class User {
private:
    std::string user_login_;
    std::string user_password_;
    std::string user_id_;
    std::string active_status_;
    // time_t time_creation;

public:
    User()
    : user_login_(std::string()), user_password_(std::string()), 
      user_id_(std::string()), active_status_(std::string()) 
    {}

    User(std::string user_id, std::string user_login, 
    std::string user_password, std::string active_status) 
    : user_login_(std::move(user_login)), user_password_(std::move(user_password)),
      user_id_(std::move(user_id)), active_status_(active_status) 
    {}


    User(std::string user_login, 
    std::string user_password) 
    : user_login_(std::move(user_login)), user_password_(std::move(user_password)),
      user_id_(std::string()), active_status_(std::string()) 
    {}

    ~User() = default;

    std::string get_id() const;
    std::string get_login() const;
    std::string get_password() const;
    std::string get_active_status() const;

    void set_id(const std::string &user_id);
    void set_login(const std::string &user_login);
    void set_password(const std::string &user_password);
    void set_active_status(const std::string &active_status);

    bool operator==(const User &user) const;
    bool operator!=(const User &user) const;
};

/* Добавить работу со временем отправки сообщения */
class TextMessage {
public:
    TextMessage()
    : message_id_(std::string()), parent_chat_id_(std::string()),
    sender_id_(std::string()), address_id_(std::string()),
    text_message_(std::string()), is_read_(false) {}

    TextMessage(std::string parent_chat_id, 
                std::string sender_id, std::string address_id,
                std::string text_message) :
                message_id_(std::string()), parent_chat_id_(parent_chat_id), 
                sender_id_(sender_id), address_id_(address_id),
                text_message_(std::move(text_message)), 
                is_read_(false) 
    {}

    ~TextMessage() = default;

    std::string get_message_id() const;
    int set_message_id(const std::string& message_id);
    
    std::string get_parent_chat_id() const;
    int set_parent_chat_id(const std::string& parent_chat_id);

    std::string get_sender_id() const;
    int set_sender_id(const std::string& sender_id);

    std::string get_address_id() const;
    int set_address_id(const std::string& address_id);

    // time_t get_time_sent() const;
    // int set_time_sent(time_t time_sent);
    bool get_read_status() const;
    int set_read_status(bool is_read);

    std::string get_message_text() const;

private:
    std::string message_id_;
    std::string parent_chat_id_;
    std::string sender_id_;
    std::string address_id_; /* пока непонятно, нужно ли это держать */
    std::string text_message_;
    // time_t time_sent_;
    bool is_read_;
};


// class Picture {
// private:
//     int weight_;
//     int height_;
//     std::vector<int> R;
//     std::vector<int> G;
//     std::vector<int> B;
// public:
//     Picture() = default;
//     ~Picture() = default;
// };


// class MemMessage : public IMessage {
// public:
//     MemMessage() = default;
//     MemMessage(std::string message_id, std::string parent_chat_id, std::string sender_id,
//                 Picture& mem_picture, time_t time_sent, bool is_read) :
//                 message_id_(message_id), parent_chat_id_(parent_chat_id), sender_id_(sender_id),
//                 mem_picture_(std::move(mem_picture)), 
//                 time_sent_(time_sent), is_read_(is_read) {
//     }
//     ~MemMessage() = default;

//     std::string get_message_id() const;
//     int set_message_id(std::string message_id);
    
//     std::string get_parent_chat_id() const;
//     int set_parent_chat_id(std::string parent_chat_id);

//     std::string get_sender_id() const;
//     int set_sender_id(std::string sender_id);

//     time_t get_time_sent() const;
//     int set_time_sent(time_t time_sent);
//     bool is_read() const;

//     Picture get_message_picture() const;

// private:
//     std::string message_id_;
//     std::string parent_chat_id_;
//     std::string sender_id_;
//     std::string mem_name_;
//     Picture mem_picture_;
//     time_t time_sent_;
//     bool is_read_;
// };


// class Audio {
// private:
//     size_t channels_;
//     size_t duration_;
//     size_t sample_rate_;
//     size_t buffer_size;
//     std::vector<std::string> buffer;

// public:
//     Audio() = default;
//     ~Audio() = default;
//     void play();
//     void record();    
// };


// class AudioMessage : public IMessage {
// public:
//     AudioMessage() = default;
//     AudioMessage(std::string message_id, std::string parent_chat_id, std::string sender_id,
//                 Audio& audio_message, time_t time_sent, bool is_read) :
//                 message_id_(message_id), parent_chat_id_(parent_chat_id), sender_id_(sender_id),
//                 audio_message_(std::move(audio_message)), 
//                 time_sent_(time_sent), is_read_(is_read) {
//     }
//     ~AudioMessage() = default;

//     std::string get_message_id() const;
//     int set_message_id(std::string message_id);
    
//     std::string get_parent_chat_id() const;
//     int set_parent_chat_id(std::string parent_chat_id);

//     std::string get_sender_id() const;
//     int set_sender_id(std::string sender_id);

//     time_t get_time_sent() const;
//     int set_time_sent(time_t time_sent);
//     bool is_read() const;

//     Audio get_message_audio() const;

//     void play_audio(const Audio&);
//     void stop_audio(const Audio&);
//     std::string transfer_to_text(const Audio&);
    

// private:
//     std::string message_id_;
//     std::string parent_chat_id_;
//     std::string sender_id_;
//     Audio audio_message_;
//     time_t time_sent_;
//     bool is_read_;
// };


/* Добавить работу со временем создания чата */
class Chat {
public:
    Chat() = default;

    Chat(std::string chat_id, std::string chat_name,
            std::vector<std::string>& participants,
            std::vector<TextMessage>& messages)
            :
            chat_id_(chat_id),
            chat_name_(chat_name),
            participants_(participants),
            messages_(messages)
    {}

    Chat(std::string chat_name,
        std::vector<std::string>& participants)
        :
        chat_id_(std::string()),
        chat_name_(chat_name),
        participants_(participants),
        messages_(std::vector<TextMessage>())
    {}

    Chat(std::string chat_id, std::string chat_name,
        std::vector<std::string>& participants)
        :
        chat_id_(chat_id),
        chat_name_(chat_name),
        participants_(participants),
        messages_(std::vector<TextMessage>())
    {}

    ~Chat() = default;

    TextMessage get_last_message() const;

    std::string get_chat_name() const;
    int set_chat_name(const std::string& chat_name);

    std::string get_chat_id() const;
    int set_chat_id(std::string chat_id);

    time_t get_chat_time_creation() const;
    int set_chat_time_creation(const time_t& time_creation);
    
    int set_participants(std::vector<std::string>& participants);
    std::vector<std::string> get_participants() const;
    std::vector<TextMessage> get_messages() const;

    int push_new_message(TextMessage new_message);
    int add_new_participant(std::string& new_participant_id);

    bool is_empty() const;
    bool is_dialogue() const;
    bool is_monologue() const;
    bool is_polilogue() const;

    bool operator!=(const Chat &chat) const;
    bool operator==(const Chat &chat) const;

private:
    std::string chat_id_;
    std::string chat_name_;
    std::vector<std::string> participants_;
    std::vector<TextMessage> messages_;
    // time_t time_creation_;
};
