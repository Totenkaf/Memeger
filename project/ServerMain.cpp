#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server/include/async.http.server.h"
#include "server/include/connection.h"

int main(int argc, char* argv[]) {
    try {
        // Проверка аргументов командной строки
        if (argc != 4) {
            std::cerr << "Usage: http_server <address> <port> <threads>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 1 \n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 1 \n";
            return 1;
        }

        // Инициализация сервера
        std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);
        auto s = std::make_shared<server3::server>(argv[1], argv[2], num_threads);

        // Запуск сервера до отмены
        std::cout << "SERVER IS RUNNING ..." << std::endl;
        s->run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
