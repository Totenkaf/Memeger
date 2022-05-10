#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  

namespace server3
{
    class Response
    {
        http::response<http::string_body> res{http::status::bad_request, request_.version()};

        std::string response_body;

        json j_response; 
        json parse();
public:
 int get_user_id(std::string)  

int change_user_login() 

int change_user_password() 

vector<std::string>get_all_chats_by_login(std::string) 

int add_chat()  

 int del_chat() 
    };

}