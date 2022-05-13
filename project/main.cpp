#include <iostream>
#include <stdio.h>
#include "ui/include/chat.h"
#include "ui/include/search.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    int switch = 1;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    if (switch == 1)
        g_signal_connect(app, "activate", G_CALLBACK(activate_chat), NULL);
    else
        g_signal_connect(app, "activate", G_CALLBACK(activate_search), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}