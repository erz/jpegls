/**
*@file Lz77.c
*@author Aurelien FIOL
*@author Florent MAS
*@date 13.12.2006
*@version 1.00
*
*Ce module gere tout ce qui rapporte à la compression par l'algorithme de Lempel-Ziv 77.
*/

/*	DEFINES	*/
#ifndef __LZ77__
#define __LZ77__

/** 
 * Nombre de bits utilisés pour coder le decalage dans le dictionnaire.
*/
#define BITS_DECALAGE 12
/** 
 * Nombre de bits utilisés pour coder la longueur dans le dictionnaire.
*/
#define BITS_LONGUEUR 4 
/** 
 * Nombre de bits utilisés pour representer une ligne de sortie dans le dictionnaire.
 * (bits pour le decalage + bits pour la longueur) 
 *  Doit etre un multiple de 8 pour que les octets de sortie soient alignés.
*/
#define BITS_LIGNE (BITS_LONGUEUR + BITS_DECALAGE)
/**
 * Taille de la fenetre
 *
*/
#define BITS_FENETRE (1 << BITS_DECALAGE)  /* 4096 */
/**
 * Taille du lookahead
 * 
*/
#define BITS_LOOKAHEAD ((1 << BITS_LONGUEUR) - 1)  /* 15 */
/**
 * Octets par ligne dans le dico.
 *
*/
#define OCTETS_LIGNE (BITS_LIGNE / 8)  /* 2 */

//Typedef du dictionnaire
typedef struct dico_st dico;

//Fonctions privees
static void decompresserAvecDico(dico*, char const*, int, char*, int, int*);
static void compresserAvecDico(dico*, char const*, int, int*, char*, int);
static int remplirBuffer(char*, int, FILE*);
static int matchLongueur(char const*, int, char const*, int);
static void* memrot(void*, int, int);
static int viderBuffer(char const*, int, FILE*);
static void majDico(dico*, char const*, int);

/** 
 * Cree un dictionnaire pour utiliser LZ77.
*/
dico* creerDico(void);

/** Supprime le dictionnaire.
*/
void detruireDico(dico* d);

/** 
 * Compresse en LZ77 en utilisant le dictionnaire d.
*/
void LZ_Compression(dico* d, FILE* in, FILE* out);

/** 
 * Decompresse en LZ77 en utilisant le dictionnaire d.
*/
void LZ_Decompression(dico* d, FILE* in, FILE* out);

#endif

