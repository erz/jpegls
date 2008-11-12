/**
*@file Lz77.c
*@author Aurelien FIOL
*@author Florent MAS
*@date 13.12.2006
*@version 1.00
*
*Ce module gere tout ce qui rapporte à la compression par l'algorithme de Lempel-Ziv 77.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Lz77.h"

/** La structure du dictionnaire.
*/
struct dico_st
{
	/** Fenetre du dico.
	*/
	char fenetre[BITS_FENETRE];
};

/** 
 * Cree un dictionnaire pour utiliser LZ77.
*/
dico* creerDico(void)
{
	dico* d;

	d = (dico*)malloc(sizeof(*d));
	if (d == NULL) {
		return NULL;
	}

	//On initialise à 0
	memset(d->fenetre, 0, BITS_FENETRE);

	return d;
}


/** Supprime le dictionnaire.
*/
void detruireDico(dico* d)
{
	if (d == NULL) 
	{
		return;
	}

	free(d);
}


/** 
 * Compresse en LZ77 en utilisant le dictionnaire d
*/
void LZ_Compression(dico* d, FILE* in, FILE* out)
{
	int tailleBuffer, longueurBuffer, consommation, n;
	char* lookAheadBuffer;
	char bufferSortie[2];

	tailleBuffer = BITS_LOOKAHEAD + 1;  /* 15 + le symbole */

	lookAheadBuffer = (char*)malloc(tailleBuffer);
	if (lookAheadBuffer == NULL) 
	{
		return;
	}
	
	longueurBuffer = 0;

	/* Compression */
	while(1)
	{
		/* 
		 * On remplit le buffer de look-ahead.
		*/
		longueurBuffer += remplirBuffer(&lookAheadBuffer[longueurBuffer], tailleBuffer - longueurBuffer, in);

		if (longueurBuffer == 0) 
			break;

		/* Compression et ecriture */

		consommation = 0;
		compresserAvecDico(d, lookAheadBuffer, longueurBuffer - 1, &consommation, bufferSortie, sizeof(bufferSortie));

		n = viderBuffer(bufferSortie, 2, out);
		if (n < 2) 
		{  /* erreur */
			break;
		}

		/* On met a jour le dico */
		majDico(d, lookAheadBuffer, consommation);

		memmove(lookAheadBuffer, &lookAheadBuffer[consommation], longueurBuffer - consommation);
		longueurBuffer -= consommation;

		/* On ecrit le nouveau symbole */

		n = viderBuffer(lookAheadBuffer, 1, out);
		if (n < 1) 
		{  /* erreur */
			break;
		}

		/* MAJ du dico */
		majDico(d, lookAheadBuffer, 1);

		memmove(lookAheadBuffer, &lookAheadBuffer[1], longueurBuffer - 1);
		longueurBuffer--;
	}

	free(lookAheadBuffer);
}


/** 
 * Remplit un buffer avec un fichier d'entrée.
*/
static int remplirBuffer(char* buffer, int size, FILE* in)
{
	int i;
	int c;
	
	for(i = 0; i < size; i++) 
	{
		c = getc(in);
		if (c == EOF) 
		{
			break;
		}

		buffer[i] = (char)c;
	}

	return i;
}


/** 
 * Ecrit un buffer dans un fichier de sortie.
*/
static int viderBuffer(char const* buffer, int size, FILE* out)
{
	int i;
	int s;
	
	for (i = 0; i < size; ++i) 
	{
		s = putc(buffer[i], out);
		if (s == EOF) 
		{  /* erreur */
			break;
		}
	}

	return i;
}


/** 
 * Decompresse en LZ77 en utilisant le dictionnaire d.
*/
void LZ_Decompression(dico* d, FILE* in, FILE* out)
{
	int tailleBuffer, n, production;
	char* decodeBuffer;
	char inBuffer[2];
	char cc;

	tailleBuffer = BITS_LOOKAHEAD;  /* 15 */

	decodeBuffer = (char*)malloc(tailleBuffer);
	if (decodeBuffer == NULL) 
	{
		return;
	}


	/* Decompression */
	while(1)
	{

		/* remplissage du buffer */
		n = remplirBuffer(inBuffer, 2, in);
		if (n == 0) 
			break;

		if (n < 2) 
		{  /* erreur */
			break;
		}

		/* Decompression et ecriture */

		production = 0;
		decompresserAvecDico(d, inBuffer, OCTETS_LIGNE, decodeBuffer, tailleBuffer, &production);

		n = viderBuffer(decodeBuffer, production, out);
		if (n < production) {  /* output error */
			break;
		}

		/* MAJ du dico */
		majDico(d, decodeBuffer, production);

		/* ecrire le nouveau symbole */

		n = remplirBuffer(&cc, 1, in);
		if (n == 0) 
		{  /* manque le dernier symbole */
			break;
		}

		n = viderBuffer(&cc, 1, out);
		if (n == 0) 
		{  /* erreur */
			break;
		}

		/* MAJ du dico */
		majDico(d, &cc, 1);
	}

	free(decodeBuffer);
}


