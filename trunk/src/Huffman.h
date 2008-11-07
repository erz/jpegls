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

// Codes d'erreurs
#define ERRCODE_PASDERREUR                  0L  
#define ERRCODE_OUVERTURELECTURE            1L
#define ERRCODE_OUVERTUREECRITURE           2L
#define ERRCODE_PASANALYSE                  3L
#define ERRCODE_PASCOMPRESSE                4L
#define ERRCODE_FICHIERINDEFINI             5L
#define ERRCODE_FICHIERVIDE                 6L
#define ERRCODE_ERREURECRITURE              7L
#define ERRCODE_ERREURLECTURE               8L
#define ERRCODE_INATTENDUE                  9L



// Type permetant de définir les messages a envoyer aux fonctions de comm
typedef enum {ddMuet = 0, ddProgression = 1, ddEtape = 2, ddBavard = 3}
        TDegreDialogue;

// Fonction permattant d'afficher un message a partir d'un string
typedef void (*TFoncMessage)(const std::string &) ;

// Structure utilisée pour la file de priorité pour éviter la recopie
// espace de nomage contenant le nécessaire à la (dé)compression de Huffman
namespace Huffman
{

   // structure du noeud de l'arbre de Huffman
   struct TNoeudHuffman
   {
      unsigned char Valeur;                    // Fragment
      unsigned int Frequence;                  // Frequence des sous-arbres
      std::string * lpsCode;                   // Pointeur sur la chaine qui
                                               // contient le code de Huffman 
      TNoeudHuffman * FilsG;
      TNoeudHuffman * FilsD;

      inline TNoeudHuffman(unsigned char Val = 0, unsigned int Freq = 0,
                           std::string * lpsCodeStr = NULL,
                           TNoeudHuffman * Fg = NULL, TNoeudHuffman * Fd = NULL)
                : Valeur(Val), Frequence(Freq), lpsCode(lpsCodeStr), FilsG(Fg),
                  FilsD(Fd){}

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
         bool bCompresse;                      // Compression réalisée (O/N)
         std::string sSource;                  // Fichier source
         std::string sDestination;             // Fichier de destination
         TNoeudHuffman * ArbreHuffman;         // Arbre de Huffman
         unsigned short int NombreElements;         // Nombre d'éléments différents
         unsigned long int TailleReelle;       // Taille du fichier originale
                                               // lue dans l'entête du fihier
                                               // source.
         unsigned long long int FinEntete;          // Position à laquelle continuer
                                               // la lecture pour décompresser  
         TStatistiqueHuffman * Statistiques;   // Statistiques du fichier source
         TDegreDialogue DegreVerbalite;        // Degre de parole de la classe
         TFoncMessage FoncAffichageProgression;// Fonction pour affichage de la
                                               // progression des différentes
                                               // fonctionnalités de la classe
         TFoncMessage FoncAffichageEtape;      // Idem pour les etapes
         unsigned int ErrCode;                 // Code de la dernière erreur
                                               // survenue
         std::string sErrString;               // Message associé à la dernière
                                               // erreur survenue      

      protected :
         virtual bool EcrireEntete();          // Ecrit l'entête dans le fichier
                                               // de destination
         virtual bool LireEntete();            // Lit l'entête à partir du
                                               // fichier source
         bool ConstruireArbreFreq();           // Construit l'arbre de Huffman à
                                               // partir des frequences
                                               // ==> déduire le code
         bool ConstruireArbreCode();           // Construit l'arbre de Huffman à
                                               // partir des codes
                                               // ==> décompression

      public :
         // Constructeur / Destructeur
         explicit THuffman(const std::string &sSrc = std::string(""),
                                  const std::string &sDst = std::string(""),
                                  TDegreDialogue Dialogue = ddBavard,
                                  const TFoncMessage Progres = NULL,
                                  const TFoncMessage Etape = NULL);
         virtual ~THuffman();

         virtual bool Analyser();              // Analyse le fichier source et
                                               // alimente les statistiques
         bool Compresser();                    // Compresser le fichier source
         bool Decompresser();                  // Décompresser le fichier source

         unsigned long int TailleInitiale();
         unsigned long int TailleCompresse();  // Disponible uniquement après
                                               // l'analyse
         unsigned long int TailleFinale();     // Disponible uniquement après
                                               // la compression

         void SetSource(const std::string &sSrc);
         void SetDestination(const std::string &sDst);
         void SetDegreDialogue(const TDegreDialogue v);
         void SetFonctionProgression(TFoncMessage Progres);
         void SetFonctionEtape(TFoncMessage Etape);

         std::string GetSource() const;
         std::string GetDestination() const;
         TDegreDialogue GetDegreDialogue() const;

         unsigned int GetDerniereErreur(std::string &sMessage) const;

         // Fonction par défaut pour l'affichage des messages.
         static void AfficherMessage(const std::string &sMessage);
   };
}
#endif
