//Copyright 2022 by Artem Ustsov
#pragma once
#include "idatabase.h"
#include "models.h"

class Postgre_DB : public IDataBase {
    private:
        std::shared_ptr <pqxx::connection> PG_conn;
        /*добавить константность и ссылки на векторы*/
        virtual int save(const std::string& table, const std::vector<std::string>& table_fields, 
                         const std::vector<std::string>& values, std::vector<std::string>& output_params, 
                         std::string where) override;
        virtual int update(const std::string& table, const std::vector<std::string>& table_fields, 
                           const std::vector<std::string>& values, std::string where) override;
        virtual int insert(const std::string& table, const std::vector<std::string>& table_fields, 
                           const std::vector<std::string>& values, std::vector<std::string>& output_params) override;
        virtual int delete_(const std::string& table, std::string where) override;
        virtual pqxx::result select(const std::string& table, std::string where, std::vector<std::string> what, int limit) override;

        int init_tables();

        std::string parse_table_fields(const std::vector<std::string>& fields); /* по идее, константные */
        std::string parse_table_values(const std::vector<std::string>& values);
        std::string parse_output_params(const std::vector<std::string>& output_params);
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

        /* Проверить константность всех методов */
        int add_user(User& user);
        int change_user_login(User& user, const std::string& new_login);
        int change_user_password(User& user, const std::string& new_password);
        int change_user_status(User& user, const std::string& new_status);
        int delete_user(User& user);
        bool find_user_by_login(const std::string& login);  /* по идее, константный */
        User get_user_by_login(const std::string& login); /* по идее, константный */
        std::string get_user_id(const std::string& login); /* по идее, константный */
        std::string get_user_login(const std::string& id); /* по идее, константный */

        int add_message(TextMessage& message);
        int delete_message(TextMessage& message);
        int change_read_status(TextMessage& message, bool new_status); /*is it need, how to process read status to message*/
        std::vector<TextMessage> get_last_N_messages_from_chat(const Chat& chat, int num_of_messages); /* по идее, константный */

        int add_chat(Chat& chat);
        int add_new_participant(const User& user, const Chat& chat);
        int change_chat_name(Chat& chat, const std::string& new_chat_name);
        int delete_chat(Chat& chat);
        bool find_chat_by_chat_name(const std::string& chat_name); /* по идее, константный */
        Chat get_chat_by_chat_name(const std::string& chat_name); /* по идее, константный */
        Chat get_chat_by_id(const std::string& chat_id); /* по идее, константный */
        std::vector<std::string> get_participants_from_chat(const Chat& chat); /* по идее, константный */
        std::vector<Chat> get_all_chats_by_user_login(const std::string& login); /* по идее, константный */
        
        int add_user_chat_link(const std::string& user_id, const std::string& chat_id); /*Возможно добавить в приватную секцию. Поменять id на конснтатные объекты*/
};
