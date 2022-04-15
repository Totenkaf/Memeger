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
    class Request
    {
        std::string request_str;
         std::string get_request_str();
         public:
        int get_user_id(std::string);

        std::string get_user_by_login(std::string);

  //   vector<std::string> get_all_chats_by_login(std::string);

       std::string add_chat() ;

        std::string    del_chat()  ;

         bool what_type_request(std::string); 
    };

}