/** 
 * Compression avec un dico donné.
*/
static void compresserAvecDico(
	dico* d,
	char const* in,  /* entree */
	int inSize,  /* taille de l'entree, en octets */
	int* inUsed,  /* nb d'octets utilisés pour la compression */
	char* out,  /* buffer de sortie */
	int outSize  /* taille en octets du buffer de sortie */
)
{
	int decalage, longueur;

	*inUsed = 0;

	/* can't encode more than BITS_LOOKAHEAD */
	if (inSize > BITS_LOOKAHEAD) 
	{
		inSize = BITS_LOOKAHEAD;
	}

	if (outSize < OCTETS_LIGNE) 
	{
		return;
	}
	
	/* On cherche la souschaine correspondante la plus grande dans la fenetre */
	int r, m;

	longueur = 0;  
	decalage = 0;  

	for (r = 0; r < BITS_FENETRE; r++) 
	{  
		m = matchLongueur(&(d->fenetre[r]), BITS_FENETRE - r, in, inSize);
		if (m > longueur) 
		{ 
			longueur = m;
			decalage = r;
		}
	}

	*inUsed = longueur;

	if (longueur == 0) 
	{
		decalage = 0; 
	}

	/* Ecriture du decalage et de la longueur */
	out[0] = (char)(decalage >> (BITS_DECALAGE - 8));  
	out[1] = (char)((decalage << BITS_LONGUEUR) | longueur); 
}

/** 
 * Renvoie la longueur d'une correspondance de chaine possible.
*/
static int matchLongueur(char const* a, int an, char const* b, int bn)
{
	int n;

	n = 0;  /* match length */
	while (an-- > 0 && bn-- > 0 && *a++ == *b++) {
		n++;
	}

	return n;
}

/** 
 * Decompression avec un dictionnaire donné.
*/
static void decompresserAvecDico(dico* d,
	char const* in,  /* entree */
	int inSize,  /* taille de l'entree, en octet */
	char* out,  /* buffer de sortie */
	int outSize,  /* taille du buffer de sortie en octets */
	int* outUsed  /* nombe d'octets de sortie generés pendant la compression */
)
{
	int decalage, longueur;
	
	*outUsed = 0;

	if (inSize < OCTETS_LIGNE) 
	{
		return;
	}

	/* On decode le decalage et la longueur */
	decalage = ((unsigned char)in[0] << (BITS_DECALAGE - 8)) | ((unsigned char)in[1] >> BITS_LONGUEUR);
	longueur = in[1] & ((unsigned char)0xff >> (8 - BITS_LONGUEUR));

	if (outSize < longueur) 
		return;

	memmove(out, &d->fenetre[decalage], longueur);

	*outUsed = longueur;
}


/** 
 * MAJ du dictionnaire avec de nouvelles données.
*/
static void majDico(
	dico* d,
	char const* in,  /* entree pour la mise a jour */
	int inSize  /* taille en octets */
)
{
	if (inSize >= BITS_FENETRE) 
	{  
		memmove(d->fenetre, &in[inSize - BITS_FENETRE], BITS_FENETRE);
	} 
	else if (inSize > 0) 
	{  
		memmove(d->fenetre, in, inSize);
		memrot(d->fenetre, BITS_FENETRE, inSize);
	} 
}


/** 
 * Fait tourner un buffer de n octets vers la gauche.
*/
static void* memrot(void* p, int size, int n)
{
	int rounds, a, b;
	char *q, *e;

	if (size == 0) {  
		return p;
	}

	n %= size;  
	if (n < 0) { 
		n += size;
	}

	if (n == 0) {  
		return p;
	}

	q = (char*)p;  
	e = q + size;  
	 
	//Plus grand diviseur commun
	a = size;
	b = n;
	while (b != 0) 
	{
		int c = a % b;
		a = b;
		b = c;
	}
	rounds = a;

	for (; rounds > 0; q++, rounds--) 
	{
		char* r = q;
		char c = *r;  
		for (;;) {
			char* s = r + n;  
			if (s >= e) {  
				s -= size;
			}
			if (s == q) {
				break;  
			}

			*r = *s;  
			r = s;  
		}
		*r = c;
	}

	return p;
}
