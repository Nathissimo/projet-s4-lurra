#include <gtk/gtk.h>

void choose(GtkWidget *widget, gpointer data)
{
    GtkContainer* display = (GtkContainer*) data;
    gtk_widget_set_visible((GtkWidget*)display, 1);
    gtk_widget_hide(widget);

    GList* l;
    l = g_list_first(gtk_container_get_children(display));
    gtk_image_set_from_file((GtkImage*) l->data, 
            (const gchar*)gtk_file_chooser_get_filename((GtkFileChooser*)widget));
}

void convenience(GtkWidget* Albert, gpointer data)
{
    GtkWidget* but = (GtkWidget*) data;
    gtk_widget_set_visible(but, 1);
}
    

void appear(GtkWidget *oui, gpointer data)
{
    GtkWidget* fc = (GtkWidget*) data;
    gtk_widget_set_visible(fc, 1);
} 

void goway(GtkWidget *button, gpointer pic)
{
    GtkWidget* cip = (GtkWidget*) pic;
    gtk_widget_set_visible(cip, 0);
    gtk_widget_set_visible(button, 0);

}


int main (int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *window;
    GObject *quit;
    GObject *entry;
    GObject *button2;
    GObject *image;
    GObject *fc;
    GObject* boit;
    GObject* display;
    GError *error = NULL;

    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "solve.glade", &error) == 0)
    {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

    image = gtk_builder_get_object(builder, "image");
    display = gtk_builder_get_object(builder, "display");
    button2 = gtk_builder_get_object(builder, "Bchoose_image");
     
    fc = gtk_builder_get_object(builder, "Fchooser");
    g_signal_connect(fc, "file-activated", G_CALLBACK(choose), display);
    g_signal_connect(fc, "file-activated", G_CALLBACK(convenience), button2);

    window = gtk_builder_get_object (builder, "org.gtk.inter");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    quit = gtk_builder_get_object(builder, "Bquit");
    g_signal_connect(quit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    boit = gtk_builder_get_object(builder, "boit");
    

    g_signal_connect(button2, "clicked", G_CALLBACK(appear), fc);
    g_signal_connect(button2, "clicked", G_CALLBACK(goway), display);
    

    gtk_main ();

    return 0;
}
