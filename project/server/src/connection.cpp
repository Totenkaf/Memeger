#include "connection.h"
 
#include "models.h"
#include "postgresql_db.h"
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "response.h"
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
 
namespace server3 {

Request obj_request;
Response obj_response;


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
            http::async_read(stream_, buffer_,  obj_request.request_,
                             beast::bind_front_handler(
                                     &Connection::handle_read,
                                     shared_from_this()));
        }
        
    

        void Connection::handle_read(beast::error_code e,
                                     std::size_t bytes_transferred)
        {
            if( e == http::error::end_of_stream)
                {std::cout<<"!!!!!";
                return do_close();}
                
            if (!e)
            {
                 
                 obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                std::string &request_str=obj_request.request_str;
                boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                std::string &response_body=obj_response.response_body;
                Postgre_DB database("127.0.0.1", "5432", "postgres", "postgres", "postgres");
                // database.drop_tables();
               
                bool check;
                
                std::cout << "request:\n" << request_<< std::endl << std::endl;

               
                if (req_method(obj_request))
                {
                    if(request_str.find("/check_login/") != std::string::npos)
                    {
                        
                        
                        check = obj_response.usr.check_login(obj_request,obj_response,database);
                         
                    }
                   else if(request_str.find("/get_user/") != std::string::npos)
                    {
                        
                         check = obj_response.usr.get_user(obj_request,obj_response,database);
                        
                    }

                    else if(request_str.find("/get_chat_by_name/") != std::string::npos)
                    {
                        
                         check = obj_response.cht.get_chat_by_name(obj_request,obj_response,database);
                        
                    }
                     else if(request_str.find("/participants_from_chat/") != std::string::npos)
                    {
                         check = obj_response.cht.get_participants_from_chat(obj_request,obj_response,database);
                       
                        
                    }

                    else if(request_str.find("/all_chats_by_login/") != std::string::npos)
                    {
                         check = obj_response.cht.get_all_chats_by_login(obj_request,obj_response,database);
                       
                    }

                   else  if(request_str.find("/find_chat/") != std::string::npos)
                    {
                        
                       check = obj_response.cht.find_chat(obj_request,obj_response,database);
                         
                    }
                    else  if(request_str.find("/get_last_messages/") != std::string::npos)
                    {
                        
                        check = obj_response.msg.get_last_messages(obj_request,obj_response,database);
                    }
                }
                else if (!req_method(obj_request)) 
                {
                if (request_.target().find("/registration/") != std::string::npos)
                {
                      check = obj_response.usr.registration(obj_request,obj_response,database);
                }
                else if (request_.target().find("/change_login/") != std::string::npos)
                {
                     check = obj_response.usr.change_login(obj_request,obj_response,database);
                }
                else if (request_.target().find("/change_password/") != std::string::npos)
                {
                    check = obj_response.usr.change_password(obj_request,obj_response,database);
                }
                else if (request_.target().find("/change_user_status/") != std::string::npos)
                {
                    check = obj_response.usr.change_user_status(obj_request,obj_response,database);
                }
               else if (request_.target().find("/delete_user/") != std::string::npos)
                {
                     check = obj_response.usr.delete_user(obj_request,obj_response,database);
                }
                else if (request_.target().find("/add_chat/") != std::string::npos)
                {
                    
                      check = obj_response.cht.add_chat(obj_request,obj_response,database);
                }

                else if (request_.target().find("/add_new_participant/") != std::string::npos)
                {
                   
                     check = obj_response.cht.add_new_participant(obj_request,obj_response,database);
                }


                else if (request_.target().find("/change_chat_name/") != std::string::npos)
                {
                      check = obj_response.cht.change_chat_name(obj_request,obj_response,database);
                     
                }



                else if (request_.target().find("/delete_chat/") != std::string::npos)
                {
                   check = obj_response.cht.delete_chat(obj_request,obj_response,database);
                     
                }
                else if (request_.target().find("/add_message/") != std::string::npos)
                {
                    check = obj_response.msg.add_message(obj_request,obj_response,database);
                     
                }

                else if (request_.target().find("/delete_message/") != std::string::npos)
                {
                     check = obj_response.msg.delete_message(obj_request,obj_response,database);
                }
                }

                else
                {

                    obj_response.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    obj_response.res.set(http::field::content_type, "application/json");
                    obj_response.res.set(http::field::connection, "Keep-Alive");
                    obj_response.res.result(http::status::not_found);
                    obj_response.res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                
                }

                //res.prepare_payload();

                auto sp = std::make_shared<http::message<false, http::string_body>>(std::move(obj_response.res));
                obj_response.res_ = sp;

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

