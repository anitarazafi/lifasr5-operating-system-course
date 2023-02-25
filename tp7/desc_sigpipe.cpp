#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> 

using namespace std;


// Messages d'erreur
void die(void) {
  cerr << "Erreur : " << strerror(errno) << endl;
  exit(EXIT_FAILURE);
}

// Gestionnaire du signal SIGPIPE
void gest_sigpipe(int s){
  cout << "signal "  << s << " reçu par le processus " << getpid() << endl; 
}

int main(void) {
  int rf, p[2];

  pipe(p);

  rf = fork();

  // Mise en place d'un gestionnaire pour le signal SIGPIPE.
  struct sigaction act;
  sigaction(SIGPIPE, NULL, &act); // On récupére la structure sigaction actuelle pour SIGPIPE.
  act.sa_handler = &gest_sigpipe; // On remplace dans act le gestionnaire de signal par le notre.
  sigaction(SIGPIPE, &act, NULL); // On installe le nouveau gestionnaire de signal.
    
  if(rf > 0) { // processus père

    cout << "(père) mon PID est "<< getpid() << endl << flush;
    cout << "(père) le PID de mon fils est "<< rf << endl << flush;
    close(p[0]);
    int wr;
    for (int i = 1; i < 100; i++){
      wr = write(p[1], &i, sizeof(int));
      if(wr == -1) die();
      cout << "(père) " << i << endl << flush;
    }
    close(p[1]); 
    if( waitpid(rf, NULL, 0) == -1) die();
    cout << "(père) terminaison normale." << endl << flush;

  }
  else { // processus fils
    cout << "(fils) mon PID est " << getpid() << endl << flush;
    close(p[1]); 

    // Lecture d'un seul entier sur le pipe
    int rd, n;
    rd = read(p[0], &n, sizeof(int));
    if(rd == -1) die();
    cout << "(fils) j'ai lu " << n << endl << flush;

    // Le fils ferme tout de suite son descripteur de fichier en lecture,
    // alors que le père écrit encore de l'autre côté !
    close(p[0]);
    cout << "(fils) terminaison normale." << endl << flush;
  }

  return EXIT_SUCCESS;
}


