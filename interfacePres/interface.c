#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <sys/random.h>
#include <time.h>
#include <pthread.h>
#include "../traitement/k_moyen.h"

//add file
#include <netdb.h>

#include "../reseau_interface/struct.h"
#include "../reseau_interface/client.h"
#include "../reseau_interface/list.h"

//add
#define SIZE 1024
//Reseau 
int cfd =0;
int delete = 1;
int connect_in_reseau = 0;
list_connect* data_name = NULL;

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

	//add
	GtkButton * button_send;
	GtkEntry *  message;

	//for list people connect to server
	GtkEntry* name;
	GtkComboBoxText * combo_list_name;
	GtkButton* button_save_name;
	int edit_name;

	//all message
	GtkLabel* message1;
	GtkLabel* message2;
	GtkLabel* message3;
	GtkLabel* message4;
	GtkLabel* message5;
	GtkLabel* message6;

	GtkLabel* message11;
	GtkLabel* message12;
	GtkLabel* message13;
	GtkLabel* message14;
	GtkLabel* message15;
	GtkLabel* message16;
	//quit
	GtkButton* button_quit_reseau;

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

	//add
	gtk_widget_set_visible(GTK_WIDGET(inter->button_send), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message), FALSE);

	//for list name
	gtk_widget_set_visible(GTK_WIDGET(inter->name), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->combo_list_name), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->button_save_name), FALSE);

	//all message
	gtk_widget_set_visible(GTK_WIDGET(inter->message1), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message2), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message3), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message4), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message5), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message6), FALSE);

	gtk_widget_set_visible(GTK_WIDGET(inter->message11), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message12), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message13), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message14), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message15), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message16), FALSE);

	gtk_widget_set_visible(GTK_WIDGET(inter->button_quit_reseau), FALSE);

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

	//add
	gtk_widget_set_visible(GTK_WIDGET(inter->button_send), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message), FALSE);

	//for list name
	gtk_widget_set_visible(GTK_WIDGET(inter->name), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->combo_list_name), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->button_save_name), FALSE);

	//all message
	gtk_widget_set_visible(GTK_WIDGET(inter->message1), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message2), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message3), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message4), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message5), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message6), FALSE);

	gtk_widget_set_visible(GTK_WIDGET(inter->message11), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message12), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message13), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message14), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message15), FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message16), FALSE);


	gtk_widget_set_visible(GTK_WIDGET(inter->button_quit_reseau), FALSE);

	if (button != NULL)
	{
		inter->usless = 1;
	}
}

//add
void on_messagerie ( GtkButton * button, gpointer user_data)
{
	Inter * inter = user_data;

	gtk_widget_set_visible(GTK_WIDGET(inter->GImage),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BChoixImage),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->TChoixCouleur),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BChoixCouleur),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BStartSimplification),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BImageBase),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BImageSimplification),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->BTelechargement),FALSE);

	gtk_widget_set_visible(GTK_WIDGET(inter->TPresentation),FALSE);

	// messagerie
	gtk_widget_set_visible(GTK_WIDGET(inter->button_send), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message), TRUE);


	//for list name
	gtk_widget_set_visible(GTK_WIDGET(inter->name), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->combo_list_name), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->button_save_name), TRUE);

	//all message
	gtk_widget_set_visible(GTK_WIDGET(inter->message1), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message2), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message3), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message4), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message5), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message6), TRUE);

	gtk_widget_set_visible(GTK_WIDGET(inter->message11), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message12), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message13), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message14), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message15), TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->message16), TRUE);

	gtk_widget_set_visible(GTK_WIDGET(inter->button_quit_reseau), TRUE);

	if (button != NULL)
	{
		inter->usless = 1;
	}

}

//add

void add_message(Inter * inter)
{
	//right message
	gtk_label_set_text ( inter->message6 , gtk_label_get_text(inter->message5));
	gtk_label_set_text ( inter->message5 , gtk_label_get_text(inter->message4));
	gtk_label_set_text ( inter->message4 , gtk_label_get_text(inter->message3));
	gtk_label_set_text ( inter->message3 , gtk_label_get_text(inter->message2));
	gtk_label_set_text ( inter->message2 , gtk_label_get_text(inter->message1));

	//meft message
	gtk_label_set_text ( inter->message16 , gtk_label_get_text(inter->message15));
	gtk_label_set_text ( inter->message15 , gtk_label_get_text(inter->message14));
	gtk_label_set_text ( inter->message14 , gtk_label_get_text(inter->message13));
	gtk_label_set_text ( inter->message13 , gtk_label_get_text(inter->message12));
	gtk_label_set_text ( inter->message12 , gtk_label_get_text(inter->message11));

}


