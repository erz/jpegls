#include <queue>
#include <fstream>
#include <bitset>
#include <stdio.h>

#include "Comparateur.h"
#include "Huffman.h"

namespace Huffman
{
   // Fonctions de la classe TNoeudHuffman
   void TNoeudHuffman::DeterminerCodeHuffman(const std::string &sCode) const
   {
      // seules les feuilles terminales devraient avoir lpsCode non NULL
      if (lpsCode)
         *lpsCode = sCode;
      else
      {
         if (FilsG)
            FilsG->DeterminerCodeHuffman(sCode + "0");
         if (FilsD)
            FilsD->DeterminerCodeHuffman(sCode + "1");
      }
   }

   void TNoeudHuffman::CreerFeuille(const std::string &sCode,
                                    unsigned char valeur)
   {
      TNoeudHuffman * * p;
      // D�termination du pointeur qui va �tre modifi�
      if (sCode[0] == '0')
         p = &FilsG;
      else
         p = &FilsD;

      if (sCode.length() > 1)
      {
         // Cr�ation d'un noeud interm�diaire
         if (!(*p))
            *p = new TNoeudHuffman();
         (*p)->CreerFeuille(sCode.substr(1,sCode.length()),valeur);
      }
      else
      {
         // Cr�ation de la feuille (*p est obligatoirement null puisqu'une
         // feuille ne peut pas �tre un noeud interm�diaire)
         *p = new TNoeudHuffman(valeur);
         p = p;
      }

   }

   // Op�rateurs de la classe TNoeudHuffman
   bool operator<(const TNoeudHuffman &A, const TNoeudHuffman &B)
   {
      if (A.Frequence != B.Frequence)
         return A.Frequence < B.Frequence;
      else
         return A.Valeur < B.Valeur;
   }

   bool operator>(const TNoeudHuffman &A, const TNoeudHuffman &B)
   {
      if (A.Frequence != B.Frequence)
         return A.Frequence > B.Frequence;
      else
         return A.Valeur > B.Valeur;
   }

   bool operator==(const TNoeudHuffman &A, const TNoeudHuffman &B)
   {
      return (A.Frequence == B.Frequence) && (A.Valeur == B.Valeur);
   }

   // Fonctions associ�es � TStatistiqueHuffman
   TNoeudHuffman * TStatistiqueHuffman::CreerNoeudHuffman(unsigned char Valeur)
   {
      return new TNoeudHuffman(Valeur, Frequence, &this->sCode);
   }

   // Fonctions associ�es � THuffman
   // Fonctions publiques
   THuffman::THuffman(const std::string &sSrc, const std::string &sDst,
                      TDegreDialogue Dialogue, const TFoncMessage Progres,
                      const TFoncMessage Etape)
         : sSource(sSrc), sDestination(sDst), DegreVerbalite(Dialogue),
	   bCompresse(false), ArbreHuffman(NULL), Statistiques(NULL)
   {
      SetFonctionProgression(Progres);
      SetFonctionEtape(Etape);
   }

   THuffman::~THuffman()
   {
      //Suppression des donn�es allou�es dynamiquement
      if (Statistiques)
      {
         delete [] Statistiques;

         if (ArbreHuffman)
            delete ArbreHuffman;
      }
   }



