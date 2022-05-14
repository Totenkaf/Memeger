#include <iostream>
#include <gtk/gtk.h>

#include "ui/include/chat.h"
#include "ui/include/search.h"
#include "ui/include/login-signup.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    int switch_ = 2;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    if (switch_ == 1)
        g_signal_connect(app, "activate", G_CALLBACK(activate_chat), NULL);
    else if (switch_ == 2)
        g_signal_connect(app, "activate", G_CALLBACK(activate_login_signup), NULL);
    else
        g_signal_connect(app, "activate", G_CALLBACK(activate_search), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}