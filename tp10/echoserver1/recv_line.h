#ifndef __RECV_LINE__
#define __RECV_LINE__

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <string>
#include <iostream>

// Reçoit une chaîne de caractères du type string sur la
// socket s, jusqu'à rencontrer le caractère c, qui par défaut
// est le caractère de retour à la ligne '\n'. Ce caractère est
// consommé sur la socket, et il est recopié dans la chaîne
// résultat. Retourne :
//  * -1 en cas d'échec,
//  *  0 si l'on n'a rien lu sur la socket car le descripteur
//     de fichier a été fermé par l'hôte distant,
//  * le nombre de caractères lus sinon.
int recv_line(int s, std::string &line, char c = '\n');

#endif
