#include <iostream>
#include <utility>
#include <thread>
#include "nlohmann/json.hpp"
#include "store.h"
#include "login-signup.h"
#include "chat.h"
#include "../../client/include/async.http.client.h"

// struct Chat_rec
// {
//   int uid;
//   const gchar *name;
// };

// Chat_rec chats_record[5] = {
//     {1, "first chat"}, {2, "second chat"}, {3, "third chat"}, {4, "fourth chat"}, {5, "fiveth chat"}};

void switch_boxes(GtkWidget *box_to_remove, GtkWidget *box_to_display)
{
  gtk_container_remove(GTK_CONTAINER(login_signup_window), box_to_remove);
  gtk_container_add(GTK_CONTAINER(login_signup_window), box_to_display);
  gtk_widget_show_all(login_signup_window);
  return;
}

void submit_start_chat() {
  std::cout << "start chat" << std::endl;
  net::io_context io_context;
  std::shared_ptr<Client> cli = std::make_shared<Client>(io_context);

  std::string user_1 = "111";
  std::string user_2 = "123";
  cli->start_chat(user_1, user_2);

  std::thread t([&io_context]()
                { io_context.run(); });

  t.join();

  auto response = cli->handle_response();
  std::cout << response.body() << std::endl;
  return;
}

void create_chat_box()
{
  void submit_chat();
  // user_window = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  // chats_list = gtk_list_box_new();

  // for (size_t i = 0; i < 5; ++i)
  // {
  //   GtkWidget *row = create_row(chats_record[i].name);
  //   gtk_list_box_insert(GTK_LIST_BOX(chats_list), row, -1);
  // }

  // start_chat_button = gtk_button_new_with_label("Start chat");
  // gtk_box_pack_end(GTK_BOX(chat_box), start_chat_button, FALSE, TRUE, 0);
  // g_signal_connect(start_chat_button, "clicked", G_CALLBACK(submit_start_chat), NULL);

  message_box = gtk_entry_new();
  gtk_widget_set_size_request(message_box, 200, -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(message_box), "Message");
  gtk_box_pack_end(GTK_BOX(chat_box), message_box, FALSE, TRUE, 0);

  send_button = gtk_button_new_with_label("Send");
  gtk_box_pack_end(GTK_BOX(chat_box), send_button, FALSE, TRUE, 0);
  g_signal_connect(send_button, "clicked", G_CALLBACK(submit_chat), NULL);

  sent_message_box = gtk_label_new("Messages:");
  gtk_box_pack_start(GTK_BOX(chat_box), sent_message_box, FALSE, TRUE, 0);

  net::io_context io_context;
  std::shared_ptr<Client> cli = std::make_shared<Client>(io_context);

  cli->get_last_messages();

  std::thread t([&io_context]()
                { io_context.run(); });

  t.join();

  auto response = cli->handle_response();
  std::cout << response.body() << std::endl;

  // std::string key = "message:1";
  // auto j = nlohmann::json::parse(response.body());
  // std::cout << j.find(key) << std::endl;
  // for (int i = 2; i <= 12; i++) {
  //   key = "message:" + std::to_string(i);
  //   std::cout << key << std::endl;
  //   //std::cout << j.at(2) << std::endl;
  // }

  // gtk_box_pack_end(GTK_BOX(chat_box), chat_box, TRUE, TRUE, 0);
  // gtk_box_pack_end(GTK_BOX(user_window), chats_list, FALSE, TRUE, 0);
}

void submit_chat()
{
  const char *message = gtk_entry_get_text(GTK_ENTRY(message_box));
  sent_message_box = gtk_label_new(message);
  gtk_box_pack_start(GTK_BOX(chat_box), sent_message_box, FALSE, TRUE, 0);
  std::cout << "Message: " << message << std::endl;
  gtk_widget_show_all(login_signup_window);
  net::io_context io_context;
  std::shared_ptr<Client> cli = std::make_shared<Client>(io_context);

  cli->send_message(nickname, message);

  std::thread t([&io_context]()
                { io_context.run(); });

  t.join();

  auto response = cli->handle_response();
  std::cout << response.body() << std::endl;
  return;
}

GtkWidget *create_row(const gchar *text)
{
  GtkWidget *row, *box, *label;
  row = gtk_list_box_row_new();
  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  label = gtk_label_new(text);
  gtk_container_add(GTK_CONTAINER(row), box);
  gtk_container_add(GTK_CONTAINER(box), label);
  return row;
}

void activate_chat(GtkApplication *app)
{
  chat_window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(chat_window), "Chat");
  gtk_window_set_default_size(GTK_WINDOW(chat_window), 600, 800);
  create_chat_box();
  gtk_container_add(GTK_CONTAINER(chat_window), chat_box);
  gtk_widget_grab_focus(chat_window);
  gtk_widget_show_all(chat_window);
  return;
}

void create_signup_box()
{
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

void on_create_account_button_click(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  create_signup_box();
  switch_boxes(login_box, signup_box);
  return;
}

void create_login_box()
{
  void submit(GtkWidget * widget, GdkEvent * event, gpointer data);

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
  g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_click), NULL);
}

void signup_submit(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  const char *nick = gtk_entry_get_text(GTK_ENTRY(signup_nickname_box));
  const char *password = gtk_entry_get_text(GTK_ENTRY(signup_password_box));

  if (strlen(nick) == 0)
  {
    std::cout << "enter nick\n"
              << std::endl;
    return;
  }

  if (strlen(password) == 0)
  {
    std::cout << "enter password" << std::endl;
    return;
  }

  net::io_context io_context;
  std::shared_ptr<Client> cli = std::make_shared<Client>(io_context);

  cli->signup(nick, password);

  std::thread t([&io_context]()
                { io_context.run(); });

  t.join();

  auto response = cli->handle_response();
  std::cout << response.body() << std::endl;

  create_login_box();
  switch_boxes(signup_box, login_box);
  return;
}

void submit(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  const char *nick = gtk_entry_get_text(GTK_ENTRY(nick_name_box));
  const char *password = gtk_entry_get_text(GTK_ENTRY(password_box));

  if (strlen(nick) == 0)
  {
    std::cout << "enter nick\n"
              << std::endl;
    return;
  }

  if (strlen(password) == 0)
  {
    std::cout << "enter password" << std::endl;
    return;
  }
  nickname = nick;
  std::cout << "nick " << nick << std::endl;
  std::cout << "password " << password << std::endl;

  net::io_context io_context;
  std::shared_ptr<Client> cli = std::make_shared<Client>(io_context);

  cli->login(nick, password);

  std::thread t([&io_context]()
                { io_context.run(); });

  t.join();

  auto response = cli->handle_response();
  std::cout << response.body() << std::endl;
  create_chat_box();
  switch_boxes(login_box, chat_box);
  return;
}

void activate_login_signup(GtkApplication *app, gpointer user_data)
{
  login_signup_window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(login_signup_window), "Login or signup");
  gtk_window_set_default_size(GTK_WINDOW(login_signup_window), 600, 800);

  create_login_box();
  gtk_container_add(GTK_CONTAINER(login_signup_window), login_box);
  gtk_widget_grab_focus(login_signup_window);
  gtk_widget_show_all(login_signup_window);
  return;
}
