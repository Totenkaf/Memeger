#pragma once 
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
class Client : public std::enable_shared_from_this<Client>{
    public:
    void   run(const std::string& server, const std::string& port,const  std::string& path);
    Client(boost::asio::io_context& io_context);
    private:
    void handle_resolve(beast::error_code err,tcp::resolver::results_type results);
    void handle_connect(beast::error_code err, tcp::resolver::results_type::endpoint_type);
    void handle_write_request(beast::error_code err,std::size_t bytes_transferred);
    void handle_read(beast::error_code err,std::size_t bytes_transferred);
    tcp::resolver resolver_;

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)

    http::request<http::string_body> request_;
    http::response<http::string_body> response_;
};
 