TODO:
- понять как компилить gtk (желательно gtk4)

# install dependencies
- sudo apt install libgtk-3-dev

# Run command
- g++ `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0`

# GTK 3 doc
- https://docs.gtk.org/gtk3/
- https://docs.gtk.org/gtk3/getting_started.html#building-applications