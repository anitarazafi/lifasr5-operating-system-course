#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sys/wait.h>

using namespace std;

int main(void) {
  int pid;
  int a = 0;

  cout << "Je suis le père, de PID " << getpid() << ". "
       << "Je vais créer un fils..." << endl << flush;

  pid = fork();

  if(pid == -1) {
    cerr << "Erreur, aucun fils n'a été créé : " << strerror(errno) << "."
    << endl << flush;
    return 1;
  }

  if(pid > 0) {
    cout << "Je suis le père, de PID " << getpid() << ". "
         << "J'ai un fils dont le PID est " << pid << "."
         << endl << flush;
  }
  else {
    cout << "Je suis le fils, de PID " << getpid() << ". "
         << " Mon père a pour PID " << getppid() << "." 
         << endl << flush;
  }

  cout << "Je suis le processus de PID " << getpid() << ". " 
       << "Mon père a le PID " << getppid() << "."
       << endl << flush;                                     

  if(pid > 0) {
    waitpid(0, NULL, WNOHANG);
    cout << "Je suis le père, de PID " << getpid() << ". "
         << "Je vais me terminer..." << endl << flush;
  }
  else {
    cout << "Je suis le fils, de PID " << getpid() << ". "
         << "Mon père a pour PID " << getppid() << ". "
         << "Je vais me terminer..." << endl << flush;
  }

  cout << "Variable a : " << a << endl << flush;

  return 0;                                                  
}

