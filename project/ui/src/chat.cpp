#include <iostream>
// #include <string>
#include <gtk/gtk.h>

#include "../include/chat.h"

GtkWidget *window;

GtkWidget *message_box;
GtkWidget *chat_box;
GtkWidget *user_window;
GtkWidget *chats_list;
GtkWidget *send_button;
GtkWidget *sent_message_box;

struct Chat {
    int uid;
    const gchar* name;
};

Chat chats_record[5] = {
    {1, "first chat"}, {2, "second chat"}, {3, "third chat"}, {4, "fourth chat"}, {5, "fiveth chat"}
};

void submit_chat() {
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_box));
    sent_message_box = gtk_label_new(message);
    gtk_box_pack_start(GTK_BOX(chat_box), sent_message_box, FALSE, TRUE, 0);
    std::cout << "Message: " << message << std::endl;

    gtk_widget_show_all(window);
    return;
}

GtkWidget * create_row(const gchar* text)
{
    GtkWidget *row, *box, *label;

    row = gtk_list_box_row_new ();
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
    label = gtk_label_new (text);
    gtk_container_add (GTK_CONTAINER (row), box);
    gtk_container_add (GTK_CONTAINER (box), label);

    return row;
}

// extern void activate_chat(GtkApplication *app) {
void activate_chat(GtkApplication *app) {
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Chat");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 800);

    user_window = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    chats_list = gtk_list_box_new();

    for (int i = 0; i < 5; i++)
    {
        GtkWidget *row = create_row(chats_record[i].name);
        gtk_list_box_insert(GTK_LIST_BOX(chats_list), row, -1);
    }

    message_box = gtk_entry_new();
    gtk_widget_set_size_request(message_box, 200, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(message_box), "Message");
    gtk_box_pack_end(GTK_BOX(chat_box), message_box, FALSE, TRUE, 0);

    send_button = gtk_button_new_with_label("Send");
    gtk_box_pack_end(GTK_BOX(chat_box), send_button, FALSE, TRUE, 0);
    g_signal_connect(send_button, "clicked", G_CALLBACK(submit_chat), NULL);

    sent_message_box = gtk_label_new("Messages:");
    gtk_box_pack_start(GTK_BOX(chat_box), sent_message_box, FALSE, TRUE, 0);

    gtk_box_pack_end(GTK_BOX(user_window), chat_box, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(user_window), chats_list, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), user_window);

    gtk_widget_grab_focus(window);
    gtk_widget_show_all(window);
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