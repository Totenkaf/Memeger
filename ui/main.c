#include <iostream>
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget* nick_name_box;
GtkWidget* password_box;
GtkWidget* submit_button;
GtkWidget* login_box;

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

  gtk_container_remove(GTK_CONTAINER(window), login_box);
}

static void activate (GtkApplication *app, gpointer user_data)
{
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Chat");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 800);

  login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  // gtk_box_set_center_widget(GTK_CONTAINER(window), login_box);

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

  gtk_container_add(GTK_CONTAINER(window), login_box);

  gtk_widget_grab_focus(window);
  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}