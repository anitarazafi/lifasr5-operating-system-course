#include <signal.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cerrno>

int nb = 25; // variable globale, initialisée à 25

// Fonction à appeler régulièrement
void top(int s) { // le numéro s du signal n'est pas utilisé ici
  if(nb % 5 == 0)
    std::cout << "+" << std::flush;
  else
    std::cout << "." << std::flush;
  nb--;
}

int main(int argc, char* argv[]) {
  struct sigaction sa;
  //TODO 1: installer un gestionnaire pour le signal SIGALRM, qui doit lancer top
  sigaction(SIGALRM, NULL, &sa); 
  sa.sa_handler = &top;
  sigaction(SIGALRM, &sa, NULL);
  alarm(1);
  while (1) {
    pause();
    if(nb == 0) break;
    //TODO 2: programmer une alarme 
    alarm(1); 
  }
  std::cout << std::endl << std::flush;
  return 0;
}