void on_send_message ( GtkButton * button, gpointer user_data)
{

	Inter * inter = user_data;


	char* empty = "";

	//send the name and message in reseau
	//send the name receiver
	char* name_receiver = gtk_combo_box_text_get_active_text ( inter->combo_list_name);

	//if you don' t select people
	if (name_receiver == NULL)
	{
		return;
	}

	send ( cfd, name_receiver, sizeof ( name_receiver), 0);
	


	//move old message
	add_message (inter);	

	//add new message
	const char* text_message_1= gtk_entry_get_text(inter->message);
	
	char * text_message = calloc( SIZE, sizeof ( char));
	strcpy ( text_message, text_message_1);

	gtk_label_set_text ( inter->message1 , text_message);


	sleep( 1);
	//send the message in the reseau
	send ( cfd, text_message, SIZE,0);
	printf ("name %s , message send : %s\n",name_receiver ,text_message);

	//clean a message

	gtk_entry_set_text ( inter->message, empty);
	gtk_label_set_text ( inter->message11 , empty);


	//add message in struct message
	list_message* list_message = seach_connect( name_receiver, data_name);
	add_message_send ( text_message, list_message);



	//printf ("send message : %s\n", text_message);
	if ( button != NULL)
		inter->usless = 1;

}

//first thread for resseau
void * thread_reseau ( void *arg)
{
	char* new_name = arg;
	int e =0;
	printf ("%s\n", new_name);



	cfd = created_client ();
	connect_in_reseau = 1;
	e = send ( cfd , new_name , strlen(new_name), 0);


	e++;

	return NULL;

}

void on_new_name ( GtkEntry* button, gpointer user_data)
{
	Inter * inter = user_data;
	//for don't add a new same name in the list and in GtkComboBoxText

	if ( strcmp (gtk_entry_get_text (inter->name), "") == 0)
	{
		return;
	}

	if (inter->edit_name == 0)
	{
		return;
	}

	const char* new_name_1 = gtk_entry_get_text ( inter->name);
	char* new_name = calloc(16, sizeof (char));
	strcpy ( new_name, new_name_1);


	gtk_combo_box_text_append ( inter->combo_list_name, NULL, new_name);

	//list connect local
	push_connect ( new_name, data_name);

	// don't edit the name
	gtk_editable_set_editable ( GTK_EDITABLE(inter->name), FALSE);	
	inter->edit_name = 0;


	// start a new client in server
	//create thread for use application and reseau in the same time
	pthread_t thr;
	int e = pthread_create ( &thr, NULL, thread_reseau, new_name);
	if (e != 0)
		errx (EXIT_FAILURE, "error for create a new thread");


	if (button != NULL)
		inter->usless = 1;	
}

void on_quit_reseau ( GtkButton * button, gpointer user_data)
{
	Inter * inter = user_data;

	send ( cfd, "exit\n", sizeof ("exit\n"), 0);

	gtk_editable_set_editable ( GTK_EDITABLE(inter->name), TRUE);
	//gtk_combo_box_text_remove_all ( inter->combo_list_name);
	inter->edit_name = 1;
	gtk_entry_set_text ( inter->name, "");

	connect_in_reseau = 0;

	if (button != NULL)
		inter->usless = 1;
}


void on_name_receiver ( GtkComboBoxText* button, gpointer user_data)
{
	Inter * inter = user_data;

	char* name_receiver = gtk_combo_box_text_get_active_text (inter->combo_list_name);

	list_message* list_message = seach_connect ( name_receiver, data_name);


	//change the label text	
	//right message
	gtk_label_set_text ( inter->message6 , list_message->mes6);
	gtk_label_set_text ( inter->message5 , list_message->mes5);
	gtk_label_set_text ( inter->message4 , list_message->mes4);
	gtk_label_set_text ( inter->message3 , list_message->mes3);
	gtk_label_set_text ( inter->message2 , list_message->mes2);
	gtk_label_set_text ( inter->message1 , list_message->mes1);

	//meft message
	gtk_label_set_text ( inter->message16 , list_message->mes16);
	gtk_label_set_text ( inter->message15 , list_message->mes15);
	gtk_label_set_text ( inter->message14 , list_message->mes14);
	gtk_label_set_text ( inter->message13 , list_message->mes13);
	gtk_label_set_text ( inter->message12 , list_message->mes12);
	gtk_label_set_text ( inter->message11 , list_message->mes11);




	if (button != NULL)
                inter->usless = 1;


}


