
#include <iostream>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>


using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>


class Client : public std::enable_shared_from_this<Client>
{
public:
    Client(boost::asio::io_context& io_context)
            : resolver_(net::make_strand(io_context)),
              stream_(net::make_strand(io_context))
    {
    }

    void run(const std::string& server, const std::string& port,const  std::string& path)
    {

        //add_message 

    //  std::string user_login="TOM";
    // std::string chat_name="Monolog";
    //  std::string content="Privet";
     
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


    // std::string user_login="TOM";
    // std::string chat_name="Monolog";
    // std::string content="Privet";
     
	// std::string body = "{\"login\":\"" + user_login + "\"," + "\"chat_name\":\"" + chat_name + "\"," + "\"content\":\"" +content+ "\"}";
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

    // std::string chat_id="50b52487-684a-4ae1-8e7a-39342144f794";
    // std::string num_of_messages="2";
    
     
	// std::string body = "{\"chat_id\":\"" + chat_id + "\"," + "\"num_of_messages\":\"" + num_of_messages + "\"}";
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

    // std::string user_login="Jerry";
    // std::string user_password="rwfewe";
    
     
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

    // std::string user_login="Tom";
     
     
    // std::string new_user_login="TOM";

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

    // std::string user_login="Jack";
    // std::string user_password="321";
    
    // std::string new_user_pass="123";
    
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

    // std::string user_login="Jack";
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
    // std::string user_login="Jerry";
    
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

   
    // std::string chat_name="ULTRA";
    // std::vector<std::string> participants(2);//укажите количество динамически
    // participants[0]="TOM";
    //  participants[1]="Jack"; 
    //  int size=participants.size();
    
    // std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"participant_1\":\"" + participants[0] + "\"," + "\"participant_2\":\"" + participants[1] + "\"," + "\"size\":\"" + std::to_string(size) + "\"}";
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

//     std::string chat_name="Monolog";
//    std::string  participant={"Jerry"};
      
    
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


//get_participants_from_chat


    // std::string chat_name="Monolog";
   
      
    
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

    // std::string login="Jack";
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


//     std::string chat_name="ULTRA";
//    std::string  new_chat_name={"Ultra"};
      
    
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

    // std::string chat_name="Ultra";
   
      
    
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


    

    //get chat by name and find_chat


    // std::string chat_name="Monolog";
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

private:
    void handle_resolve(beast::error_code err,
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

    void handle_connect(beast::error_code err, tcp::resolver::results_type::endpoint_type)
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

    void handle_write_request(beast::error_code err,
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

    void handle_read(beast::error_code err,
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

private:
    tcp::resolver resolver_;

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)

    http::request<http::string_body> request_;
    http::response<http::string_body> response_;
};

int main(int argc, char* argv[])
{
    
    try
    {
        if (argc != 4)
        {
            std::cout << "Usage: async_client <server> <port> <path>\n";
            std::cout << "Example:\n";
            std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
            return 1;
        }

        net::io_context io_context;

        std::make_shared<Client>(io_context)->run(argv[1], argv[2], argv[3]);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}