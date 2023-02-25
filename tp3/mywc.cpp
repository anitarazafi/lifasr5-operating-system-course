#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>	

using namespace std;

/* Exercice wc, fichiers, LIFASR5 2020, N. Louvet, L.Gonnord*/

// Fonction de documentation utilisateur
void print_usage(char *name) {
  cerr << name << ": count the number of bytes in a file" << endl;
  cerr << "usage: " << name <<" filename" << endl;
}

int main(int argc, char *argv[]) {
  int fd; // descripteur de fichier
  char c; // servira ici de buffer
  int nbrd; // stockera la valeur de retour de l'appel à read.
  int  nbbytes= 0;

  if(argc < 2) {
    print_usage(argv[0]);
    return -1;
  }

  // REMPLIR ICI
  // TODO : Ouvrir le fichier en lecture (read only)
  fd = open(argv[1], O_RDONLY)

  //

  // TODO : n'oubliez pas de fermer le fichier à la fin.
  close(fd);

  cout << "Number of bytes = " << nbbytes << endl;
  
  return 0;
}