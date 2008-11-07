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



// Type permetant de d�finir les messages a envoyer aux fonctions de comm
typedef enum {ddMuet = 0, ddProgression = 1, ddEtape = 2, ddBavard = 3}
        TDegreDialogue;

// Fonction permattant d'afficher un message a partir d'un string
typedef void (*TFoncMessage)(const std::string &) ;

// Structure utilis�e pour la file de priorit� pour �viter la recopie
// espace de nomage contenant le n�cessaire � la (d�)compression de Huffman
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
         bool bCompresse;                      // Compression r�alis�e (O/N)
         std::string sSource;                  // Fichier source
         std::string sDestination;             // Fichier de destination
         TNoeudHuffman * ArbreHuffman;         // Arbre de Huffman
         unsigned short int NombreElements;         // Nombre d'�l�ments diff�rents
         unsigned long int TailleReelle;       // Taille du fichier originale
                                               // lue dans l'ent�te du fihier
                                               // source.
         unsigned long long int FinEntete;          // Position � laquelle continuer
                                               // la lecture pour d�compresser  
         TStatistiqueHuffman * Statistiques;   // Statistiques du fichier source
         TDegreDialogue DegreVerbalite;        // Degre de parole de la classe
         TFoncMessage FoncAffichageProgression;// Fonction pour affichage de la
                                               // progression des diff�rentes
                                               // fonctionnalit�s de la classe
         TFoncMessage FoncAffichageEtape;      // Idem pour les etapes
         unsigned int ErrCode;                 // Code de la derni�re erreur
                                               // survenue
         std::string sErrString;               // Message associ� � la derni�re
                                               // erreur survenue      

      protected :
         virtual bool EcrireEntete();          // Ecrit l'ent�te dans le fichier
                                               // de destination
         virtual bool LireEntete();            // Lit l'ent�te � partir du
                                               // fichier source
         bool ConstruireArbreFreq();           // Construit l'arbre de Huffman �
                                               // partir des frequences
                                               // ==> d�duire le code
         bool ConstruireArbreCode();           // Construit l'arbre de Huffman �
                                               // partir des codes
                                               // ==> d�compression

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
         bool Decompresser();                  // D�compresser le fichier source

         unsigned long int TailleInitiale();
         unsigned long int TailleCompresse();  // Disponible uniquement apr�s
                                               // l'analyse
         unsigned long int TailleFinale();     // Disponible uniquement apr�s
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

         // Fonction par d�faut pour l'affichage des messages.
         static void AfficherMessage(const std::string &sMessage);
   };
}
#endif
