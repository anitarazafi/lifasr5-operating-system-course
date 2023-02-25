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
#include <signal.h>

#include "socklib.h"

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

void dial(int sd) {
  // Lors d'un appel à la fonction dial(), on souhaite que le signal
  // SIGTERM soit bloqué : cela signifie que s'il est envoyé au processus
  // alors qu'il est en train d'exécuter cette fonction, le gestionnaire
  // de fonction pour SIGTERM ne sera pas appelé avant que l'on sorte de
  // la fonction. C'est le rôle de l'appel à sigprocmask() ci-dessous.
  sigset_t mask;
  sigemptyset(&mask);
  if(sigaddset(&mask, SIGTERM) == -1) exit_error();
  if(sigprocmask(SIG_BLOCK, &mask, NULL) == -1) exit_error();
  
  string line;
  int res;
  
  res = send_str(sd, "Bonjour !\n");
  if(res == -1) exit_error("send_str");

  while(true) {
    res = recv_line(sd, line, '\n');
    
    if(res == -1) {
      print_error();
      break;
    }

    if(res == 0) {
      cout << "Connexion fermée du côté du client..." << endl;
      break;
    }
    
    if(!line.empty() && line.back() == '\n') line.pop_back();
    if(!line.empty() && line.back() == '\r') line.pop_back();

    cout << "Serveur : \"" << line << "\"" << endl << flush;
    
    if(line == "quit") {
      res = send_str(sd, "Au revoir, je ferme la connexion...\n");
      if(res == -1) exit_error("send_str");
      break;
    }
    
    line = "Vous avez envoyé \"" + line + "\"\n";
    res = send_str(sd, line);
    if(res == -1) exit_error("send_str");
  }
  
  close(sd); // fermeture de la socket de dialogue
  
  // On débloque le signal SIGTERM pour le processus courant.
  if(sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) exit_error();
  // Si le signal a été envoyé pendant l'exécution de la fonction
  // dial(), il sera pris en compte.
}

// ====================================================================
// ====================================================================

void exit_usage(const string &name) {
  cerr << "Usage : " << name << " port" << endl;
  exit(EXIT_FAILURE);
}

bool quit = false;

void handler(int sig) {
  quit = true;
}

int main(int argc, char *argv[]) {
  char *port = NULL;
  
  if(argc < 2) exit_usage(string(argv[0]));
  port = argv[1];

  struct sigaction sa;
  if(sigaction(SIGTERM, NULL, &sa) == -1) exit_error();
  sa.sa_handler = handler;
  if(sigaction(SIGTERM, &sa, NULL) == -1) exit_error();

  cout << "serveur : le PID du serveur est " << getpid() << endl;
  cout << "Serveur : on utilise le port " << port << endl;

  int s = create_server_socket(port);
  if(s == -1) exit_error("création socket d'écoute");
  
  int sd = accept_connection(s);
  if(sd == -1) exit_error("attente d'une connexion");

  printf("Serveur : une connexion vient d'être acceptée.\n");
  
  close(s); // la socket d'écoute n'est plus nécessaire

  dial(sd); // dialogue avec le client

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

