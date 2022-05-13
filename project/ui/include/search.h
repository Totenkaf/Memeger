#pragma once

#include <iostream>
#include <gtk/gtk.h>

GtkWidget *window;
void submit_search();
static void activate_search(GtkApplication *app)