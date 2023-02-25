#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "socklib.h"
#include "recv_line.h"

using namespace std;

// Affiche l'éventuel message d'erreur passé en argument,
// suivi du message donné par la variable errno.
void print_error(const string &msg = "");

// Affiche l'éventuel message d'erreur passé en argument,
// avec print_error(), puis termine le processus appelant
// avec pour valeur de retour EXIT_FAILURE.
void exit_error(const string &msg = "");

// ====================================================================
// ====================================================================

void exit_usage(const string &name) {
  cerr << "Usage : " << name << " port" << endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  char *port = NULL;
  int res;
  
  if(argc < 2) exit_usage(string(argv[0]));
  port = argv[1];

  cout << "On utilise le port " << port << endl;

  int s = create_server_socket(port);
  if(s == -1) exit_error("création socket d'écoute");
  
  int sd = accept_connection(s);
  if(sd == -1) exit_error("attente d'une connexion");

  printf("Une connexion vient d'être acceptée.\n");
  
  close(s); // la socket d'écoute n'est plus nécessaire

  res = send_str(sd, "Bonjour !\n");
  if(res == -1) exit_error("send_str");

  // A TERMINER
  string line;
  while(true){
    res = recv_line(sd, line, '\n');
    //cout<<"J'ai reçu "<<line<<endl;

    if(res == -1) { /*gestion d'erreur */}
    if (res == 0) { /* ... */}
    if (line == "quit") {
      res = send_str(sd, "au revoir\n");
      if(res==-1){ /* ges */}
      break; 
      close(sd);
    }
    
  }
  return(0);
}

// ====================================================================
// ====================================================================

void print_error(const string &msg) {
  if(msg == "") cerr << "Erreur : " << strerror(errno) << endl;
  else cerr << "Erreur, " << msg << " : " << strerror(errno) << endl;
}

void exit_error(const string &msg) {
  print_error(msg);
  exit(EXIT_FAILURE);
}

