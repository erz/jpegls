#ifndef HUFFMAN_HEADER
#define HUFFMAN_HEADER _

#include <string>
#include <iostream>

// Constantes diverses
#if defined (win32)
   #define SEPARATEUR_REPERTOIRE "\\"
#else
   #define SEPARATEUR_REPERTOIRE "/"
#endif



// Fonction permattant d'afficher un message a partir d'un string
typedef void (*TFoncMessage)(const std::string &) ;

// Structure utilis�e pour la file de priorit� pour �viter la recopie
// espace de nomage contenant le n�cessaire � la (d�)compression de Huffman
namespace Huffman
{

   // structure du noeud de l'arbre de Huffman
   struct TNoeudHuffman
   {
   	  // Fragment
      unsigned char Valeur;
      // Frequence des sous-arbres                    
      unsigned int Frequence; 
      // Pointeur sur la chaine qui contient le code de Huffman                 
      std::string * lpsCode;                   
                                               
      TNoeudHuffman * FilsG;
      TNoeudHuffman * FilsD;

      inline TNoeudHuffman(unsigned char Val = 0, unsigned int Freq = 0,std::string * lpsCodeStr = NULL, TNoeudHuffman * Fg = NULL, TNoeudHuffman * Fd = NULL)
                : Valeur(Val), Frequence(Freq), lpsCode(lpsCodeStr), FilsG(Fg), FilsD(Fd){}

      inline ~TNoeudHuffman()
                {
                   if (FilsG) delete FilsG;
                   if (FilsD) delete FilsD;
                }
      // Fonction r�cursive pour d�terminer le code de Huffman des feuilles
      void DeterminerCodeHuffman(const std::string &sCode) const;

      // Fonction r�cursive pour cr�er les noeuds correspondants � un code
      void CreerFeuille(const std::string &sCode, unsigned char valeur);

      friend bool operator<(const TNoeudHuffman &A, const TNoeudHuffman &B);
      friend bool operator>(const TNoeudHuffman &A, const TNoeudHuffman &B);
      friend bool operator==(const TNoeudHuffman &A, const TNoeudHuffman &B);
   };
   typedef TNoeudHuffman * PNoeudHuffman;

   // Structure des stats pour chaque �l�ment d'analyse
   struct TStatistiqueHuffman
   {
      unsigned long int Frequence;
      std::string sCode;

      TStatistiqueHuffman() : Frequence(0), sCode("") {}
      TNoeudHuffman * CreerNoeudHuffman(unsigned char Valeur);
   };

   // Classe Huffman permettant de r�aliser la (d�)compression de Huffman
   class THuffman
   {
      private :
      	 // Compression r�alis�e (O/N)
         bool bCompresse;
         // Fichier source                      
         std::string sSource;
         // Fichier de destination                 
         std::string sDestination;
         // Arbre de Huffman             
         TNoeudHuffman * ArbreHuffman;  
         // Nombre d'�l�ments diff�rents       
         unsigned short int NombreElements;  
         // Taille du fichier originale lue dans l'ent�te du fihier source    
         unsigned long int TailleReelle;      
                                               
         // Position � laquelle continuer la lecture pour d�compresser                                       
         unsigned long long int FinEntete;         
                                              
         // Statistiques du fichier source                                     
         TStatistiqueHuffman * Statistiques;
         
         
         // Fonction pour affichage de la progression des diff�rentes fonctionnalit�s de la classe   
         TFoncMessage FoncAffichageProgression;
         
         // Idem pour les etapes
         TFoncMessage FoncAffichageEtape;      
         
         
         std::string sErrString;               // Message associ� � la derni�re
                                               // erreur survenue      

      protected :
      
      	 // Ecrit l'ent�te dans le fichier de destination	
         virtual bool EcrireEntete();      
         // Lit l'ent�te � partir du fichier source  
         virtual bool LireEntete();   
         
          // Construit l'arbre de Huffman � partir des frequences ==> d�duire le code         
         bool ConstruireArbreFreq();          
          // Construit l'arbre de Huffman � partir des codes ==> d�compression
         bool ConstruireArbreCode();          

      public :
         // Constructeur 
         explicit THuffman(const std::string &sSrc = std::string(""),const std::string &sDst = std::string(""));
         // Destructeur
         virtual ~THuffman();
		 
		 // Analyse le fichier source et alimente les statistiques
         virtual bool Analyser(); 
         
         // Compresser le fichier source             
         bool Compresser();              
         
          // D�compresser le fichier source      
         bool Decompresser();                 

         unsigned long int TailleInitiale();
         unsigned long int TailleCompresse();  // Disponible uniquement apr�s
                                               // l'analyse
         unsigned long int TailleFinale();     // Disponible uniquement apr�s
                                               // la compression

         void SetSource(const std::string &sSrc);
         void SetDestination(const std::string &sDst);

         std::string GetSource() const;
         std::string GetDestination() const;

         // Fonction par d�faut pour l'affichage des messages.
         static void AfficherMessage(const std::string &sMessage);
   };
}
#endif