   bool THuffman::Analyser()
   {
      std::ifstream fSrc;
      char Valeur;
      char BufferProgression[10];
      unsigned long int Taille = TailleInitiale();
      unsigned long int Position = 0;

      if (DegreVerbalite & ddEtape)
         FoncAffichageEtape("Analyse du fichier source");


      // Si le nom du fichier n'est pas d�fini ce n'est pas n�cessaire d'essayer
      // de l'ouvrir
      if (sSource.length())
         fSrc.open(sSource.c_str(), std::ios::in | std::ios::binary);
      else
      {
         std::cout<<"Veuillez pr�ciser le nom du fichier source"<<std::endl;
         return false;
      }

      if (fSrc.is_open())
      {
         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("0%");
         // Allocation du tableau de statistiques
         Statistiques = new TStatistiqueHuffman[256];

         // Parcours du fichier caract�re par caract�re
         // Les optimisations d'acc�s au fichier sont g�r�es par l'OS
         while (!fSrc.eof())
         {
            fSrc.read((char *)(&Valeur),sizeof(char));
            Statistiques[(unsigned char)(Valeur)].Frequence++;
            sprintf(BufferProgression,"%d%%",int(float(Position)/Taille*100));
            if (DegreVerbalite & ddProgression)
               FoncAffichageProgression(BufferProgression);
            Position++;
         }

         // Fermeture du fichier
         fSrc.close();
         return true;
      }
      else
      {
         std::cout<<"Impossible d'ouvrir le fichier d'entr�e en lecture"<<std::endl;
         return false;
      }
   }

   bool THuffman::Compresser()
   {
      typedef std::bitset<32> TDataBuffer;
      std::ifstream fSrc;
      std::ofstream fDst;
      char Valeur;
      char BufferProg[10];
      unsigned long int Position = 0;
      std::string BufferOut;
      TDataBuffer Data;
      unsigned long int uData;

      TailleReelle = TailleInitiale();

      if (!Statistiques)
         if (!Analyser())
            return false;

      if (!ArbreHuffman)
         if (!ConstruireArbreFreq())
            return false;

      if (!EcrireEntete())
         return false;

      // Compression du fichier source

      if (DegreVerbalite & ddEtape)
         FoncAffichageEtape("Compression en cours");
      if (sSource.length())
      {
         // Ouverture du fichier source
         fSrc.open(sSource.c_str(), std::ios::in | std::ios::binary);

         // Ouverture du fichier de destination
         if (fSrc.is_open())
         {
            if (sDestination.length())
              fDst.open(sDestination.c_str(), std::ios::app | std::ios::binary);
            else
            {
              sErrString = "Veuillez pr�ciser le nom du fichier de destination";
               return false;
            }
         }
         else
         {
              sErrString = "Impossible d'ouvrir le fichier d'entr�e en lecture";
               return false;
         }
      }
      else
      {
         sErrString = "Veuillez pr�ciser le nom du fichier source";
         return false;
      }

      if (fDst.is_open())
      {
         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("0%");
         // Parcours du fichier caract�re par caract�re
         // Les optimisations d'acc�s au fichier sont g�r�es par l'OS
         fSrc.read((char *)(&Valeur),sizeof(char));
         while (!fSrc.eof())
         {
            // insertion du code correspondant dans le buffer
            BufferOut += Statistiques[(unsigned char)(Valeur)].sCode;
            if (BufferOut.length() > 31)
            {
               // On dispose d'assez de donn�es pour �crire
               // Conversion du buffer de caract�res '1' et '0' en entier long
               Data = static_cast<TDataBuffer>(BufferOut.substr(0,32));
               uData = Data.to_ulong();
               // �criture des donn�es
               fDst.write((char *)(&uData),sizeof(unsigned long int));
               if (!fDst.good())
               {
                  sErrString = "Erreur lors de l'�criture des donn�es";
                  return false;
               }
               // lib�ration du buffer
               BufferOut.erase(0,32);
            }
            sprintf(BufferProg,"%d%%",int(float(Position) / TailleReelle * 100));
            if (DegreVerbalite & ddProgression)
               FoncAffichageProgression(BufferProg);
            Position++;
            fSrc.read((char *)(&Valeur),sizeof(char));
         }

         // vidage du reste du buffer
         Data.reset();
         BufferOut.resize(32,'0');
        Data = static_cast<TDataBuffer>(BufferOut);
         uData = Data.to_ulong();
         fDst.write((char *)(&uData),sizeof(unsigned long int));

         // Fermeture des fichier
         fSrc.close();
         fDst.close();
      }
      else
      {
         sErrString = "Impossible d'ouvrir le fichier de sortie en �criture";
         return false;
      }

      return true;
   }

