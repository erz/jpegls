/**
*@file main.c
*@author Aurélien FIOL
*@author Florent MAS
*@date 30.10.2008
*@version 1.00
*
*Point d'entree du programme.
*Ce module permet d'analyser les parametres saisis par l'utilisateur, puis de lancer la compression du fichier.
*/

/*   INCLUDES   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fichiers.h"
#include "Compression.h"


/*	DEFINES	*/
#ifdef WIN32
#define CLEAR_SCREEN system("cls") ;
#else
#define CLEAR_SCREEN system("clear") ;
#endif

/*   DECLARATION DES FONCTIONS   */
static void afficherAide() ;

/**************************************************************************************
							FONCTIONS PUBLIC
***************************************************************************************/

/**
*Point d'entree du programme. Ce programme analyse les parametres saisis par l'utilisateur, puis lance la compression.
*
*@param argc nombre d'arguments
*@param argv arguments
*@return 0 si le le programme se termine correctement
*/
int main(int argc, char *argv[])
{
	CLEAR_SCREEN ;	//On efface le contenu de la console.
	printf("********************************************************************************\n") ;
	printf("\t\t\tPROGRAMME DE COMPRESSION \n") ;
	printf("********************************************************************************\n") ;
	
     if (argc != 4)
     {
          fprintf(stderr, "Vous n'avez pas saisi le bon nombre d'arguments.\n") ;
          afficherAide() ;
     }
     else
     {
     	if (strcmp(argv[1],"d") && strcmp(argv[1],"c"))
     	{
          fprintf(stderr, "Vous n'avez pas saisi les bons arguments.\n") ;
          afficherAide() ;
		} 
		else
		{
			//on teste si le fichier source passe en parametre existe.
		   if (existeFichier(argv[2]))
		   {
				//On verifie que le fichier de destination n'existe pas deja.
				if (existeFichier(argv[3]))
				{
					fprintf(stderr, "Erreur : Le fichier de destination que vous avez indique existe deja .\n Veuillez indiquer un autre nom ou un autre emplacement pour le fichier de destination ou alors supprimez d'abord le fichier deja present.\n") ;
				}
				//Alors on lance le programme
				else
				{                
					if (strcmp(argv[1],"c"))
					{		
						//On appelle la fonction de compression.
						printf("Compression en cours...\n") ;
						compresser(argv[2], argv[3]) ;
						printf("\nLa compression a reussie .\n") ;
					}
					else
					{
						//On appelle la fonction de compression.
						printf("Décompression en cours...\n") ;
						compresser(argv[2], argv[3]) ;
						printf("\nLa décompression a reussie .\n") ;
					}
					
				}
		   }
		   else
		   {
				fprintf(stderr, "Erreur : Le fichier source que vous avez indique n'existe pas .\n") ;
		   }	
   		}
     }

     return (0) ;
}

/***************************************************************************************
							FONCTIONS PRIVATE
***************************************************************************************/

/**
*Affiche un message a l'ecran pour permettre a l'utilisateur de savoir comment utiliser le programme.
*/
static void afficherAide(void)
{
     printf("\nLe programme necessite que vous indiquiez 2 parametres :\n") ;
     printf("\t1. L'action souhaitee (c pour compression, d pour décompression.\n") ;
     printf("\t2. Le nom du fichier source .\n") ;
	printf("\t3. Le nom du fichier une fois compressé.\n") ;
     printf("\nL'usage du programme est donc :\n") ;
     printf("\n\t.\\Compression <c|d> <nom_fichier_source> <nom_fichier_destination>\n") ;
     printf("\n") ;
     printf("<nom_fichier_source>\t: nom du fichier a crypter ou decrypter .\n") ;
	printf("<nom_fichier_destination>\t: nom du fichier une fois crypte ou decrypte .\n") ;
}
