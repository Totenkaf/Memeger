#include <iostream>
#include "search.h"

void submit_search() {
    const char *query = gtk_entry_get_text(GTK_ENTRY(query_box));
    std::cout << "Query: " << query << std::endl;
    gtk_widget_show_all(window_);
    return;
}

void activate_search(GtkApplication *app) {
    window_ = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window_), "Search");
    gtk_window_set_default_size(GTK_WINDOW(window_), 600, 800);

    search_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    query_box = gtk_entry_new();
    gtk_widget_set_size_request(query_box, 200, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(query_box), "Search");
    gtk_box_pack_start(GTK_BOX(search_box), query_box, FALSE, TRUE, 0);

    search_button = gtk_button_new_with_label("Search");
    gtk_box_pack_start(GTK_BOX(search_box), search_button, FALSE, TRUE, 0);
    g_signal_connect(search_button, "clicked", G_CALLBACK(submit_search), NULL);

    gtk_container_add(GTK_CONTAINER(window_), search_box);

    gtk_widget_grab_focus(window_);
    gtk_widget_show_all(window_);
    return;
}

// int main(int argc, char **argv) {
//     GtkApplication *app;
//     int status;

//     app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
//     g_signal_connect(app, "activate", G_CALLBACK(activate_search), NULL);
//     status = g_application_run(G_APPLICATION(app), argc, argv);
//     g_object_unref(app);

//     return status;
// }