   bool THuffman::Decompresser()
   {
      typedef std::bitset<32> TDataBuffer;
      std::ifstream fSrc;
      std::ofstream fDst;
      char Valeur;
      char BufferProg[10];
      unsigned long int Position = 0;
      TDataBuffer Data;
      unsigned long int uData;
      TNoeudHuffman * Pos;
      int i;


      if (!Statistiques)
         if (!LireEntete())
            return false;

      if (!ArbreHuffman)
         if (!ConstruireArbreCode())
            return false;

      // D�compression du fichier source
      if (DegreVerbalite & ddEtape)
         FoncAffichageEtape("D�compression en cours");
      if (sSource.length())
      {
         // Ouverture du fichier source
         fSrc.open(sSource.c_str(), std::ios::in | std::ios::binary);

         // Ouverture du fichier de destination
         if (fSrc.is_open())
         {
            if (sDestination.length())
              fDst.open(sDestination.c_str(), std::ios::out | std::ios::binary);
            else
            {
              sErrString = "Veuillez pr�ciser le nom du fichier de destination";
               return false;
            }
         }
         else
         {
              sErrString = "Impossible d'ouvrir le fichier d'entr�e en lecture";
               return false;
         }
      }
      else
      {
         sErrString = "Veuillez pr�ciser le nom du fichier source";
         return false;
      }

      if (fDst.is_open())
      {
         fSrc.seekg(FinEntete, std::ios::beg);
         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("0%");
         Pos = ArbreHuffman;
         // Parcours du fichier 4 octets par 4 octets
         // Les optimisations d'acc�s au fichier sont g�r�es par l'OS
         fSrc.read((char *)(&uData),sizeof(unsigned long int));
         while (!fSrc.eof())
         {

            // Conversion du buffer en bitset
            Data = static_cast<TDataBuffer>(uData);

            // Parcours du buffer pour d�coder les caract�res
            for (i = 31 ; (i >= 0) && (Position < TailleReelle) ; i--)
            {
               // Parcours de l'arbre
               if (Data.test(i))
                  Pos = Pos->FilsD;
               else
                  Pos = Pos->FilsG;

               if (!Pos)
               {
                  sErrString = "Impossible de lire dans l'arbre de Huffman";
                  return false;
               }

               if (!(Pos->FilsD || Pos->FilsG))
               {
                  // Feuille terminale : �criture de l'octet
                  fDst.write((char *)(&(Pos->Valeur)),1);
                  if (!fDst.good())
                  {
                     sErrString = "Erreur lors de l'�criture des donn�es";
                     return false;
                  }
                  // retour � la racine de l'arbre
                  Pos = ArbreHuffman;
                  Position++;
               }
            }

            sprintf(BufferProg,"%d%%",int(float(Position) / TailleReelle * 100));
            if (DegreVerbalite & ddProgression)
               FoncAffichageProgression(BufferProg);

            fSrc.read((char *)(&uData),sizeof(unsigned long int));
         }

         // Fermeture des fichier
         fSrc.close();
         fDst.close();
      }
      else
      {

         sErrString = "Impossible d'ouvrir le fichier de sortie en �criture";
         return false;
      }

      return true;
   }



   unsigned long int THuffman::TailleInitiale()
   {
      std::ifstream fSrc;
      unsigned long int TailleFichier;

      // Si le nom du fichier n'est pas d�fini ce n'est pas n�cessaire d'essayer
      // de l'ouvrir
      if (sSource.length())
         fSrc.open(sSource.c_str(), std::ios::in | std::ios::binary);
      else
      {
         sErrString = "Veuillez pr�ciser le nom du fichier source";
         return 0L;
      }

      if (fSrc.is_open())
      {
         // Calcul de la taille du fichier
         // (position fin de fichier - position d�but de fichier)
         fSrc.seekg(0, std::ios::end);
         TailleFichier = fSrc.tellg();
         fSrc.seekg(0, std::ios::beg);
         TailleFichier -= fSrc.tellg();
         fSrc.close();

         return TailleFichier;
      }
      else
      {
         sErrString = "Impossible d'ouvrir le fichier d'entr�e en lecture";
         return 0L;
      }
   }

