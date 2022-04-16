// Copyright 2022 Yaroslav Zhovtyak
#pragma once

#include <iostream>
#include <vector>
#include <string>

class UserInfo {
private:
    std::string name;
    std::string id;
    Picture profile_picture;
public:
    UserInfo() = default;

    ~UserInfo() = default;

    UserInfo(const UserInfo &user_info);

    std::string get_name();

    std::string get_id();

    Picture get_profile_picture();

    void set_name(std::string new_name);

    void set_id(std::string id);

    void set_profile_picture(Pictrue new_picture);
};
