#pragma once
#include <gtk/gtk.h>

void submit_search();
void activate_search(GtkApplication *app);

GtkWidget *window_;
GtkWidget *query_box;
GtkWidget *search_box;
GtkWidget *search_button;