   unsigned long int THuffman::TailleCompresse()
   {
      // Les entiers de 64 bits sont n�cessaires pour pouvoir faire le compte
      // puisque on compte en bits et que si la compression est nulle et le
      // fichier fait la taille maximale autoris�e par le syst�me de fichiers
      // (4 Go) les 32 bits des unsigned long int ne suffiront pas :(
      long long int TailleBits = 0;
      unsigned long int TailleEvaluee;

      if (Statistiques)
      {
         // Cumulation du nombre de bits pour chaque fragment
         for (int i=0 ; i < 256 ; i++)
         {
            TailleBits +=
                     Statistiques[i].Frequence * Statistiques[i].sCode.length();
         }
         // Conversion de bits en octets
         TailleEvaluee = static_cast<unsigned long int>(TailleBits / 8L);

         return TailleEvaluee;
      }
      else
      {

         sErrString = "L'analyse n'a pas �t� effectu�e";
         return 0L;
      }
   }

   unsigned long int THuffman::TailleFinale()
   {
      std::ifstream fDst;
      unsigned long int TailleFichier;

      if (bCompresse)
      {
         // Si le nom du fichier n'est pas d�fini ce n'est pas n�cessaire
         // d'essayer de l'ouvrir
         if (sDestination.length())
            fDst.open(sDestination.c_str(), std::ios::in | std::ios::binary);
         else
         {
            sErrString = "Veuillez pr�ciser le nom du fichier de destination";
            return 0L;
         }

         if (fDst.is_open())
         {
            // Calcul de la taille du fichier
            fDst.seekg(0, std::ios::end);
            TailleFichier = fDst.tellg();
            fDst.seekg(0, std::ios::beg);
            TailleFichier -= fDst.tellg();
            fDst.close();

            return TailleFichier;
         }
         else
         {

            sErrString = "Impossible d'ouvrir le fichier de sortie en lecture";
            return 0L;
         }
      }
      else
      {

         sErrString = "La compression n'a pas �t� effectu�e";
         return 0L;
      }
   }



   void THuffman::SetSource(const std::string &sSrc)
   {
      if (Statistiques)
      {
         // Suppression des dnn�es associ�es � l'ancien fichier source
         delete [] Statistiques;
         Statistiques = NULL;

         if (ArbreHuffman)
         {
            delete ArbreHuffman;
            ArbreHuffman = NULL;
         }
         bCompresse = false;
      }

      sSource = sSrc;
   }

   void THuffman::SetDestination(const std::string &sDst)
   {
      // On consid�re que la compression n'a pas eu lieu
      bCompresse = false;
      sDestination = sDst;
   }

   void THuffman::SetDegreDialogue(const TDegreDialogue v)
   {
      DegreVerbalite = v;
   }

   void THuffman::SetFonctionProgression(TFoncMessage Progres)
   {
      // Si l'adresse de la fonction est NULL alors on utilise la fonction par
      // d�faut instanci�e par la classe
      if (!Progres)
         Progres = THuffman::AfficherMessage;
      FoncAffichageProgression = Progres;
   }

   void THuffman::SetFonctionEtape(TFoncMessage Etape)
   {
      // Si l'adresse de la fonction est NULL alors on utilise la fonction par
      // d�faut instanci�e par la classe
      if (!Etape)
         Etape = THuffman::AfficherMessage;
      FoncAffichageEtape = Etape;
   }



   std::string THuffman::GetSource() const
   {
      return sSource;
   }

   std::string THuffman::GetDestination() const
   {
      return sDestination;
   }

   TDegreDialogue THuffman::GetDegreDialogue() const
   {
      return DegreVerbalite;
   }



   unsigned int THuffman::GetDerniereErreur(std::string &sMessage) const
   {
      sMessage = sErrString;
      return ErrCode;
   }



