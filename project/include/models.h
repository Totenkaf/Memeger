//Copyright 2022 by Artem Ustsov

#include "imessage.h"

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


class TextMessage : public IMessage {
public:
    TextMessage() = default;
    TextMessage(std::string message_id, std::string parent_chat_id, std::string sender_id,
                std::string text_message, time_t time_sent, bool is_read) :
                message_id_(message_id), parent_chat_id_(parent_chat_id), sender_id_(sender_id),
                text_message_(std::move(text_message)), 
                time_sent_(time_sent), is_read_(is_read) {
    }
    ~TextMessage() = default;

    std::string get_message_id() const override;
    int set_message_id(std::string message_id) override;
    
    std::string get_parent_chat_id() const override;
    int set_parent_chat_id(std::string parent_chat_id) override;

    std::string get_sender_id() const override;
    int set_sender_id(std::string sender_id) override;

    time_t get_time_sent() const override;
    int set_time_sent(time_t time_sent) override;
    bool is_read() const override;

    std::string get_message_text() const;

private:
    std::string message_id_;
    std::string parent_chat_id_;
    std::string sender_id_;
    std::string text_message_;
    time_t time_sent_;
    bool is_read_;
};


class Picture {
private:
    int weight_;
    int height_;
    std::vector<int> R;
    std::vector<int> G;
    std::vector<int> B;
public:
    Picture() = default;
    ~Picture() = default;
};


class MemMessage : public IMessage {
public:
    MemMessage() = default;
    MemMessage(std::string message_id, std::string parent_chat_id, std::string sender_id,
                Picture& mem_picture, time_t time_sent, bool is_read) :
                message_id_(message_id), parent_chat_id_(parent_chat_id), sender_id_(sender_id),
                mem_picture_(std::move(mem_picture)), 
                time_sent_(time_sent), is_read_(is_read) {
    }
    ~MemMessage() = default;

    std::string get_message_id() const override;
    int set_message_id(std::string message_id) override;
    
    std::string get_parent_chat_id() const override;
    int set_parent_chat_id(std::string parent_chat_id) override;

    std::string get_sender_id() const override;
    int set_sender_id(std::string sender_id) override;

    time_t get_time_sent() const override;
    int set_time_sent(time_t time_sent) override;
    bool is_read() const override;

    Picture get_message_picture() const;

private:
    std::string message_id_;
    std::string parent_chat_id_;
    std::string sender_id_;
    std::string mem_name_;
    Picture mem_picture_;
    time_t time_sent_;
    bool is_read_;
};


class Audio {
private:
    size_t channels_;
    size_t duration_;
    size_t sample_rate_;
    size_t buffer_size;
    std::vector<std::string> buffer;

public:
    Audio() = default;
    ~Audio() = default;
    void play();
    void record();    
};


class AudioMessage : public IMessage {
public:
    AudioMessage() = default;
    AudioMessage(std::string message_id, std::string parent_chat_id, std::string sender_id,
                Audio& audio_message, time_t time_sent, bool is_read) :
                message_id_(message_id), parent_chat_id_(parent_chat_id), sender_id_(sender_id),
                audio_message_(std::move(audio_message)), 
                time_sent_(time_sent), is_read_(is_read) {
    }
    ~AudioMessage() = default;

    std::string get_message_id() const override;
    int set_message_id(std::string message_id) override;
    
    std::string get_parent_chat_id() const override;
    int set_parent_chat_id(std::string parent_chat_id) override;

    std::string get_sender_id() const override;
    int set_sender_id(std::string sender_id) override;

    time_t get_time_sent() const override;
    int set_time_sent(time_t time_sent) override;
    bool is_read() const override;

    Audio get_message_audio() const;

    void play_audio(const Audio&);
    void stop_audio(const Audio&);
    std::string transfer_to_text(const Audio&);
    

private:
    std::string message_id_;
    std::string parent_chat_id_;
    std::string sender_id_;
    Audio audio_message_;
    time_t time_sent_;
    bool is_read_;
};


class Chat {
public:
    Chat() = default;
    Chat(std::string chat_id,
            std::vector<std::string> participants,
            std::vector<IMessage&> messages) :
            chat_id_(chat_id),
            participants_(participants),
            messages_(messages) {}

    ~Chat() = default;

    IMessage& get_last_message() const;
    time_t get_chat_time_creation() const;
    std::string get_chat_id() const;

    std::vector<std::string> get_participants() const;
    std::vector<IMessage&> get_messages() const;


    int push_new_message(const IMessage& new_message);
    int add_new_participant(std::string& new_participant_id);

    bool is_empty() const;
    bool is_dialogue() const;
    bool is_monologue() const;
    bool is_polilogue() const;

private:
    std::string chat_id_;
    std::vector<IMessage&> messages_;
    std::vector<std::string> participants_;
    time_t date_creation;
};
