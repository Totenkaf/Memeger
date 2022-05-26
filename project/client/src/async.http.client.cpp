#include <iostream>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include "async.http.client.h"

using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>

const std::string server = "localhost";
const std::string port = "8080";

Client::Client(boost::asio::io_context& io_context)
: resolver_(net::make_strand(io_context)), stream_(net::make_strand(io_context))
{}


void Client::signup(const std::string user_login, const std::string user_password) {

        std::cout << "nick is " << user_login << std::endl;
        std::cout << "password is " << user_password << std::endl;

        std::string path = "/registration/";
        std::string body = "{\"login\":\"" + user_login + "\"," + "\"password\":\"" + user_password +  "\"}";
        request_.version(10);
        request_.method(http::verb::post);
        request_.target(path);
        request_.set(http::field::content_length, std::to_string(body.length()));
        request_.set(http::field::host, server);
        request_.set(http::field::accept, "application/json");
        request_.set(http::field::connection, "Keep-Alive");
        request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        request_.body() = body;

        this->run(path);
}


// void Client::run(const std::string& server, const std::string& port, const std::string& path){
void Client::run(const std::string path){

    //add_message 

//  std::string user_login="Lol";
// std::string chat_name="Chat";
//  std::string content="1123dede";
    
// std::string body = "{\"login\":\"" + user_login + "\"," + "\"chat_name\":\"" + chat_name + "\"," + "\"content\":\"" +content+"\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

//delete message 


// std::string user_login="Jack";
// std::string chat_name="Monolog";
// std::string content="Hello my friends! How are u ";
    
// std::string body = "{\"login\":\"" + user_login + "\"," + "\"chat_name\":\"" + chat_name + "\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;


//get_last_N_messages_from_chat

// std::string chat_name="Chat";//
// std::string num_of_messages="2";

    
// std::string body = "{\"chat_name\":\"" + chat_name + "\"," + "\"num_of_messages\":\"" + num_of_messages + "\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

    //add_user

// std::string user_login="Lol2";
// std::string user_password="ewe";

    
// std::string body = "{\"login\":\"" + user_login + "\"," + "\"password\":\"" + user_password +  "\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

//check login

// std::string user_login="Tom";
// std::string body = "{\"login\":\"" + user_login +"\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

//get user

// std::string user_login="Tom";
// std::string body = "{\"login\":\"" + user_login +"\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

//change user login

// std::string user_login="Jack";
    
    
// std::string new_user_login="Jerry";

//  std::string status="active";
// std::string body = "{\"login\":\"" + user_login + "\","  +"\"new_log\":\"" +new_user_login+"\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

//change user password

// std::string user_login="Jerry";
    

// std::string new_user_pass="12342281337";

// std::string body = "{\"login\":\"" + user_login + "\","  +"\"new_pass\":\"" +new_user_pass+"\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;



// change user status 

// std::string user_login="Jerry";
// std::string status="inactive";
// std::string body = "{\"login\":\"" + user_login + "\","  +"\"status\":\"" +status+"\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;


//delete user
// std::string user_login="Lol";

// std::string body = "{\"login\":\"" + user_login +"\"}";    
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;


//add chat 


// std::string chat_name="Dialog";
// std::vector<std::string> participants={"Tom","Jerry"};
    
// std::string size =std::to_string(participants.size()) ;
// std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"participant_1\":\"" + participants[0] +"\"," + "\"participant_2\":\"" + participants[1]+ "\"," + "\"size\":\""+size +"\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;


//add participant in chat

//     std::string chat_name="Dialog";
//    std::string  participant={"Lol"};
    

//     std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"participant\":\"" + participant+   "\"}";
//     request_.version(10);
// 	request_.method(http::verb::post);
// 	request_.target(path);
// 	request_.set(http::field::content_length, std::to_string(body.length()));
// 	request_.set(http::field::host, "localhost");
// 	request_.set(http::field::accept, "application/json");
// 	request_.set(http::field::connection, "Keep-Alive");
// 	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// 	request_.body() = body;


//get_participants_from_chat   (DODELAT)


// std::string chat_name="Dialog";

    

// std::string body = "{\"chat_name\":\"" + chat_name+   "\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;




//get_all_chats_by_user_login

// std::string login="Lol";
// std::string body = "{\"login\":\"" + login+   "\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

    //change chat name


//     std::string chat_name="Dialog";
//    std::string  new_chat_name="Chat";
    

//     std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"new_chat_name\":\"" + new_chat_name+   "\"}";
//     request_.version(10);
// 	request_.method(http::verb::post);
// 	request_.target(path);
// 	request_.set(http::field::content_length, std::to_string(body.length()));
// 	request_.set(http::field::host, "localhost");
// 	request_.set(http::field::accept, "application/json");
// 	request_.set(http::field::connection, "Keep-Alive");
// 	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// 	request_.body() = body;


//del chat

// std::string chat_name="Chat# 1";

    

// std::string body = "{\"chat_name\":\"" + chat_name+  "\"}";
// request_.version(10);
// request_.method(http::verb::post);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;


// find chat by name

// std::string chat_name="Chat";
// std::string body = "{\"chat_name\":\"" + chat_name+  "\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

//get chat by name


// std::string chat_name="Chat";
// std::string body = "{\"chat_name\":\"" + chat_name+  "\"}";
// request_.version(10);
// request_.method(http::verb::get);
// request_.target(path);
// request_.set(http::field::content_length, std::to_string(body.length()));
// request_.set(http::field::host, "localhost");
// request_.set(http::field::accept, "application/json");
// request_.set(http::field::connection, "Keep-Alive");
// request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
// request_.body() = body;

    resolver_.async_resolve(server, port,
                            beast::bind_front_handler(
                                    &Client::handle_resolve,
                                    shared_from_this()));

        
}


void Client:: handle_resolve(beast::error_code err,
                    tcp::resolver::results_type results)
{
    if (!err)
    {
        stream_.async_connect(
                results,
                beast::bind_front_handler(
                        &Client::handle_connect,
                        shared_from_this()));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void Client:: handle_connect(beast::error_code err, tcp::resolver::results_type::endpoint_type)
{
    if (!err)
    {
        http::async_write(stream_, request_,
                            beast::bind_front_handler(
                                    &Client::handle_write_request,
                                    shared_from_this()));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void Client:: handle_write_request(beast::error_code err,
                            std::size_t bytes_transferred)
{
    if (!err)
    {
        http::async_read(stream_, buffer_, response_,
                            beast::bind_front_handler(
                                    &Client::handle_read,
                                    shared_from_this()));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void Client:: handle_read(beast::error_code err,
                    std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (!err)
    {
        // Write the message to standard out
        std::cout << response_ << std::endl;

        // Gracefully close the socket
        stream_.socket().shutdown(tcp::socket::shutdown_both, err);

        // not_connected happens sometimes so don't bother reporting it.
        if(err && err != beast::errc::not_connected)
            std::cout << err <<  "shutdown";
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

// int main(int argc, char* argv[])
// {

//     try
//     {
//         if (argc != 4)
//         {
//             std::cout << "Usage: async_client <server> <port> <path>\n";
//             std::cout << "Example:\n";
//             std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
//             return 1;
//         }

//         net::io_context io_context;

//         std::make_shared<Client>(io_context)->run(argv[1], argv[2], argv[3]);

//         io_context.run();
//     }
//     catch (std::exception& e)
//     {
//         std::cout << "Exception: " << e.what() << "\n";
//     }

//     return 0;
// }