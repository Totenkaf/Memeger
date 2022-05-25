#pragma once
#include <gtk/gtk.h>

void activate_login_signup(GtkApplication *app, gpointer user_data);
void submit(GtkWidget* widget, GdkEvent  *event, gpointer data);
void onCreateAccountButtonClick(GtkWidget* widget, GdkEvent  *event, gpointer data);
void signup_submit(GtkWidget* widget, GdkEvent* event, gpointer data);
void switchBoxes(GtkWidget* box_to_remove, GtkWidget*  box_to_display);
void createSignupBox();
void createLoginBox();

GtkWidget *login_signup_window;
GtkWidget* nick_name_box;
GtkWidget* password_box;
GtkWidget* submit_button;
GtkWidget* create_account_button;

GtkWidget* login_box;

GtkWidget* signup_box;
GtkWidget* signup_nickname_box;
GtkWidget* signup_password_box;
GtkWidget* signup_submit_button;
