#include "connection.h"
 
#include"../DB/include/models.h"
#include"../DB/include/postgresql_db.h"
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace server3 {

        Connection::Connection(tcp::socket&& socket)
                : stream_(std::move(socket))
        {
        }


        void Connection::start()
        {
            net::dispatch(stream_.get_executor(),
                          beast::bind_front_handler(&Connection::do_read,
                                             shared_from_this()));

        }

        void Connection::do_read()
        {
            request_ = {};

            // Read a request
            http::async_read(stream_, buffer_, request_,
                             beast::bind_front_handler(
                                     &Connection::handle_read,
                                     shared_from_this()));
        }
        
    

        void Connection::handle_read(beast::error_code e,
                                     std::size_t bytes_transferred)
        {
            if( e == http::error::end_of_stream)
                
                return do_close();
                
            if (!e)
            {
                 
                http::response<http::string_body> res{http::status::bad_request, request_.version()};
                std::string request_str = request_.target().to_string();
                std::string response_body;
                 Postgre_DB database;
                // database.drop_tables();
                //database.init_tables();
                
                
                std::cout << "request:\n" << request_ << std::endl << std::endl;

               
                if (request_.method() == http::verb::get)
                {
                    if(request_str.find("/check_login/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        bool check = database.find_user_by_login(j_req["login"]);
 
                        json j_response;
                         
                        j_response["check_login"] = check;

                        if(!check)
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }
                        
                        else
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/user/" + std::to_string(check));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                        }
                         
                    }
                   else if(request_str.find("/get_user/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        User usr = database.get_user_by_login(j_req["login"]);
 
                        json j_response;
                         
                        j_response["id"] = usr.get_id();
                        j_response["login"] = usr.get_login();
                        j_response["password"] = usr.get_password();
                        j_response["status"] = usr.get_active_status();

                        if(usr.get_login()== std::string())
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }

                        else
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/user/" + usr.get_active_status());
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                        }
                        
                    }

                    else if(request_str.find("/get_chat_by_name/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        
                        Chat chat = database.get_chat_by_chat_name(j_req["chat_name"]);
                        
                        json j_response;
                        
                        j_response["chat_id_"] = chat.get_chat_id();
                        j_response["chat_name"] = chat.get_chat_name();
                        // j_response["password"] = chat.;
                        // j_response["status"] = usr.get_active_status();
                        bool check = database.find_chat_by_chat_name(j_req["chat_name"]) ;
                        if( !database.find_chat_by_chat_name(j_req["chat_name"])   || chat.get_chat_name()== std::string()  )
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }
                        
                        else
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/chat/" + std::to_string(check));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                        }
                        
                    }
                     else if(request_str.find("/participants_from_chat/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        
                        Chat chat = database.get_chat_by_chat_name(j_req["chat_name"]);
                        std::vector<std::string> participants=chat.get_participants();
                        json j_response;
                        int size =participants.size();
                        if(size==0)
                        {
                            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }"; 
                        }
                        std::string name="paticipant #";
                        for(int i=0;i<size;++i)
                        {
                            std::string name2= name+ std::to_string(i+1);
                            j_response[name2]=participants[i];
                            
                        }
                       
                        bool check = database.find_chat_by_chat_name(j_req["chat_name"]) ;
                        if( !database.find_chat_by_chat_name(j_req["chat_name"])   || chat.get_chat_name()== std::string()  )
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }
                        
                        else
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/participants/" + std::to_string(check));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                        }
                        
                    }

                    else if(request_str.find("/all_chats_by_login/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        
                        
                        std::vector<Chat> chats=database.get_all_chats_by_user_login( j_req["login"] );
                        json j_response;
                        int size =chats.size();
                        if(size==0)
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }"; 
                        }
                        std::string name="chats: ";
                        std::string name1="participants_from_chat: "; 
                       
                        for(size_t i=0;i<size;++i)
                        {
                            std::string name2= name+ std::to_string(i+1);
                            j_response[name2]=chats[i].get_chat_name();
                            std::vector<std::string> participants=chats[i].get_participants();
                            std::string name3;
                            for (size_t j = 0; j < participants.size(); j++)
                            {
                                name3= name1+ std::to_string(j+1);
                                j_response[name3]=participants ;
                            }
                            
                            
                        }
                       
                        
                        
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/participants/" + std::to_string(0));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                         
                        
                    }

                   else  if(request_str.find("/find_chat/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        bool check  =database.find_chat_by_chat_name(j_req["chat_name"]);
                        //bool check = database.find_user_by_login(j_req["chat_name"]);
 
                        json j_response;
                         
                        j_response["check_chat"] = check;

                        if(!check)
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }
                        
                        else
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/chat/" + std::to_string(check));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                        }
                         
                    }
                    else  if(request_str.find("/get_last_messages/") != std::string::npos)
                    {
                        
                        json j_req = json::parse(request_.body());
                        
                       std::string str;
                       str= j_req["num_of_messages"];
                        int digit =boost::lexical_cast<int>( str.c_str());
                        std::vector<TextMessage> messages=database.get_last_N_messages_from_chat(j_req["chat_id"],digit);
                        json j_response;
                         
                        //j_response["check_chat"] = check;

                        if(messages.size()==0)
                        {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }
                        std::string mess="message:";
                        for (size_t i = 0; i < messages.size(); i++)
                        {
                              std::string mess2=mess +std::to_string(i+1);
                              j_response[mess2]=messages[i].get_message_text();
                        }
                        
                        
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/chat/" + std::to_string(0));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                         
                         
                    }
                    else
                    {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                }
                else if (request_.method() == http::verb::post) 
                {
                if (request_.target().find("/registration/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    User user(j_req["login"],j_req["password"]);
                    
                    int result = database.add_user(user);

                    if (result != 0) {
                        std::cerr<< "New add  user went wrong" << std::endl;
                    }
                     json j_response;
                     j_response["status"] = result;



                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/user/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else if (request_.target().find("/change_login/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    User user=database.get_user_by_login(j_req["login"]);
                     
                    
                    int result = database.change_user_login(user,j_req["new_log"]);

                    if (result != 0) {
                        //std::cerr<< "New login for  user went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;



                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/user/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else if (request_.target().find("/change_password/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    //User user(j_req["login"],j_req["password"]);
                     User user=database.get_user_by_login(j_req["login"]);
                    
                    int result = database.change_user_password(user,j_req["new_pass"]);

                    if (result != 0) {
                        //std::cerr<< "New password for  user went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;



                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/user/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else if (request_.target().find("/change_user_status/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    User user=database.get_user_by_login(j_req["login"]);
                     
                    
                    int result = database.change_user_status(user,j_req["status"]);

                    if (result != 0) {
                        //std::cerr<< "New login for  user went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;



                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/user/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
               else if (request_.target().find("/delete_user/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    User user  = database.get_user_by_login(j_req["login"]);
                     std::cout<<user.get_id()<<std::endl;
                    std::cout<<user.get_login()<<std::endl;
                    int result = database.delete_user(user);
                    
                    if (result != 0) {
                        //std::cerr<< "Delete user went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;



                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/user/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else if (request_.target().find("/add_chat/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    std::vector<std::string> participants={j_req["participant_1"],j_req["participant_2"]};
                    
                    Chat chat(j_req["chat_name"],participants);
                    int result=database.add_chat(chat);


                    if (result != 0) {
                        //std::cerr<< "New chat for  users went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;
                     
                     

                   


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/chat/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                     
                }

                else if (request_.target().find("/add_new_participant/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                  
                    Chat chat=database.get_chat_by_chat_name(j_req["chat_name"]);
                    User user= database.get_user_by_login( j_req["participant"]);
                    int result=database.add_new_participant(user,chat);


                    if (result != 0) {
                        //std::cerr<< "New chat for  users went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;
                     
                     

                   


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/chat/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                     
                }


                else if (request_.target().find("/change_chat_name/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                  
                    Chat chat=database.get_chat_by_chat_name(j_req["chat_name"]);
                   
                    int result=database.change_chat_name(chat,j_req["new_chat_name"]);


                    if (result != 0) {
                        //std::cerr<< "New chat for  users went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;
                     
                     

                   


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/chat/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                     
                }



                else if (request_.target().find("/delete_chat/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    
                    
                    Chat chat=database.get_chat_by_chat_name(j_req["chat_name"]);
                    int result=database.delete_chat(chat);


                    if (result != 0) {
                        std::cerr<< "New chat for  users went wrong" << std::endl;
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;
                     
                 


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/chat/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                     
                }
                else if (request_.target().find("/add_message/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    
                    std::string user_id=database.get_user_id(j_req["login"]);
                    Chat chat=database.get_chat_by_chat_name(j_req["chat_name"]);
                    TextMessage message= TextMessage(chat.get_chat_id(),user_id,user_id,j_req["content"]);
                    int result=database.add_message( message);


                    if (result != 0) {
                        
                        
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;
                     
                 


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/message/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                     
                }

                         else if (request_.target().find("/delete_message/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    
                    std::string user_id=database.get_user_id(j_req["login"]);
                    Chat chat=database.get_chat_by_chat_name(j_req["chat_name"]);
                    TextMessage message= TextMessage(chat.get_chat_id(),user_id,user_id,j_req["content"]);
                    int result=database.delete_message( message);


                    if (result != 0) {
                        
                        
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                     json j_response;
                     j_response["status"] = result;
                     
                 


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/message/" + std::to_string(result));
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                     
                }
                }

                else
                {

                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "application/json");
                res.set(http::field::connection, "Keep-Alive");
                res.result(http::status::not_found);
                res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                
                }

                //res.prepare_payload();

                auto sp = std::make_shared<http::message<false, http::string_body>>(std::move(res));
                res_ = sp;

                http::async_write(stream_,
                        *sp,
                        beast::bind_front_handler(
                                &Connection::handle_write,
                                shared_from_this(),
                                sp->need_eof()));
            }
        }

        void Connection::handle_write(bool close,
                                      beast::error_code e,
                                      std::size_t bytes_transferred)
        {
            if(close)
            {
                // This means we should close the connection, usually because
                // the response indicated the "Connection: close" semantic.
                return do_close();
            }

            if (!e)
            {
                do_read();
            }
        }

        void Connection::do_close()
        {
            beast::error_code ec;
            stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        }

    } // namespace server3

