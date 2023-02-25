#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void die(void) {
  cerr << "Erreur : " << strerror(errno) << endl << flush;
  exit(EXIT_FAILURE);
}

int main(void) {
  int rf, p[2];
  int i, r, n;

  pipe(p); // initialisation du pipe p

  rf = fork(); // création d'un fils

  if(rf > 0) { // processus père

    close(p[0]);
    for(i = 1; i < 100; i++) {
      write(p[1], &i, sizeof(int));
      cout << "(père) " << i << endl << flush;
    }
    close(p[1]); // <-- décommenter pour fermer p[1] (descripteur pour l'écriture)
    if(waitpid(rf, NULL, 0) == -1) die();
    cout << "(père) terminaison normale" << endl << flush;

  }
  else { // processus fils
    
    close(p[1]); // <-- décommenter pour fermer p[1] (descripteur pour l'écriture)
    r = read(p[0], &n, sizeof(int));
    while(r == sizeof(int)) {
      cout << "(fils) " << n << endl << flush;
      r = read(p[0], &n, sizeof(int));
    }
    if (r == -1) die();
    close(p[0]);
    cout << "(fils) terminaison normale" << endl << flush;

  }

  return EXIT_SUCCESS;
}

