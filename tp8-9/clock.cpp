#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char outstr[256];
  time_t t;
  struct tm *lt;

  printf("En ce moment, il est : \n");

  while(1) {
    t = time(NULL);
    if( (lt = localtime(&t)) == NULL) {
      perror("localtime");
      exit(EXIT_FAILURE);
    }

    if(strftime(outstr, sizeof(outstr), "%T", lt) == 0) {
      fprintf(stderr, "strftime");
      exit(EXIT_FAILURE);
    }

    printf("\r%s", outstr);
    fflush(stdout);
    sleep(1);
  }

  exit(EXIT_SUCCESS);
}

