#include <boost/bind/bind.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>

#include "client/include/async.http.client.h"
#include "ui/include/chat.h"
#include "ui/include/search.h"
#include "ui/include/login-signup.h"
#include "ui/include/store.h"

using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>

int main(int argc, char* argv[]) {
    try {
        // if (argc != 4) {
        //     std::cerr << "Usage: async_client <server> <port> <path>\n";
        //     std::cerr << "Example:\n";
        //     std::cerr << "async_client www.boost.org /LICENSE_1_0.txt\n";
        //     return 1;
        // }

        net::io_context io_context;
        std::make_shared<Client>(io_context)->run(argv[1], argv[2], argv[3]);
        std::cout << "CLIENT IS RUNNING ..." << std::endl;
        io_context.run();
        
        // GtkApplication *app;
        // int widget_type = 2;

        // app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        // if (widget_type == 1) {
        //     g_signal_connect(app, "activate", G_CALLBACK(activate_chat), nullptr);
        // }
        // else if (widget_type == 2) {
        //     g_signal_connect(app, "activate", G_CALLBACK(activate_login_signup), nullptr);
        // }
        // else {
        //     g_signal_connect(app, "activate", G_CALLBACK(activate_search), nullptr);
        // }
        // g_application_run(G_APPLICATION(app), argc, argv);
        // g_object_unref(app);
    }

    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}