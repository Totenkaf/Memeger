//Copyright 2022 by Artem Ustsov

#include "idatabase.h"
#include "models.h"

#include "dirent.h"
#include <fstream>
#include <memory>
#include <list>

class Postgre_DB : public IDataBase {
    private:
        std::shared_ptr <connection> PG_conn;
        
        virtual int save(const std::string & table, std::vector <std::string> values, std::string where = "") override;
        virtual int update(const std::string & table, std::vector <std::string> values, std::string where = "") override;
        virtual int insert(const std::string & table, std::vector <std::string> values) override;
        virtual int delete_(const std::string & table, std::string where = "") override;
        virtual result select(const std::string table, std::string where = "", std::vector <std::string> what = std::vector <std::string>()) override;
        int init_tables();
        
    public:
        Postgre_DB(std::string host, std::string port);
        ~Postgre_DB();

        int add_user(const User& user);
        User get_user_by_login(std::string login) const;
        bool find_user_by_login(std::string login) const;
        int change_user_login(User& user, string new_login);
        int change_user_password(User& user, string new_password);
        int save_user(User& user);
        int delete_user(User& user);



        int add_message(TextMessage& message);
        int save_message(TextMessage& message);
        std::vector<std::string> get_last_N_messages(TextMessage& message) const;
        int delete_message(TextMessage& message);



        int add_chat(Chat& chat);
        std::string find_chat_by_participants(Chat& chat);
        std::vector<std::string> get_participants_from_chat(const Chat& chat) const;
        std::vector<Chat> get_all_chats_by_login(std::string login) const;
};
