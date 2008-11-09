/**
*@file Conversions.c
*@author Aurelien FIOL
*@author Florent MAS
*@date 6.12.2006
*@version 1.00
*
*Ce module permet d'effectuer les conversions de types.
*/

/*   INCLUDES   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Conversions.h"

/***************************************************************************************
							FONCTIONS PUBLIC
***************************************************************************************/

/**
*Convertit la chaine de caracteres passee en entree, composee de 0 et de 1 en sa valeur entiere.
*
*@param chaineBinaire chaine de caracteres
*@return entier
*/
extern int binaireEntier(char *chaineBinaire)
{	
	int valeur = 0 ;
     int indice ;
     int longueurChaine ;
     
     longueurChaine = strlen(chaineBinaire) ;
	
     for (indice = 0 ; indice < longueurChaine ; indice++)
	{
		if (chaineBinaire[indice] == '1')
		{
			valeur = valeur + pow(2, longueurChaine - indice - 1) ;
		}
	}
	
	return (valeur) ;
}

/**
*Convertit le nombre entier passe en parametre en une chaine de caractere representant sa valeur en binaire, et tenant sur le nombre de bits passes en parametres
*
*@param nombre nombre entier
*@param nbBits nombre de bits 
*@return chaine de caracteres composee de 0 et de 1
*/
extern char *entierBinaire(int nombre, int nbBits)
{	
	int indice ;
	char *valeur = (char *)malloc(nbBits + 1) ;
	valeur[nbBits] = '\0' ;
	
	for (indice = (nbBits-1) ; indice >= 0 ; indice--)
	{
		if (nombre % 2 == 0)
		{
			valeur[indice] = '0' ;
		}
		else
		{
			valeur[indice] = '1' ;
		}

		nombre = nombre / 2 ;
	}

	return (valeur) ;
}

/**
*Convertit la chaine de caracteres passee en parametre en une autre chaine de caracteres representant le code ASCII de cette chaine
*
*@param chaineBinaire chaine de caracteres composee de 0 et de 1
*@return chaine de caracteres representant le code ASCII de la chaine binaire
*/
extern char *binaireAscii(char *chaineBinaire)
{
     int indice1 ;
     int indice2 ;
     int indice3 = 0 ;
     char valeurAscii = 0 ;
     int longueurBinaire = strlen(chaineBinaire) ;
     char *chaineTemp = (char *)malloc(9) ;
	chaineTemp[8] = '\0' ;     
	char *chaineAscii = (char *)malloc((longueurBinaire / 8) + 1) ;
	chaineAscii[longueurBinaire / 8] = '\0' ;

     for(indice1 = 0 ; indice1 < longueurBinaire ; indice1++)
     {
		for(indice2 = 0 ; indice2 < 8 ; indice2++)
		{
			chaineTemp[indice2] = chaineBinaire[indice2 + indice1] ;
		}

		indice1 = indice1 + 7 ;
		valeurAscii = binaireEntier(chaineTemp) ;
		chaineAscii[indice3] = valeurAscii ; 
		indice3++ ;
	}
     
     free(chaineTemp) ;
     return (chaineAscii) ;  
}

/**
*Convertit une chaine de caracteres passee en parametre en une autre chaine de caracteres composee de 0 et de 1
*
*@param chaineAscii chaine de caracteres representant un code ASCII
*@return chaine de caracteres composee de 0 et de 1 representant la valeur binaire du code ASCII
*/
extern char *asciiBinaire(char *chaineAscii)
{
	int indice1 ;
	int indice2 ;
	int indice3 = 0 ;
	int longueurAscii = strlen(chaineAscii) ;
	int longueurBin = (longueurAscii * 8) ;
     int valeurAscii = 0 ;
     char *chaineTemp = (char *)malloc(9) ;
	chaineTemp[8] = '\0' ;     
	char *chaineBinaire = (char *)malloc(longueurBin + 1) ;     
     chaineBinaire[longueurBin] = '\0' ;
     
     for ( indice1 = 0 ; indice1 < longueurAscii ; indice1++ )
     {
		valeurAscii = (int)(chaineAscii[indice1]) ;
		chaineTemp = entierBinaire(valeurAscii, 8) ;
              
		for ( indice2 = 0 ; indice2 < 8 ; indice2++)
		{
			chaineBinaire[indice2 + indice3] = chaineTemp[indice2] ;
		}
		indice3 = indice3 + 8 ;
	}
     free(chaineTemp) ;
     return (chaineBinaire) ;
}

/**
*Effectue l'operation XOR sur les deux chaines de bit passees en parametres
*
*@param mot1 chaine de caracteres composee de 0 et de 1
*@param mot2 chaine de caracteres composee de 0 et de 1
*@return chaine de caracteres composee de 0 et de 1
*/
extern char *additionBitABit(char *mot1, char *mot2)
{
     int indice1 ;
     int indice2 = 0 ;
     int longMot1 = strlen(mot1) ;
     int longMot2 = strlen(mot2) ;
     int longueurMax = (longMot1 > longMot2) ? longMot1 : longMot2 ;
     char *resultat = (char *)malloc(longueurMax + 1) ;
     
     if (longMot1 == longueurMax)
     {
		for(indice1 = 0; indice1 < (longueurMax-longMot2); ++indice1)
		{
			resultat[indice1] = mot1[indice1] ;
		}
		while (indice1 < longueurMax)
		{
			if (mot1[indice1] == mot2[indice2])
				resultat[indice1] = '0' ;
			else
				resultat[indice1] = '1' ;
			++indice1 ;
			++indice2 ;
		}  
     }              
     else    
     {
		for(indice1 = 0; indice1 < (longueurMax-longMot1); ++indice1)
		{
			resultat[indice1] = mot2[indice1] ;
		}
		while (indice1 < longueurMax)
		{
			if (mot1[indice2] == mot2[indice1])
				resultat[indice1] = '0' ;
			else
				resultat[indice1] = '1' ;
			++indice1 ;    
			++indice2 ;    
		}      
     }     
     resultat[longueurMax] = '\0' ;
    
     return (resultat) ;                                                      
}
