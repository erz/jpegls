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
#include "Lz77.h"


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
	//On trouve un nom de fichier temporaire qui n'existe pas dans le dossier.
	char * nomTemp = ".TEMP";
	while(existeFichier(nomTemp))
	{
			char * nomTemp2 = (char*)malloc(sizeof(*nomTemp)+2);
			strcat(nomTemp2, nomTemp);
			strcat(nomTemp2, "_\0");
			nomTemp = nomTemp2;
	}
	
	//LZ77
	printf("Compression LZ77 en cours...\n");
    FILE *source = NULL ;
	FILE *destination = NULL ;
	dico* dictionnaire = NULL;
	
	source = ouvrirFichier(fichierACompresser, "rb") ;
	destination = ouvrirFichier(nomTemp, "wb+") ;
	dictionnaire = creerDico();
	LZ_Compression(dictionnaire, source, destination);
	fermerFichier(source);
	fermerFichier(destination);
	detruireDico(dictionnaire);
	printf("Compression LZ77 terminee.\n");
	
	// HUFFMAN 
	printf("Compression Huffman en cours...\n");
    Huffman::THuffman comp(nomTemp,fichierDestination);
    comp.Compresser();
	printf("Compression Huffman terminee...\n");
}

/**
*Décompresse le fichier.
*
*@param fichierADecompresser Le fichier d'origine
*@param fichierDestination Le fichier de destination
*/
extern void decompresser(const char *fichierADecompresser, const char *fichierDestination)
{  	
	//On trouve un nom de fichier temporaire qui n'existe pas dans le dossier.
	char * nomTemp = ".TEMP";
	while(existeFichier(nomTemp))
	{
			char * nomTemp2 = (char*)malloc(sizeof(*nomTemp)+2);
			strcat(nomTemp2, nomTemp);
			strcat(nomTemp2, "_\0");
			nomTemp = nomTemp2;
	}
 	
	//HUFFMAN
	printf("Decompression Huffman en cours...\n");
	Huffman::THuffman dec(fichierADecompresser,nomTemp);
	dec.Decompresser();
	printf("Decompression Huffman terminee.\n");
	
   	//LZ77 
	printf("Decompression LZ77 en cours...\n");
    FILE *source = NULL ;
	FILE *destination = NULL ;
	dico* dictionnaire = NULL;
	
	source = ouvrirFichier(nomTemp, "rb") ;
	destination = ouvrirFichier(fichierDestination, "wb+") ;
	dictionnaire = creerDico();
	LZ_Decompression(dictionnaire, source, destination);
	fermerFichier(source);
	fermerFichier(destination);
	detruireDico(dictionnaire);
	printf("Decompression LZ77 terminee.\n");
}
