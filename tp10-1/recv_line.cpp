#include "recv_line.h"

using namespace std;

// Reçoit une chaîne de caractères du type string sur la
// socket s, jusqu'à rencontrer le caractère c, qui par défaut
// est le caractère de retour à la ligne '\n'. Ce caractère est
// consommé sur la socket, et il est recopié dans la chaîne
// résultat. Retourne :
//  * -1 en cas d'échec,
//  *  0 si l'on n'a rien lu sur la socket car le descripteur
//     de fichier a été fermé par l'hôte distant,
//  * le nombre de caractères lus sinon.
int recv_line(int s, std::string &line, char c) {
  ssize_t nb_read;      // nombre de caractère lus à chaque recv
  ssize_t tot_read = 0; // nombre total de caractères lus
  char t;               // pour recevoir un caractère

  line.clear();
  // A TERMINER
  
  do {
    nb_read = recv(s, &t, 1, 0);
    if (nb_read == 1) {

      //gestion d'erreurs
    }
    if (nb_read == 0) break;
    line.push_back(t);
    tot_read++;
  } while (t!= c);
  return tot_read;
}

