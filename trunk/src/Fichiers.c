/**
*@file Fichiers.c
*@author Aurelien FIOL
*@author Florent MAS
*@date 13.12.2006
*@version 1.00
*
*Ce module permet d'effectuer toutes les operations de lecture et d'ecriture dans les fichiers.
*Il teste egalement l'existence des fichiers.
*/

/*   INCLUDES   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fichiers.h"

/***************************************************************************************
							FONCTIONS PUBLIC
***************************************************************************************/

/**
*Sert a verifier que le fichier existe ou non
*
*@param fichierATester chaine de caracteres representant le fichier a tester
*@return 0 si le fichier n'existe pas, 1 sinon
*/
extern int existeFichier(const char *fichierATester) 
{
	int existe = 0 ;
	FILE *fichier = NULL ;
	fichier = fopen(fichierATester, "r") ;

	if (fichier != NULL)
	{
		existe = 1 ;
		fclose(fichier);
	}
	else
	{
		existe = 0 ;
	}
	
	return (existe);
}

/**
*Sert a ouvrir un fichier
*
*@param fichierAOuvrir chaine de caracteres representant le fichier a ouvrir
*@param mode mode d'ouverture du fichier
*@return renvoie un pointeur sur FILE
*/
extern FILE *ouvrirFichier(const char *fichierAOuvrir, const char *mode)
{
	FILE *fichier = NULL ;
	fichier = fopen(fichierAOuvrir, mode) ;

	if (fichier == NULL)
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier '%s' dans le mode indique .\n", fichierAOuvrir) ;
	}
	else
	{
		printf("Ouverture ou creation du fichier '%s' reussie .\n", fichierAOuvrir) ;
	}

	return (fichier) ;
}

/**
*Sert a fermer un fichier
*
*@param fichier fichier a fermer
*/
extern void fermerFichier(FILE *fichier)
{
	fclose(fichier) ;
}

/**
*Sert a lire directement dans un fichier le nombre d'octets passe en parametre.
*
*@param fichier fichier dans lequel on souhaite lire
		 
*@return une chaine de caracteres composee de 0 et de 1 qui represente le bloc de caracteres lus converti en binaire
*/
extern char *lireDansFichier(FILE *fichier, int nbALire)
{
	int indice = 0 ;
	int valEnt;
	char *buffOct  = (char *)malloc(8+1) ;
	char *buffer = (char *)malloc(nbALire*8+1) ;
	strcpy(buffer, "") ;
	
    for (indice = 0; indice < nbALire; ++indice)
	{
	    valEnt = fgetc(fichier) ;
         
         if (valEnt < 0)
	    {
		  if (indice==0) 
               return ((char*)(NULL)) ;
	       buffOct = "00100000" ;
         }
         else
	         buffOct = entierBinaire(valEnt, 8) ;

	    strcat(buffer, buffOct);
    }
    buffer[nbALire*8] = '\0' ;
     return (buffer) ;
}

/**
*Prend la chaine de caracteres passee en parametre, et l'ecrit directement en binaire dans le fichier passe en parametre.
*
*@param fichier fichier dans lequel on souhaite ecrire
*@param chaine chaine de caracteres, composee de 0 et de 1, a ecrire dans le fichier
*/
extern void ecrireDansFichier(FILE *fichier, char *chaine, int nbAEcrire)
{
    int indice1 ;
    int indice2 ;
    char *buffer = (char *)malloc(9) ;
    int valEnt;
    for (indice1 = 0; indice1 < nbAEcrire; ++indice1)
    {
        for (indice2 = 0; indice2 < 8; ++indice2)
        {
            buffer[indice2] = chaine[indice2 + indice1*8] ;
        }
        buffer[8] = '\0' ;
        valEnt = binaireEntier(buffer) ;
        fputc(valEnt, fichier) ;
    }
    free(buffer) ;
}

/**
*Sert a tester simplement si la fin du fichier est atteinte ou non
*
*@param fichier fichier sur lequel on souhaite tester si la fin est atteinte
*@return 0 tant que la fin du fichier n'est pas atteinte et une valeur non nulle sinon
*/
extern int finFichier(FILE *fichier)
{
	return (feof(fichier)) ;
}