   void THuffman::AfficherMessage(const std::string &sMessage)
   {
      // Affichage sur la console par d�faut
      std::cout << sMessage << std::endl;
   }




   // Fonction prot�g�es
   bool THuffman::EcrireEntete()
   {
      std::ofstream fDst;
      unsigned long int ULongValue;
      unsigned short int UShortValue;
      unsigned char UCharValue;
      std::string sNomFichier;
      unsigned long int DernierSeparateur;
      char BufferProgression[10];


      if (DegreVerbalite & ddEtape)
         FoncAffichageEtape("Ecriture de l'entete");

      if (sSource.length())
      {
         if (sDestination.length())
            fDst.open(sDestination.c_str(), std::ios::out | std::ios::binary);
         else
         {
            sErrString = "Veuillez pr�ciser le nom du fichier de destination";
            return false;
         }
      }
      else
      {
         sErrString = "Veuillez pr�ciser le nom du fichier source";
         return false;
      }

      if (fDst.is_open())
      {
         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("0%");
         // Ecriture de la taille du fichier en octets
         ULongValue = TailleInitiale();
         fDst.write((char *)(&ULongValue),sizeof(unsigned long int));
         if (!fDst.good())
         {
            sErrString = "Erreur lors de l'�criture de l'ent�te";
            return false;
         }

         // Ecriture du nom du fichier original sans le chemin
         DernierSeparateur = sSource.rfind(SEPARATEUR_REPERTOIRE);
         if (DernierSeparateur > sSource.length())
            // Il n'y a pas le chemin
            sNomFichier = sSource;
         else
            sNomFichier = sSource.substr(DernierSeparateur + 1);

         if (sSource.empty())
         {
            sErrString = "Veuillez pr�ciser le fichier d'entr�e";
            return false;
         }


         // Ecriture de la taille du nom du fichier original
         UCharValue = sNomFichier.length();
         fDst.write((char *)(&UCharValue),sizeof(char));
         if (!fDst.good())
         {
            sErrString = "Erreur lors de l'�criture de l'ent�te";
            return false;
         }

         fDst.write(sNomFichier.c_str(),UCharValue);
         if (!fDst.good())
         {
            sErrString = "Erreur lors de l'�criture de l'ent�te";
            return false;
         }

         // Ecriture du nombre d'�l�ments diff�rents
         fDst.write((char *)(&NombreElements),sizeof(unsigned short int));
         if (!fDst.good())
         {
            sErrString = "Erreur lors de l'�criture de l'ent�te";
            return false;
         }

         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("1%");
         for (int i = 0 ; i < 256 ; i++)
         {
            // Si le caract�re a une fr�quence nulle on passe au suivant
            if (!Statistiques[i].Frequence)
               continue;

            // Ecriture du code du caract�re
            UCharValue = i;
            fDst.write((char *)(&UCharValue),sizeof(char));
            if (!fDst.good())
            {
               sErrString = "Erreur lors de l'�criture de l'ent�te";
               return false;
            }

            // Ecriture du nombre de bits du caract�re
            UCharValue = Statistiques[i].sCode.length();
            fDst.write((char *)(&UCharValue),sizeof(char));
            if (!fDst.good())
            {
               sErrString = "Erreur lors de l'�criture de l'ent�te";
               return false;
            }

            if (UCharValue <= 8)
            {
               // Ecriture du code dans un espace de 8 bits
               UCharValue = 0;
               for (int j = 0 ; j < Statistiques[i].sCode.length() ; j++)
                  if (Statistiques[i].sCode[j] == '1')
                     UCharValue |= (0x01 << (7 - j));
               fDst.write((char *)(&UCharValue),sizeof(char));
               if (!fDst.good())
               {
                  sErrString = "Erreur lors de l'�criture de l'ent�te";
                  return false;
               }
            }
            else
            {
               // Ecriture du code dans un espace de 16 bits
               UShortValue = 0;
               for (int j = 0 ; j < Statistiques[i].sCode.length() ; j++)
                  if (Statistiques[i].sCode[j] == '1')
                     UShortValue |= (0x01 << (15 - j));
               fDst.write((char *)(&UShortValue),sizeof(unsigned short int));
               if (!fDst.good())
               {
                  sErrString = "Erreur lors de l'�criture de l'ent�te";
                  return false;
               }
            }
            sprintf(BufferProgression,"%d%%",(((i*99)/256) + 1));
            if (DegreVerbalite & ddProgression)
               FoncAffichageProgression(BufferProgression);
         }

         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("100%");
         fDst.close();
         return true;
      }
      else
      {
         sErrString = "Impossible d'ouvrir le fichier de sortie en �criture";
         return false;
      }
   }


