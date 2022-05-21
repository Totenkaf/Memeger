#ifndef BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H
#define BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


namespace server3 {

        class server : public std::enable_shared_from_this<server>
        {
        public:
            explicit server(const std::string& address, const std::string& port,
                            std::size_t thread_pool_size);

            /// Запуск цикла io_context сервера.
            void run();

        private:
            /// Инициация асинхронной операции приема.
            void start_accept();

            /// Обработка завершения асинхронной операции приема.
            void handle_accept(beast::error_code e, tcp::socket socket);

            /// Обработка запроса на остановку сервера.
            void handle_stop();

        private:

            /// Следующее соединение, которое должно быть принято.
           // boost::shared_ptr<Connection> new_connection_;

            /// Обработчик для всех входящих запросов.
            //Router<Response(*)(const Request &request)> request_router;

            net::io_context io_context_;
            tcp::acceptor acceptor_;

            /// Количество потоков, которые будут вызывать io_context::run().
            std::size_t thread_pool_size_;

            /// The signal_set is used to register for process termination notifications.
            //boost::asio::signal_set signals_;
        };

} // namespace server3


#endif //BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H
