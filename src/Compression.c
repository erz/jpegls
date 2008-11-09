/**
*@file Fichiers.c
*@author Aurelien FIOL
*@author Florent MAS
*@date 13.12.2006
*@version 1.00
*
*Ce module permet d'effectuer la compression et la decompression de fichiers.
*/

/*   INCLUDES   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fichiers.h"
#include "Compression.h"
#include "Huffman.h"


/***************************************************************************************
							FONCTIONS PUBLIC
***************************************************************************************/

/**
*Compresse le fichier.
*
*@param fichierACompresser Le fichier d'origine
*@param fichierDestination Le fichier de destination
*/
extern void compresser(const char *fichierACompresser, const char *fichierDestination)
{
	///////////////TEST//////////////
	/*
	FILE *source = NULL ;
	FILE *destination = NULL ;
	char *chaine = (char *)malloc(65) ;
	
	source = ouvrirFichier(fichierACompresser, "rb") ;
	destination = ouvrirFichier(fichierDestination, "wb+") ;
	while (!finFichier(source))
	{
		chaine = lireDansFichier(source, 10) ;	
		if (chaine == NULL) 
			 break ;
		
		huffmann(lz77(chaine));
		
		ecrireDansFichier(destination, chaine, 10) ;
	}
	
	free(chaine);
	*/
   Huffman::THuffman comp(fichierACompresser,fichierDestination);

   comp.Compresser();
	
	//////////////FIN TEST/////////////
}

/**
*Codage de Huffmann
*
*@param chaine Chaine de bits à compresser
*@return renvoie la chaine compressée
*/
extern char* huffmann(char * chaineACompresser)
{
		///////////A CODER
		return chaineACompresser;
}

/**
*Codage de Huffmann
*
*@param chaine Chaine de bits à compresser
*@return renvoie la chaine compressée
*/
extern char* lz77(char * chaineACompresser)
{
		///////////A CODER
		printf("Lu : %s\n", chaineACompresser);
		return chaineACompresser;
}

/**
*Décompresse le fichier.
*
*@param fichierADeompresser Le fichier d'origine
*@param fichierDestination Le fichier de destination
*/
extern void decompresser(const char *fichierADecompresser, const char *fichierDestination)
{
	 Huffman::THuffman dec(fichierADecompresser,fichierDestination);

   	 dec.Decompresser();
}
