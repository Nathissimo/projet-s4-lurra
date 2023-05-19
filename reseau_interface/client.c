#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "client.h"

#define BUFFER_SIZE 32
#define PORT 2048


int created_client ()
{
	/*
	const char* command = "nc localhost 2048";
	int cr = system ( command);

	if ( cr != 0)
	{
		errx ( 1, "Commande is not valid");
	}
	return 1;*/
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Création du socket client
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Conversion de l'adresse IP en format binaire
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Adresse IP invalide ou non supportée");
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }
    return sock;


}
