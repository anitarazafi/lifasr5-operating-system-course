#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include "socklib.h"

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
  sd = create_client_socket(host,port); 
  /* errereu... */

  cout << "Logiquement, on est connecté au serveur." << endl;

  string line;
  while(1) {
    // Réception d'une ligne sur la socket, puis affichage.
    // TODO2
  recv_line(sd, line, '\n');
  cout<<line<<endl; 
    // Lecture d'une ligne sur l'entrée standard, et envoi sur la socket.
    getline(cin, line); // ATTENTION, getline() *enlève* le '\n' de fin de ligne !
    // TODO3
    if(line=="quit") {
      cout << "fin discussion" <<endl;
      send_str(sd, "quit\n");
      break;
    }
    // TODO4
    line.push_back('\n');
    send_str(sd, line);
  }

  cout << "Fermeture de la connexion." << endl;

  close(sd);

  return(0);
}

