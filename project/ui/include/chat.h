#pragma once
#include <gtk/gtk.h>
#include <gtk/gtktypes.h>

void submit_chat();
void activate_chat(GtkApplication *app);
GtkWidget* create_row(const gchar* text);

GtkWidget *window;

GtkWidget *message_box;
GtkWidget *chat_box;
GtkWidget *user_window;
GtkWidget *chats_list;
GtkWidget *send_button;
GtkWidget *sent_message_box;