   bool THuffman::LireEntete()
   {
      std::ifstream fSrc;
      unsigned short int UShortValue;
      unsigned char UCharValue;
      char * buffer;
      int Elt;
      int NbBits;
      char BufferProgression[10];


      if (DegreVerbalite & ddEtape)
         FoncAffichageEtape("Lecture de l'ent�te");

      if (sSource.length())
         fSrc.open(sSource.c_str(), std::ios::in | std::ios::binary);
      else
      {
         sErrString = "Veuillez pr�ciser le nom du fichier source";
         return false;
      }

      if (fSrc.is_open())
      {
         // r�initialisation des statistiques
         if (Statistiques)
            delete [] Statistiques;

         Statistiques = new TStatistiqueHuffman[256];

         // Lecture de la taille du fichier en octets
         fSrc.read((char *)(&TailleReelle),sizeof(unsigned long int));
         if (!fSrc.good())
         {
            sErrString = "Erreur lors de la lecture de l'ent�te";
            return false;
         }

         // Lecture de la taille du nom du fichier original
         fSrc.read((char *)(&UCharValue),sizeof(char));
         if (!fSrc.good())
         {
            sErrString = "Erreur lors de la lecture de l'ent�te";
            return false;
         }

         // Cr�ation du buffer pour la lecture du nom du fichier
         buffer = new char[UCharValue+1];

         // Lecture du nom du fichier original
         fSrc.read(buffer,UCharValue);
         if (!fSrc.good())
         {
            sErrString = "Erreur lors de la lecture de l'ent�te";
            return false;
         }

         // Zero terminal
         buffer[UCharValue] = 0;
         if (sDestination.empty())
            sDestination = buffer;

         // Lecture du nombre d'�l�ments diff�rents
         fSrc.read((char *)(&NombreElements),sizeof(unsigned short int));
         if (!fSrc.good())
         {
            sErrString = "Erreur lors de la lecture de l'ent�te";
            return false;
         }

         for (int i = 0 ; i < NombreElements ; i++)
         {
            // Lecture du code du caract�re
            fSrc.read((char *)(&UCharValue),sizeof(char));
            if (!fSrc.good())
            {
               sErrString = "Erreur lors de la lecture de l'ent�te";
               return false;
            }
            Elt = UCharValue;

            // Lecture du nombre de bits du caract�re
            fSrc.read((char *)(&UCharValue),sizeof(char));
            if (!fSrc.good())
            {
               sErrString = "Erreur lors de la lecture de l'ent�te";
               return false;
            }
            NbBits = UCharValue;

            if (NbBits <= 8)
            {
               // Lecture du code dans un espace de 8 bits
               fSrc.read((char *)(&UCharValue),sizeof(char));
               if (!fSrc.good())
               {
                  sErrString = "Erreur lors de la lecture de l'ent�te";
                  return false;
               }
               for (int j = 0 ; j < NbBits ; j++)
                  Statistiques[Elt].sCode += (UCharValue & (0x01 << (7 - j))) ?
                                                               "1" : "0";
            }
            else
            {
               // Lecture du code dans un espace de 16 bits
               fSrc.read((char *)(&UShortValue),sizeof(unsigned short int));
               if (!fSrc.good())
               {
                  sErrString = "Erreur lors de la lecture de l'ent�te";
                  return false;
               }
               for (int j = 0 ; j < NbBits ; j++)
                  Statistiques[Elt].sCode += (UShortValue & (0x01 << (15 - j)))
                                                             ? "1" : "0";
            }

            sprintf(BufferProgression,"%d%%",(((Elt*99)/256) + 1));
            if (DegreVerbalite & ddProgression)
               FoncAffichageProgression(BufferProgression);
         }     

         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression("100%");
         FinEntete = fSrc.tellg();
         fSrc.seekg(0, std::ios::beg);
         FinEntete -= fSrc.tellg();
         fSrc.close();

         return true;
      }
      else
      {

         sErrString = "Impossible d'ouvrir le fichier d'entr�e en lecture";
         return false;
      }
   }


