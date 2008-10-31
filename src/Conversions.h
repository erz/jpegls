/**
*@file Conversions.h
*@author Aurelien FIOL
*@author Florent MAS
*@date 6.12.2006
*@version 1.00
*
*En-tete du module permettant d'effectuer toutes les conversions de types necessaires a la compression.
*/

/*	DEFINES	*/
#ifndef __CONVERSIONS__
#define __CONVERSIONS__

/*	DECLARATIONS	*/
extern int binaireEntier(char *chaineBinaire) ;
extern char *entierBinaire(int nombre, int nbBits) ;
extern char *binaireAscii(char *chaineBinaire) ;
extern char *asciiBinaire(char *chaineAscii) ;
extern char *additionBitABit(char *mot1, char *mot2) ;

#endif
