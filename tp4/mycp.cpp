#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>	

/* Copie d'un fichier avec les primitives systèmes open, read, write*/
/* Nicolas Louvet, Laure Gonnord pour LIFASR5, 2019/20*/

using namespace std;

#define LEN 20

// Fonction d'aide en cas de mauvais appel du programme
void print_usage(const string &name) {
  cerr << "Usage : " << name << " source destination" << endl;
}

int main(int argc, char *argv[]) {
  char buf[LEN], *p; //tampon
  int fdin, fdout; //descripteur de fichiers fdin: lecture, fdout: ecriture
  int nbrd, nbwr, nbrem;
  
  if(argc < 3) { // Gestion de la ligne de commande (ldc).
    print_usage(argv[0]);
    return 1;
  }

  // Ouverture du fichier source dont le nom est argv[1]
  if((fdin = open(argv[1], O_RDONLY)) < 0) {
    cerr << "Erreur : " << strerror(errno) << endl;
    return 1;
  }

  // ouverture du fichier destination dont le nom est argv[2]
  // man open (section "Linux Programmer's Manual") pour les options
  if((fdout = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
    cerr << "Erreur : " << strerror(errno) << endl;
    close(fdin);
    return 1;
  }
  
  // MODIFIER ICI
  do {
  nbrd = read(fdin, buf, LEN);
    nbrem = nbrd; 
    if(nbrd == -1) {
      cerr<<"Erreur : "<<strerror(errno)<<endl;
      return 1;
    }
    p = buf; 
    while(nbrem>0){
      nbwr = write(fdout, p, nbrem);
      if(nbwr == -1) {
        cerr<<"Erreur : "<<strerror(errno)<<endl;
        return 1;
      }
      nbrem-=nbwr;
      p+=nbwr; 
    }
  } while (nbrd>0); 

  close(fdin);
  close(fdout);
  
  return EXIT_SUCCESS;
}