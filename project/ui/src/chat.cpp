#include <iostream>
#include <gtk/gtk.h>

#include "chat.h"

GtkWidget *window_chat;
GtkWidget *message_box;
GtkWidget *chat_box;
GtkWidget *send_button;
GtkWidget *sent_message_box;

void submit_chat() {
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_box));
    sent_message_box = gtk_label_new(message);
    gtk_box_pack_start(GTK_BOX(chat_box), sent_message_box, FALSE, TRUE, 0);
    std::cout << "Message: " << message << std::endl;

    gtk_widget_show_all(window_chat);
    return;
}

static void activate_chat(GtkApplication *app) {
    window_chat = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window_chat), "Chat");
    gtk_window_set_default_size(GTK_WINDOW(window_chat), 600, 800);

    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    message_box = gtk_entry_new();
    gtk_widget_set_size_request(message_box, 200, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(message_box), "Message");
    gtk_box_pack_end(GTK_BOX(chat_box), message_box, FALSE, TRUE, 0);

    send_button = gtk_button_new_with_label("Send");
    gtk_box_pack_end(GTK_BOX(chat_box), send_button, FALSE, TRUE, 0);
    g_signal_connect(send_button, "clicked", G_CALLBACK(submit_chat), NULL);

    sent_message_box = gtk_label_new("Messages:");
    gtk_box_pack_start(GTK_BOX(chat_box), sent_message_box, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window_chat), chat_box);

    gtk_widget_grab_focus(window_chat);
    gtk_widget_show_all(window_chat);
    return;
}

// int main(int argc, char **argv) {
//     GtkApplication *app;
//     int status;

//     app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
//     g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//     status = g_application_run(G_APPLICATION(app), argc, argv);
//     g_object_unref(app);

//     return status;
// }