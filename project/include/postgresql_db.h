//Copyright 2022 by Artem Ustsov
#pragma once
#include "idatabase.h"
#include "models.h"

#include "dirent.h"
#include <fstream>
#include <memory>
#include <list>

class Postgre_DB : public IDataBase {
    private:
        std::shared_ptr <pqxx::connection> PG_conn;
        
        virtual int save(const std::string& table, std::vector <std::string> values, std::string where) override;
        virtual int update(const std::string& table, std::vector <std::string> values, std::string where) override;
        virtual int insert(const std::string& table, std::vector <std::string> values) override;
        virtual int delete_(const std::string& table, std::string where) override;
        virtual pqxx::result select(const std::string& table, std::string where, std::vector <std::string> what) override;

        int init_tables();
        std::string remove_danger_characters(const std::string& row_column);

        std::string db_host_;
        std::string db_port_;
        std::string db_name_;
        std::string db_user_;
        std::string db_password_;

    public:
        Postgre_DB();
        Postgre_DB(std::string db_host, std::string db_port, std::string db_name, std::string db_user, std::string db_password);
        ~Postgre_DB();
        
        int drop_tables();

        int add_user(User& user);
        User get_user_by_login(const std::string& login);
        bool find_user_by_login(const std::string& login);
        int change_user_login(User& user, const std::string& new_login);
        int change_user_password(User& user, const std::string& new_password);
        int delete_user(User& user);


        int add_message(const TextMessage& message);
        std::vector<std::string> get_last_N_messages(TextMessage& message) const;
        int delete_message(TextMessage& message);

        int add_chat(const Chat& chat);
        std::string find_chat_by_participants(const Chat& chat) const;
        std::vector<std::string> get_participants_from_chat(const Chat& chat) const;
        std::vector<Chat> get_all_chats_by_login(std::string login) const;
        int delete_chat(Chat& chat);


        int add_user_chat_link(const User& user, const Chat& chat);
};
