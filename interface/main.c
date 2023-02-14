#include <gtk/gtk.h>

void load(GtkWidget *widget, gpointer data)
{
    GtkEntryBuffer *buf = gtk_entry_get_buffer((GtkEntry*) data);
    GtkWidget *image = gtk_image_new_from_file((const gchar*) buf);
    g_signal_connect(image, "clicked", G_CALLBACK(gtk_widget_destroy), NULL);
    gtk_widget_show(image);
}

int main (int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *window;
    GObject *button;
    GObject *entry;
    GError *error = NULL;

    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    button = gtk_builder_get_object (builder, "quit");
    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

    entry = gtk_builder_get_object(builder, "entry");
    g_signal_connect(entry, "activate", G_CALLBACK(load), entry);
    

    gtk_main ();

    return 0;
}
