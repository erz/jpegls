#---------------------------------------------------------------
# Nom du fichier : makefile
# Auteur : Aurelien FIOL & Florent MAS
# Objet : Commandes de compilation du programme de compression.
# Pour generer l'executable : tapez "make" ou "make Executable"
# Pour effacer les fichiers objet et l'executable : tapez "make clean"
#--------------------------------------------------------------------

CC = g++
CFLAGS = -Wall 
LDFLAGS = -lm -Wall 
exec = Compression

all: $(exec)

$(exec): main.o Fichiers.o Conversions.o Compression.o
	$(CC) -o $(exec) main.o Fichiers.o Conversions.o Compression.o $(LDFLAGS)	

Fichiers.o: Fichiers.c Fichiers.h
	$(CC) -o Fichiers.o -c Fichiers.c $(CFLAGS)
	
Compression.o: Compression.c Compression.h
	$(CC) -o Compression.o -c Compression.c $(CFLAGS)	

Conversions.o: Conversions.c Conversions.h
	$(CC) -o Conversions.o -c Conversions.c $(CFLAGS)	

main.o: main.c
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:	
	rm -rf *.o

mrproper: clean
	rm -rf $(exec)

#---------------------fin makefile--------------------------
