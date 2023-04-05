#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <sys/random.h>
#include <time.h>
#include "../traitement/k_moyen.h"


//interface
typedef struct Inter
{
    GtkWindow* window;              // Main window
    GtkButton* Bchose_image;          
    GtkButton* Bbin;       
    GtkImage* Gimage;
    int usless; //this is usless
} Inter;
//for the chose buttom
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	//active another button
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bbin),TRUE);
	//creat dialog windows
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
  	{
    		char *filename;
    		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    		filename = gtk_file_chooser_get_filename (chooser);
		k_moyen(4,filename,5,10);
		GdkPixbuf *pix = gdk_pixbuf_new_from_file(filename,NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
    		g_free (filename);
  	}

	gtk_widget_destroy (dialog);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fbin(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"new_surface.bmp",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

// Main function.                                                               
int main ()                                               
{                                                                               
     // Initializes GTK.                                                         
     gtk_init(NULL, NULL);                                                       
                                                                                 
     // Loads the UI description and builds the UI.                              
     // (Exits if an error occurs.)                                              
     GtkBuilder* builder = gtk_builder_new();                                    
     GError* error = NULL;                                                       
     if (gtk_builder_add_from_file(builder, "interfacePres/solve.glade", &error) == 0)         
     {                                                                           
         g_printerr("Error loading file: %s\n", error->message);                 
         g_clear_error(&error);                                                  
         return 1;                                                               
     }                                                                           
                                                                                 
     // Gets the widgets.                                                        
     GtkWindow* window = 
	     GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.inter"));
     GtkButton* Bchose_image = 
	     GTK_BUTTON(gtk_builder_get_object(builder, "Bchose_image"));
     
    
     GtkButton* Bbin = GTK_BUTTON(gtk_builder_get_object(builder, "Bbin"));
     
     GtkImage* Gimage = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
     
     Inter inter=
     {
	     .Bbin=Bbin,
	     .Gimage=Gimage,
	     .usless=1
     };

     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
     g_signal_connect(Bchose_image, "clicked", 
		     G_CALLBACK(on_fchose_image), &inter);
     g_signal_connect(Bbin, "clicked", G_CALLBACK(on_fbin), &inter);

     gtk_main();
     return 1;                                                             
 } 
