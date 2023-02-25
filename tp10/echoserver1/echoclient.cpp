#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include "socklib.h"
#include "recv_line.h"

using namespace std;

void print_usage(char *name) {
  cout << "usage: " << name << " client host port" << endl;
}

int main(int argc, char *argv[]) {
  char *host = NULL;
  char *port = NULL;
  
  if(argc < 3) {
    print_usage(argv[0]);
    return(1);
  }

  host = argv[1];
  port = argv[2];
  
  // TODO1
  int sd = 0;

  cout << "Logiquement, on est connecté au serveur." << endl;

  string line;
  while(1) {
    // Réception d'une ligne sur la socket, puis affichage.
    // TODO2

    // Lecture d'une ligne sur l'entrée standard, et envoi sur la socket.
    getline(cin, line); // ATTENTION, getline() *enlève* le '\n' de fin de ligne !
    // TODO3
    // TODO4
  }

  cout << "Fermeture de la connexion." << endl;

  close(sd);

  return(0);
}

