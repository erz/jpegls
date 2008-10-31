/**
*@file Fichiers.h
*@author Aurelien FIOL
*@author Florent MAS
*@date 13.12.2006
*@version 1.00
*
*En-tete du module permettant d'effectuer toutes les operations de lecture/ecriture sur les fichiers
*/

/*	DEFINES	*/
#ifndef __FICHIERS__
#define __FICHIERS__

/*	INCLUDES	*/
#include "Conversions.h"

/*	DECLARATIONS	*/
extern int existeFichier(const char *fichierATester) ;
extern FILE *ouvrirFichier(const char *fichierAOuvrir, const char *mode) ;
extern void fermerFichier(FILE *fichier) ;
extern char *lireDansFichier(FILE *fichier, int nbALire) ;
extern void ecrireDansFichier(FILE *fichier, char *chaine, int nbAEcrire) ;
extern int finFichier(FILE *fichier) ;

#endif
