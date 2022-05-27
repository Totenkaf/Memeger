#pragma once

#include "models.h"
#include "request.h"
#include "response.h"
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
#include <string>
#include <thread>
#include <vector>

namespace server3 {

class Connection : public std::enable_shared_from_this<Connection>,
                   Request,
                   Response {
public:
  /// Создаём соединение с заданным io_context.
  explicit Connection(tcp::socket &&socket);

  /// Запуск первой асинхронной операции для подключения.
  void start();

private:
  void do_read();

  /// Обработка завершения операции чтения.
  void handle_read(beast::error_code e, std::size_t bytes_transferred);

  /// Обработка завершения операции записи.
  void handle_write(bool close, beast::error_code e,
                    std::size_t bytes_transferred);

  void do_close();

private:
  /// Обработчик, используемый для обработки входящего запроса.
  // Router<Response(*)(const Request &request)> &requestRouter_;

  beast::tcp_stream stream_;
  beast::flat_buffer buffer_;

  // http::request<http::string_body> request_;
  // std::shared_ptr<void> res_;
};
} // namespace server3