   bool THuffman::ConstruireArbreFreq()
   {
      typedef PComparateur<TNoeudHuffman> PCompHuffman;
      PCompHuffman NoeudA;
      PCompHuffman NoeudB;
      // Cr�ation d'une file � priorit� class�e en ordre inverse
      std::priority_queue<PCompHuffman,
                          std::deque<PCompHuffman>,
                          std::greater<PCompHuffman> > FilePrioNoeuds;

      if (!Statistiques)
      {
         sErrString = "L'analyse n'a pas �t� effectu�e";
         return false;
      }

      // Cr�ation de la liste initiale
      for (int i = 0 ; i < 256 ; i++)
         if (Statistiques[i].Frequence)
         {
            // On ne consid�re que les fragments qui apparaissent
        FilePrioNoeuds.push(PCompHuffman(Statistiques[i].CreerNoeudHuffman(i)));
         }

      if (FilePrioNoeuds.empty())
      {
         sErrString = "Fichier vide";
         return false;
      }

      NombreElements = FilePrioNoeuds.size();

      // G�n�ration de l'arbre de Huffman
      while (FilePrioNoeuds.size() > 1)
      {
         NoeudA = FilePrioNoeuds.top();
         FilePrioNoeuds.pop();
         NoeudB = FilePrioNoeuds.top();
         FilePrioNoeuds.pop();

         FilePrioNoeuds.push(PCompHuffman(new TNoeudHuffman(0,
                                                      NoeudA.get()->Frequence
                                                      + NoeudB.get()->Frequence,
                                                      NULL,
                                                      NoeudA.get(),
                                                      NoeudB.get())));
      }

      // D�termination de la racine et vidage de la file � priorit�
      ArbreHuffman = FilePrioNoeuds.top().get();
      FilePrioNoeuds.pop();

      // D�termination des codes de Huffman
      ArbreHuffman->DeterminerCodeHuffman("");

      return true;

   }


   bool THuffman::ConstruireArbreCode()
   {
      char BufferProg[10];
      if (DegreVerbalite & ddEtape)
         FoncAffichageEtape("Cr�ation de l'arbre de d�codage");
      if (DegreVerbalite & ddProgression)
         FoncAffichageProgression("0%");
      // Cr�ation de la racine
      ArbreHuffman = new TNoeudHuffman();
      for (int i = 0 ; i <  256 ; i++)
      {
         if (Statistiques[i].sCode.length())
         {
            ArbreHuffman->CreerFeuille(Statistiques[i].sCode, i);
         }
         sprintf(BufferProg, "%d%%", (i * 100)/256);
         if (DegreVerbalite & ddProgression)
            FoncAffichageProgression(BufferProg);
      }
      return true;
   }

}

int main(int argc, char* argv[])
{
   std::string msg;
   Huffman::THuffman comp("../test/test_entree", "../test/tmp_compresse", ddEtape);

   comp.Compresser();

   comp.GetDerniereErreur(msg);

   std::cout << msg;

   Huffman::THuffman dec("../test/tmp_compresse","../test/test_decommpresse", ddEtape);

   dec.Decompresser();

   dec.GetDerniereErreur(msg);

   std::cout << msg;

  
   return 0;
}