void * thread_original ( void *arg)
{
	Inter* inter =  arg;
	int e;

	while (1)
	{
		while ( connect_in_reseau == 0)
		{
			sleep ( 1);
		}

		while (1)
		{
			char buf[SIZE];
			e = read ( cfd , buf , SIZE );

			printf ( "%s\n", buf);
			if ( strcmp ( buf , "name") == 0  )
			{

				char* buf_2 = calloc ( SIZE, sizeof ( char));;
				e = read ( cfd, buf_2, SIZE);
				gtk_combo_box_text_append ( inter->combo_list_name , NULL , buf_2 ) ;

				//push name in data locail
				printf ("name push :%s\n", buf_2);
				push_connect ( buf_2 , data_name);

				//printf ("%s ;;; %s\n", data_name->next->name, data_name->next->next->name);

			}
			else if ( strcmp ( buf, "message") == 0 )
			{

				sleep ( 0.01);
				char* buf_2 = calloc ( SIZE, sizeof( char));

				e = read ( cfd, buf_2, SIZE );

				printf ("first : %s\n" , buf_2);
				sleep ( 0.01);
				char* buf_name = calloc ( SIZE, sizeof ( char));; 
				e = read ( cfd, buf_name, SIZE);


				//for add in struct message 
				list_message* list_message = seach_connect ( buf_name, data_name);

				add_message_receive ( buf_2, list_message);


				if ( strcmp ( buf_name , gtk_combo_box_text_get_active_text( inter->combo_list_name)) == 0)
				{
					add_message (inter);

					gtk_label_set_text ( inter->message11, buf_2);
					gtk_label_set_text ( inter->message1, "");
				}

			}
			else if ( strcmp ( buf, "delete") == 0)
			{
				char* buf_2 = calloc ( SIZE, sizeof( char));
				e = read ( cfd, buf_2, SIZE);
				printf("%s\n", buf_2);
				
				//if name select in 

				gtk_widget_set_sensitive(GTK_WIDGET(inter->button_send), FALSE);
				while ( strcmp ( gtk_combo_box_text_get_active_text ( inter->combo_list_name), buf_2) == 0)
				{
					sleep (0.1);
				}
				
				//pop name in data local
				int index = pop_connect ( buf_2, data_name);
				printf ("index : %i\n", index);
				gtk_combo_box_text_remove ( inter->combo_list_name, index);
				gtk_widget_set_sensitive(GTK_WIDGET(inter->button_send), TRUE);

			}
			memset ( buf, 0 , SIZE);

			if( connect_in_reseau == 0)
				break;
			
		}
	}
	e++; //stop warning
	return NULL;
}

void sauvegarderImage(const char* nomFichier, int largeur, int hauteur) {
    // Ouvrir le fichier source en mode binaire pour lecture
    FILE* fichierSource = fopen("new_surface.bmp", "rb");
    if (fichierSource == NULL) {
        printf("Erreur lors de l'ouverture du fichier source.\n");
        return;
    }

    // Ouvrir le fichier de destination en mode binaire pour écriture
    FILE* fichierDestination = fopen(nomFichier, "wb");
    if (fichierDestination == NULL) {
        printf("Erreur lors de l'ouverture du fichier de destination.\n");
        fclose(fichierSource);
        return;
    }

    // Calculer la taille totale des pixels de l'image
    int taillePixels = largeur * hauteur * sizeof(int);

    // Allouer un tampon pour lire les pixels de l'image source
    int* pixels = (int*)malloc(taillePixels);
    if (pixels == NULL) {
        printf("Erreur lors de l'allocation de mémoire.\n");
        fclose(fichierSource);
        fclose(fichierDestination);
        return;
    }

    // Lire les pixels de l'image source
    int r = fread(pixels, sizeof(int), largeur * hauteur, fichierSource);
	if (r == -1)
		return;
		
    // Écrire les pixels dans le fichier de destination
    fwrite(pixels, sizeof(int), largeur * hauteur, fichierDestination);

    // Libérer la mémoire et fermer les fichiers
    free(pixels);
    fclose(fichierSource);
    fclose(fichierDestination);
}

