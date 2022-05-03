#include "connection.h"
 
#include"project/include/models.h"
#include"project/include/postgresql_db.h"
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
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
                Postgre_DB database("127.0.0.1", "5432", "postgres", "postgres", "postgres");
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
                    User user(j_req["id"],j_req["login"],j_req["password"],j_req["status"]);
                    
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
                    User user(j_req["id"],j_req["login"],j_req["password"],j_req["status"]);
                     
                    
                    int result = database.change_user_login(user,j_req["new_log"]);

                    if (result != 0) {
                        std::cerr<< "New login for  user went wrong" << std::endl;
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
                    User user(j_req["id"],j_req["login"],j_req["password"],j_req["status"]);
                     
                    
                    int result = database.change_user_password(user,j_req["new_pass"]);

                    if (result != 0) {
                        std::cerr<< "New password for  user went wrong" << std::endl;
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
               else if (request_.target().find("/delete/") != std::string::npos)
                {
                    json j_req = json::parse(request_.body());
                    User user(j_req["id"],j_req["login"],j_req["password"],j_req["status"]);
                    
                    int result = database.delete_user(user);

                    if (result != 0) {
                        std::cerr<< "Delete user went wrong" << std::endl;
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

