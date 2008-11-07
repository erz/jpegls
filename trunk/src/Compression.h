/**
*@file Fichiers.h
*@author Aurelien FIOL
*@author Florent MAS
*@date 13.12.2006
*@version 1.00
*
*En-tete du module de compression / décompression des fichiers.
*/

/*	DEFINES	*/
#ifndef __COMPRESSION__
#define __COMPRESSION__

/*	INCLUDES	*/

/*	DECLARATIONS	*/
extern void compresser(const char *fichierACompresser, const char *fichierDestination) ;
extern char* huffmann(char * chaineACompresser) ;
extern char* lz77(char * chaineACompresser);
extern void decompresser(const char *fichierADecompresser, const char *fichierDestination) ;

#endif
