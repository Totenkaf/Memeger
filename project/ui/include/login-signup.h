#pragma once
#include <gtk/gtk.h>

void activate_login_signup(GtkApplication *app, gpointer user_data);
void submit(GtkWidget* widget, GdkEvent  *event, gpointer data);
void onCreateAccountButtonClick(GtkWidget* widget, GdkEvent  *event, gpointer data);
void signup_submit(GtkWidget* widget, GdkEvent* event, gpointer data);
void switchBoxes(GtkWidget* box_to_remove, GtkWidget*  box_to_display);
void createSignupBox();
void createLoginBox();

std::string nickname;

GtkWidget *login_signup_window;
GtkWidget *login_signup_main_box;

GtkWidget *app_logo_box;
GtkWidget * form_block;

GtkWidget* nick_name_box;
GtkWidget* password_box;
GtkWidget* submit_button;
GtkWidget* create_account_button;
GtkWidget* chat_window;
GtkWidget* login_box;
GtkWidget* start_chat_button;

GtkWidget* signup_box;
GtkWidget* signup_nickname_box;
GtkWidget* signup_password_box;
GtkWidget* signup_submit_button;
