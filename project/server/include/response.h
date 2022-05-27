#pragma once

#include "models.h"
#include "postgresql_db.h"
#include "request.h"
#include <algorithm>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

namespace server3 {

class Response;
struct USER {

  void Send_ResponseUSER(Response &resp, std::string url, std::string status);
  bool check_login(Request &req, Response &resp, Postgre_DB &database);
  bool get_user(Request &req, Response &resp, Postgre_DB &database);
  bool registration(Request &req, Response &resp, Postgre_DB &database);
  bool change_login(Request &req, Response &resp, Postgre_DB &database);
  bool change_password(Request &req, Response &resp, Postgre_DB &database);
  bool change_user_status(Request &req, Response &resp, Postgre_DB &database);
  bool delete_user(Request &req, Response &resp, Postgre_DB &database);
};
struct CHAT {
  // void Not_FoundCHAT(Response &resp);
  void Send_ResponseCHAT(Response &resp, std::string url, std::string status);
  bool get_chat_by_name(Request &req, Response &resp, Postgre_DB &database);
  bool get_participants_from_chat(Request &req, Response &resp,
                                  Postgre_DB &database);
  bool get_all_chats_by_login(Request &req, Response &resp,
                              Postgre_DB &database);
  bool find_chat(Request &req, Response &resp, Postgre_DB &database);
  bool add_chat(Request &req, Response &resp, Postgre_DB &database);
  bool add_new_participant(Request &req, Response &resp, Postgre_DB &database);
  bool change_chat_name(Request &req, Response &resp, Postgre_DB &database);
  bool delete_chat(Request &req, Response &resp, Postgre_DB &database);

private:
  std::vector<std::string> participants;
  std::vector<Chat> chats;
};
// struct CHAT{};
struct MESSAGE {
  // void Not_FoundMESSAGE(Response &resp);
  void Send_ResponseMESSAGE(Response &resp, std::string url,
                            std::string status);
  bool get_last_messages(Request &req, Response &resp, Postgre_DB &database);
  bool add_message(Request &req, Response &resp, Postgre_DB &database);
  bool delete_message(Request &req, Response &resp, Postgre_DB &database);

private:
  std::vector<TextMessage> messages;
};

class Response {

public:
  json j_response;
  std::string response_body;
  http::response<http::string_body> res;
  std::shared_ptr<void> res_;
  USER usr;
  CHAT cht;
  MESSAGE msg;
  void Send_Response(Response &resp, std::string url, std::string status);
  void Not_Found(Response &resp);
};
} // namespace server3
