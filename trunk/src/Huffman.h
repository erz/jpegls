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

// Structure utilisée pour la file de priorité pour éviter la recopie
// espace de nomage contenant le nécessaire à la (dé)compression de Huffman
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
      // Fonction récursive pour déterminer le code de Huffman des feuilles
      void DeterminerCodeHuffman(const std::string &sCode) const;

      // Fonction récursive pour créer les noeuds correspondants à un code
      void CreerFeuille(const std::string &sCode, unsigned char valeur);

      friend bool operator<(const TNoeudHuffman &A, const TNoeudHuffman &B);
      friend bool operator>(const TNoeudHuffman &A, const TNoeudHuffman &B);
      friend bool operator==(const TNoeudHuffman &A, const TNoeudHuffman &B);
   };
   typedef TNoeudHuffman * PNoeudHuffman;

   // Structure des stats pour chaque élément d'analyse
   struct TStatistiqueHuffman
   {
      unsigned long int Frequence;
      std::string sCode;

      TStatistiqueHuffman() : Frequence(0), sCode("") {}
      TNoeudHuffman * CreerNoeudHuffman(unsigned char Valeur);
   };

   // Classe Huffman permettant de réaliser la (dé)compression de Huffman
   class THuffman
   {
      private :
      	 // Compression réalisée (O/N)
         bool bCompresse;
         // Fichier source                      
         std::string sSource;
         // Fichier de destination                 
         std::string sDestination;
         // Arbre de Huffman             
         TNoeudHuffman * ArbreHuffman;  
         // Nombre d'éléments différents       
         unsigned short int NombreElements;  
         // Taille du fichier originale lue dans l'entête du fihier source    
         unsigned long int TailleReelle;      
                                               
         // Position à laquelle continuer la lecture pour décompresser                                       
         unsigned long long int FinEntete;         
                                              
         // Statistiques du fichier source                                     
         TStatistiqueHuffman * Statistiques;
         
         
         // Fonction pour affichage de la progression des différentes fonctionnalités de la classe   
         TFoncMessage FoncAffichageProgression;
         
         // Idem pour les etapes
         TFoncMessage FoncAffichageEtape;      
         
         
         std::string sErrString;               // Message associé à la dernière
                                               // erreur survenue      

      protected :
      
      	 // Ecrit l'entête dans le fichier de destination	
         virtual bool EcrireEntete();      
         // Lit l'entête à partir du fichier source  
         virtual bool LireEntete();   
         
          // Construit l'arbre de Huffman à partir des frequences ==> déduire le code         
         bool ConstruireArbreFreq();          
          // Construit l'arbre de Huffman à partir des codes ==> décompression
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
         
          // Décompresser le fichier source      
         bool Decompresser();                 

         unsigned long int TailleInitiale();
         unsigned long int TailleCompresse();  // Disponible uniquement après
                                               // l'analyse
         unsigned long int TailleFinale();     // Disponible uniquement après
                                               // la compression

         void SetSource(const std::string &sSrc);
         void SetDestination(const std::string &sDst);

         std::string GetSource() const;
         std::string GetDestination() const;

         // Fonction par défaut pour l'affichage des messages.
         static void AfficherMessage(const std::string &sMessage);
   };
}
#endif

