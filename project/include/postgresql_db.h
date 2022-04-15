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
        
        int user_id(std::string login);
        std::string user_login(int id);

        int user_register(std::string login, std::string password); 
        int save_user(USERS_INFO user_info);
        int user_exist(std::string login, std::string password = "");

        int set_mark(std::string login_marker, std::string login_marked, int mark);
        std::vector <std::string> pairs_login(std::string login);
        int gender_is_different(int id1, int id2);
        int is_pair(int id1, int id2);
        int seen(int id1, int id2);
        int user_deleted(int id);


        int save_image(std::string path_to_file, int user_id, std::string name = "");
        std::vector <std::string> user_image(int user_id, std::string image_name = "");


        int delete_image(int user_id, std::string image_name = "");
        int delete_user(std::string login);
};


/*

        User* searchUserLogin(std::string login, std::string password) override;
        void writeUser(const User& user) override;
        void changePassword(const User& user) override;

        std::string getCodeFromMessage(std::string messageId) override;
        void writeMessage(Message& message) override; // проставить айдишник если не проставлен
        std::vector<Message> getNLastMessagesFromDialogue(std::string dialogueId, long count) override;
        DialogueList getLastNDialoguesWithLastMessage(User user, long count) override;
        std::vector<std::string> getParticipantsLoginsFromDialogue(std::string dialogueId) override;

        std::vector<std::string> getAllDialoguesIdByLogin(std::string login) override;
        std::vector<std::string> getLastNDialoguesIdByLogin(std::string login, long count) override;
        Dialogue createDialogue(std::string firstId, std::string secondId) override;

        void deleteMessage(Message& message) override;
        void deleteDialogue(Dialogue& dialogue) override;

        void connectToDb(const char* contactPoints);
        void disconnectFromDb();

        void migrate();

*/