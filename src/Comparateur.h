template<class T>
class PComparateur
{
   private :
      T * pointeur;

   public :
      inline explicit PComparateur(T *p = NULL) : pointeur(p) {}
      inline explicit PComparateur(T &p) : pointeur(&p) {}

      inline T * get() const { return pointeur; }
      inline void set(T *p) { pointeur = p; }
      inline void set(T &p) { pointeur = &p; }

      friend bool operator<(const PComparateur<T> &A, const PComparateur<T> &B)
         {
            return *(A.pointeur) < *(B.pointeur);
         }
      friend bool operator>(const PComparateur<T> &A, const PComparateur<T> &B)
         {
            return *(A.pointeur) > *(B.pointeur);
         }
      friend bool operator<=(const PComparateur<T> &A, const PComparateur<T> &B)
         {
            return *(A.pointeur) <= *(B.pointeur);
         }
      friend bool operator>=(const PComparateur<T> &A, const PComparateur<T> &B)
         {
            return *(A.pointeur) >= *(B.pointeur);
         }
      friend bool operator==(const PComparateur<T> &A, const PComparateur<T> &B)
         {
            return *(A.pointeur) == *(B.pointeur);
         }

      friend std::ostream &operator<<(std::ostream &out, const PComparateur<T> &B)
         {
            return out << *(B.pointeur);
         }
      friend std::istream &operator>>(std::istream &in, const PComparateur<T> &B)
         {
            return in >> *(B.pointeur);
         }
};

