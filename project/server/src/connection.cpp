#include "connection.h"

#include "models.h"
#include "postgresql_db.h"
#include "response.h"
#include <boost/beast/http/write.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace server3 {

Request obj_request;
Response obj_response;

Connection::Connection(tcp::socket &&socket) : stream_(std::move(socket)) {}

void Connection::start() {
  net::dispatch(
      stream_.get_executor(),
      beast::bind_front_handler(&Connection::do_read, shared_from_this()));
}

void Connection::do_read() {
  request_ = {};

  // Read a request
  http::async_read(
      stream_, buffer_, obj_request.request_,
      beast::bind_front_handler(&Connection::handle_read, shared_from_this()));
}
auto Connection::answer_url(const Request &obj, Postgre_DB &database)->bool
{
  bool check;
  std::vector<std::string>urls_gets={"/check_login/","/get_user/","/get_chat_by_name/","/participants_from_chat/","/all_chats_by_login/","/find_chat/","/get_last_messages/"};
  std::vector<std::string>urls_posts={"/registration/","/change_login/","/change_password/","/change_user_status/","/delete_user/","/add_chat/","/add_new_participant/","/change_chat_name/","/delete_chat/","/add_message/","/delete_message/"};
  int val=-1;
   
  if (req_method(obj)) 
  {
    for(size_t i=0;i<urls_gets.size();++i)
  {
    if(obj.request_str==urls_gets[i])
    {
      val=i;
      break;
    }
    }
    switch (val)
    {
    case 0:
    check =obj_response.usr.check_login(obj_request, obj_response, database);
      break;
    case 1:
    check = obj_response.usr.get_user(obj_request, obj_response, database);
      break;
    case 2:
    check = obj_response.cht.get_chat_by_name(obj_request, obj_response,
                                                  database);
      break;
    case 3:
     check = obj_response.cht.get_participants_from_chat(
            obj_request, obj_response, database);                                      
      break;
    case 4:
    check = obj_response.cht.get_all_chats_by_login(obj_request,
                                                        obj_response, database);
      break;
    case 5:
    check = obj_response.cht.find_chat(obj_request, obj_response, database);
      break;
    case 6:
     check = obj_response.msg.get_last_messages(obj_request, obj_response,
                                                   database);
      break;            
    
    default: check=false;
      break;
    }
  }
  else if(!req_method(obj))
  {
 for(size_t i=0;i<urls_posts.size();++i)
  {
    if(obj.request_str==urls_posts[i])
    {
      val=i;
      break;
    }
    }
    switch (val)
    {
    case 0:
    check =
            obj_response.usr.registration(obj_request, obj_response, database);
      break;
    case 1:
    check =
            obj_response.usr.change_login(obj_request, obj_response, database);
      break;
    case 2:
    check = obj_response.usr.change_password(obj_request, obj_response,
                                                 database);
      break;
    case 3:
    check = obj_response.usr.change_user_status(obj_request, obj_response,
                                                    database);
      break;
    case 4:
    check =
            obj_response.usr.delete_user(obj_request, obj_response, database);
      break;
    case 5:
    check = obj_response.cht.add_chat(obj_request, obj_response, database);
      break;
    case 6:
     check = obj_response.cht.add_new_participant(obj_request, obj_response,
                                                     database);
      break;
    case 7:
    check = obj_response.cht.change_chat_name(obj_request, obj_response,
                                                  database);
    break; 
    case 8:
     check =
            obj_response.cht.delete_chat(obj_request, obj_response, database);
    break;
    case 9:
    check =
            obj_response.msg.add_message(obj_request, obj_response, database);
    break;
    case 10:
    check =
            obj_response.msg.delete_message(obj_request, obj_response, database);
    break;             
    
    default: check=false;
      break;
    }
  }
  return check;
  
}
void Connection::handle_read(beast::error_code e,
                             std::size_t bytes_transferred) {
  if (e == http::error::end_of_stream) {
    std::cout << "!!!!!";
    return do_close();
  }

  if (!e) {

    obj_response.res = {http::status::bad_request,
                        obj_request.request_.version()};

    obj_request.request_str = obj_request.request_.target().to_string();

    std::string &request_str = obj_request.request_str;
    boost::beast::http::request<boost::beast::http::string_body> &request_ =
        obj_request.request_;
    boost::beast::http::response<boost::beast::http::string_body> &res =
        obj_response.res;
    std::string &response_body = obj_response.response_body;
    Postgre_DB database("127.0.0.1", "5432", "postgres", "postgres",
                        "postgres");
    // database.drop_tables();

    bool check=answer_url(obj_request,database);

    
     if(!check){
      obj_response.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
      obj_response.res.set(http::field::content_type, "application/json");
      obj_response.res.set(http::field::connection, "Keep-Alive");
      obj_response.res.result(http::status::not_found);
      obj_response.res.body() =
          R"({\"page\": \"Not found\", \"reason\" : \"wrong URI\" }")";
    }

    // res.prepare_payload();

    auto sp = std::make_shared<http::message<false, http::string_body>>(
        std::move(obj_response.res));
    obj_response.res_ = sp;

    http::async_write(stream_, *sp,
                      beast::bind_front_handler(&Connection::handle_write,
                                                shared_from_this(),
                                                sp->need_eof()));
  }
}

void Connection::handle_write(bool close, beast::error_code e,
                              std::size_t bytes_transferred) {
  if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Connection: close" semantic.
    return do_close();
  }

  if (!e) {
    do_read();
  }
}

void Connection::do_close() {
  beast::error_code ec;
  stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}

} // namespace server3
