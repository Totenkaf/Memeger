#include <iostream>
#include <stdio.h>
#include <gtk/gtk.h>

#include "../include/store.h"
#include "../include/login-signup.h"

#include "./http.client.cpp"

void createLoginBox();
void createSignupBox();
void switchBoxes(GtkWidget* box_to_remove, GtkWidget*  box_to_display);

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

void switchBoxes(GtkWidget* box_to_remove, GtkWidget*  box_to_display) {
  gtk_container_remove(GTK_CONTAINER(login_signup_window), box_to_remove);
  gtk_container_add(GTK_CONTAINER(login_signup_window), box_to_display);
  gtk_widget_show_all (login_signup_window);
}

void signup_submit(GtkWidget* widget, GdkEvent  *event, gpointer data) {
  const char* nick = gtk_entry_get_text(GTK_ENTRY(signup_nickname_box));
  const char* password = gtk_entry_get_text(GTK_ENTRY(signup_password_box));

  if (strlen(nick) == 0)
  {
    std::cout << "enter nick\n" << std::endl;
    return;
  }

  // message must not be empty
  if (strlen(password) == 0)
  {
      std::cout << "enter password" << std::endl;
      return;
  }

  net::io_context io_context;
  std::make_shared<Client>(io_context)->signup(nick, password);
  // std::make_shared<Client>(io_context)->run();

  createLoginBox();
  switchBoxes(signup_box, login_box);
}

void onCreateAccountButtonClick(GtkWidget* widget, GdkEvent  *event, gpointer data) {
  createSignupBox();
  switchBoxes(login_box, signup_box);
  // gtk_widget_show_all (login_signup_window);
}

void createSignupBox() {
  signup_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  signup_nickname_box = gtk_entry_new();
  gtk_widget_set_size_request(signup_nickname_box, 200, -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(signup_nickname_box), "nickname");
  gtk_box_pack_start(GTK_BOX(signup_box), signup_nickname_box, FALSE, TRUE, 0);

  signup_password_box = gtk_entry_new();
  gtk_widget_set_size_request(signup_password_box, 200, -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(signup_password_box), "password");
  gtk_box_pack_start(GTK_BOX(signup_box), signup_password_box, FALSE, TRUE, 0);

  signup_submit_button = gtk_button_new_with_label("signup");
	gtk_box_pack_start(GTK_BOX(signup_box), signup_submit_button, FALSE, TRUE, 0);
  g_signal_connect(signup_submit_button, "clicked", G_CALLBACK(signup_submit), NULL);
}

void createLoginBox() {
  void submit(GtkWidget* widget, GdkEvent  *event, gpointer data);

  login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  nick_name_box = gtk_entry_new();
  gtk_widget_set_size_request(nick_name_box, 200, -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(nick_name_box), "nickname");
  gtk_box_pack_start(GTK_BOX(login_box), nick_name_box, FALSE, TRUE, 0);

  password_box = gtk_entry_new();
  gtk_widget_set_size_request(password_box, 200, -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(password_box), "password");
  gtk_box_pack_start(GTK_BOX(login_box), password_box, FALSE, TRUE, 0);

  submit_button = gtk_button_new_with_label("login");
	gtk_box_pack_start(GTK_BOX(login_box), submit_button, FALSE, TRUE, 0);
  g_signal_connect(submit_button, "clicked", G_CALLBACK(submit), NULL);

  create_account_button = gtk_button_new_with_label("create new account");
	gtk_box_pack_start(GTK_BOX(login_box), create_account_button, FALSE, TRUE, 0);
  g_signal_connect(create_account_button, "clicked", G_CALLBACK(onCreateAccountButtonClick), NULL);
}

void submit(GtkWidget* widget, GdkEvent  *event, gpointer data)
{
  const char* nick = gtk_entry_get_text(GTK_ENTRY(nick_name_box));
  const char* password = gtk_entry_get_text(GTK_ENTRY(password_box));

  if (strlen(nick) == 0)
  {
    std::cout << "enter nick\n" << std::endl;
    return;
  }

  // message must not be empty
  if (strlen(password) == 0)
  {
      std::cout << "enter password" << std::endl;
      return;
  }

  std::cout << "nick " << nick << std::endl;
  std::cout << "password " << password << std::endl;

  switchBoxes(login_box, signup_box);
}

void activate_login_signup (GtkApplication *app, gpointer user_data)
{
  login_signup_window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (login_signup_window), "Chat");
  gtk_window_set_default_size (GTK_WINDOW (login_signup_window), 600, 800);

  // signup_box
  createLoginBox();
  // login box

  gtk_container_add(GTK_CONTAINER(login_signup_window), login_box);

  gtk_widget_grab_focus(login_signup_window);
  gtk_widget_show_all (login_signup_window);
}