void on_ftelechargement(GtkButton *button, gpointer user_data)
{
	Inter *inter = user_data;
	GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    // Création de la boîte de dialogue de sélection de fichier
    dialog = gtk_file_chooser_dialog_new("Enregistrer l'image",
                                         NULL,
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    // Ajout de filtres d'extension d'image (facultatif)
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Images");
    gtk_file_filter_add_mime_type(filter, "image/jpeg");
    gtk_file_filter_add_mime_type(filter, "image/png");
        gtk_file_filter_add_mime_type(filter, "image/bmp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Exécution de la boîte de dialogue
    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        // Enregistrer l'image à l'emplacement choisi
        sauvegarderImage(filename,800,1000);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
    
    if (button != NULL)
	{
		inter->usless = 1;
	}
}

// Main function
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


	//add
	GtkButton *button_send = 
		GTK_BUTTON (gtk_builder_get_object(builder, "button_send"));
	GtkEntry * message = 
		GTK_ENTRY (gtk_builder_get_object(builder, "message"));

	//for name people
	GtkEntry * name = 
		GTK_ENTRY ( gtk_builder_get_object( builder, "name"));
	GtkComboBoxText * combo_list_name =
		GTK_COMBO_BOX_TEXT (gtk_builder_get_object ( builder, "combo_list_name"));
	GtkButton * button_save_name =
		GTK_BUTTON ( gtk_builder_get_object(builder, "button_save_name"));

	//all message
	GtkLabel * message1 =
		GTK_LABEL (gtk_builder_get_object(builder, "message1"));
	GtkLabel * message2 =
		GTK_LABEL (gtk_builder_get_object(builder, "message2"));
	GtkLabel * message3 =
		GTK_LABEL (gtk_builder_get_object(builder, "message3"));
	GtkLabel * message4 =
		GTK_LABEL (gtk_builder_get_object(builder, "message4"));
	GtkLabel * message5 =
		GTK_LABEL (gtk_builder_get_object(builder, "message5"));
	GtkLabel * message6 =
		GTK_LABEL (gtk_builder_get_object(builder, "message6"));

	GtkLabel * message11 =
		GTK_LABEL (gtk_builder_get_object(builder, "message11"));
	GtkLabel * message12 =
		GTK_LABEL (gtk_builder_get_object(builder, "message12"));
	GtkLabel * message13 =
		GTK_LABEL (gtk_builder_get_object(builder, "message13"));
	GtkLabel * message14 =
		GTK_LABEL (gtk_builder_get_object(builder, "message14"));
	GtkLabel * message15 =
		GTK_LABEL (gtk_builder_get_object(builder, "message15"));
	GtkLabel * message16 =
		GTK_LABEL (gtk_builder_get_object(builder, "message16"));

	//quit button reseau
	GtkButton * button_quit_reseau = 
		GTK_BUTTON ( gtk_builder_get_object(builder, "button_quit_reseau" ));


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

		//add
		.message = message,
		.button_send = button_send,

		//for list name
		.name = name,
		.combo_list_name = combo_list_name,
		.button_save_name = button_save_name,
		.edit_name = 1,

		//add message
		.message1 = message1,
		.message2 = message2,
		.message3 = message3,
		.message4 = message4,
		.message5 = message5,
		.message6 = message6,

		.message11 = message11,
		.message12 = message12,
		.message13 = message13,
		.message14 = message14,
		.message15 = message15,
		.message16 = message16,

		.button_quit_reseau = button_quit_reseau,

		.usless = 1
	};

	//add for reseau

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
	g_signal_connect(BPresentation, "clicked", G_CALLBACK(on_fpresentation), &inter);
	g_signal_connect(BSimplification, "clicked", G_CALLBACK(on_fsimplification), &inter);
	// g_signal_connect(BMessagerie, "clicked", G_CALLBACK(on_fsolve), &inter);
	g_signal_connect(BQuit, "clicked", G_CALLBACK(gtk_main_quit), &inter);
	g_signal_connect(BChoixImage, "clicked", G_CALLBACK(on_fchose_image), &inter);

	g_signal_connect(BStartSimplification, "clicked", G_CALLBACK(on_fstartsimplification), &inter);

	g_signal_connect(BImageSimplification, "clicked", G_CALLBACK(on_fimagesimplifie), &inter);
	g_signal_connect(BImageBase, "clicked", G_CALLBACK(on_fimagebase), &inter);
	g_signal_connect(BTelechargement, "clicked", G_CALLBACK(on_ftelechargement), &inter);


	//add
	g_signal_connect(BMessagerie, "clicked" , G_CALLBACK(on_messagerie), &inter);

	g_signal_connect(button_send, "clicked" , G_CALLBACK(on_send_message), &inter);

	g_signal_connect(button_save_name, "clicked", G_CALLBACK(on_new_name), &inter);

	g_signal_connect(button_quit_reseau, "clicked", G_CALLBACK(on_quit_reseau), &inter);

	//for change label
	g_signal_connect(combo_list_name, "changed", G_CALLBACK( on_name_receiver), &inter);


	if ( delete == 1 )
	{
		
		data_name = init_list_connect();
		printf ("listen server\n");
		pthread_t pth;

		int errrrrrr = pthread_create ( &pth , NULL, thread_original ,&inter);
		if ( errrrrrr != 0)
			err ( EXIT_FAILURE, "error thread original");
		delete = 0;
	}

	//end add

	on_fpresentation(BPresentation, &inter);


	gtk_main();

	free(inter.filename);

	return 1;
}
