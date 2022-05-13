#pragma once

#include <iostream>
#include <vector>
#include <string>

class Chat {
private:
    std::vector <std::string> chat_participants;
public:

    Chat() = default;

    ~Chat() = default;

    Chat(const Chat &chat);

    Chat(std::string token);

    Chat(std::vector <std::string> token_vector);

    void send_message(std::string message);

    void send_picture(Picture picture);

    void send_meme(MemMessage meme);

    void send_audio(Audio audio);

    void send_voicemail(AudioMessage voicemail);

    void add_user(std::string token);

    void kick_user(std::string token);

    void ban_user(std::string token);

    std::vector <std::string> get_chat_participants();

    void set_chat_participants(std::vector <std::string> new_chat_participants);
};
