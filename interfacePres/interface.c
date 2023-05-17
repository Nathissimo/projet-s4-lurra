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

// interface
typedef struct Inter
{
	GtkWindow *window; // Main window
	GtkButton *BPresentation;
	GtkButton *BSimplification;
	GtkButton *BMessagerie;
	GtkButton *BQuit;
	GtkLabel *TPresentation;
	GtkButton *BChoixImage;
	GtkLabel *TChoixCouleur;
	GtkScale *BChoixCouleur;
	GtkButton *BStartSimplification;
	GtkButton *BImageSimplification;
	GtkButton *BImageBase;
	GtkButton *BTelechargement;
	GtkImage *GImage;
	GdkPixbuf *pix;
	char *filename;

	int usless; // this is usless
} Inter;

// for the chose buttom
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;

	gtk_widget_set_sensitive(GTK_WIDGET(inter->BStartSimplification), TRUE);

	// creat dialog windows
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new("Open File",
										 NULL,
										 action,
										 ("_Cancel"),
										 GTK_RESPONSE_CANCEL,
										 ("_Open"),
										 GTK_RESPONSE_ACCEPT,
										 NULL);

	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		inter->filename = filename;
		inter->pix = gdk_pixbuf_new_from_file(filename, NULL);
		if (inter->pix != NULL)
		{
			gtk_image_set_from_pixbuf(inter->GImage, inter->pix);
		}
	}

	gtk_widget_destroy(dialog);
	if (button != NULL)
	{
		inter->usless = 1;
	}
}

void on_fimagesimplifie(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;
	inter->pix = gdk_pixbuf_new_from_file(
		"new_surface.bmp", NULL);
	gtk_image_set_from_pixbuf(inter->GImage, inter->pix);
	if (button != NULL)
	{
		inter->usless = 1;
	}
}

void on_fimagebase(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;
	inter->pix = gdk_pixbuf_new_from_file(
		"old_surface.bmp", NULL);
	gtk_image_set_from_pixbuf(inter->GImage, inter->pix);
	if (button != NULL)
	{
		inter->usless = 1;
	}
}

void on_fstartsimplification(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;

	gtk_widget_set_sensitive(GTK_WIDGET(inter->BImageBase), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->BImageSimplification), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->BTelechargement), TRUE);


	int c = (int) gtk_range_get_value(GTK_RANGE(inter->BChoixCouleur));

	k_moyen(inter->filename, c, 10);

	on_fimagesimplifie(NULL,inter);

	if (button != NULL)
	{
		inter->usless = 1;
	}
}

void on_fsimplification(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;

	gtk_widget_set_visible(GTK_WIDGET(inter->TPresentation),FALSE);
	
	gtk_widget_set_visible(GTK_WIDGET(inter->GImage),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BChoixImage),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->TChoixCouleur),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BChoixCouleur),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BStartSimplification),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BImageBase),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BImageSimplification),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BTelechargement),TRUE);

	gtk_widget_set_sensitive(GTK_WIDGET(inter->BStartSimplification), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->BImageBase), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->BImageSimplification), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->BTelechargement), FALSE);

	if (button != NULL)
	{
		inter->usless = 1;
	}
}

void on_fpresentation(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;

	gtk_widget_set_visible(GTK_WIDGET(inter->GImage),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BChoixImage),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->TChoixCouleur),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BChoixCouleur),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BStartSimplification),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BImageBase),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BImageSimplification),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BTelechargement),FALSE);

	gtk_widget_set_visible(GTK_WIDGET(inter->TPresentation),TRUE);

	if (button != NULL)
	{
		inter->usless = 1;
	}
}


// Main function.
int main()
{
	// Initializes GTK.
	gtk_init(NULL, NULL);

	// Loads the UI description and builds the UI.
	// (Exits if an error occurs.)
	GtkBuilder *builder = gtk_builder_new();
	GError *error = NULL;
	if (gtk_builder_add_from_file(builder, "interfacePres/interface.glade", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return 1;
	}

	// Gets the widgets.
	GtkWindow *window =
		GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.inter"));

	// Main

	GtkButton *BPresentation =
		GTK_BUTTON(gtk_builder_get_object(builder, "Presentation"));

	GtkButton *BSimplification =
		GTK_BUTTON(gtk_builder_get_object(builder, "Simplification"));

	GtkButton *BMessagerie =
		GTK_BUTTON(gtk_builder_get_object(builder, "Messagerie"));

	GtkButton *BQuit =
		GTK_BUTTON(gtk_builder_get_object(builder, "Quit"));

	// Présentation

	GtkLabel *TPresentation =
		GTK_LABEL(gtk_builder_get_object(builder, "PresentationTexte"));

	// Simplification

	GtkButton *BChoixImage =
		GTK_BUTTON(gtk_builder_get_object(builder, "ChoixImage"));

	GtkLabel *TChoixCouleur =
		GTK_LABEL(gtk_builder_get_object(builder, "ChoixCouleurTexte"));

	GtkScale *BChoixCouleur =
		GTK_SCALE(gtk_builder_get_object(builder, "ChoixCouleur"));

	GtkButton *BStartSimplification =
		GTK_BUTTON(gtk_builder_get_object(builder, "StartSimplification"));

	GtkButton *BImageSimplification =
		GTK_BUTTON(gtk_builder_get_object(builder, "ImageSimplifie"));

	GtkButton *BImageBase =
		GTK_BUTTON(gtk_builder_get_object(builder, "ImageBase"));

	GtkButton *BTelechargement =
		GTK_BUTTON(gtk_builder_get_object(builder, "Telechargement"));

	GtkImage *GImage = GTK_IMAGE(gtk_builder_get_object(builder, "Image"));

	Inter inter =
		{
			.BPresentation = BPresentation,
			.BSimplification = BSimplification,
			.BMessagerie = BMessagerie,
			.BQuit = BQuit,
			.TPresentation = TPresentation,
			.BChoixImage = BChoixImage,
			.TChoixCouleur = TChoixCouleur,
			.BChoixCouleur = BChoixCouleur,
			.BStartSimplification = BStartSimplification,
			.BImageSimplification = BImageSimplification,
			.BImageBase = BImageBase,
			.BTelechargement = BTelechargement,
			.GImage = GImage,
			.usless = 1};

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
	g_signal_connect(BPresentation, "clicked", G_CALLBACK(on_fpresentation), &inter);
	g_signal_connect(BSimplification, "clicked", G_CALLBACK(on_fsimplification), &inter);
	// g_signal_connect(BMessagerie, "clicked", G_CALLBACK(on_fsolve), &inter);
	g_signal_connect(BQuit, "clicked", G_CALLBACK(gtk_main_quit), &inter);
	g_signal_connect(BChoixImage, "clicked", G_CALLBACK(on_fchose_image), &inter);

	g_signal_connect(BStartSimplification, "clicked", G_CALLBACK(on_fstartsimplification), &inter);

	g_signal_connect(BImageSimplification, "clicked", G_CALLBACK(on_fimagesimplifie), &inter);
	g_signal_connect(BImageBase, "clicked", G_CALLBACK(on_fimagebase), &inter);
	//g_signal_connect(BTelechargement, "clicked", G_CALLBACK(on_fsolve), &inter);


	on_fpresentation(BPresentation, &inter);
	gtk_main();

	free(inter.filename);

	return 1;
}
