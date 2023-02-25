#ifndef __SOCKLIB__
#define __SOCKLIB__

#include <sys/types.h>
#include <sys/socket.h>
#include <string>

// Tente de créer une socket côté serveur (localhost),
// à l'écoute sur toutes les interfaces, sur le port
// passé en paramètre.
// Affiche tout un tas de messages sur stderr.
// Retourne la socket crée en cas de succès,
// -1 en cas d'échec.
int create_server_socket(const char* port);

// Côté serveur, se met en attente bloquante d'une connexion
// sur la socket d'écoute s.
// Affiche tout un tas de messages sur stderr.
// Retourne la socket de dialogue crée en cas de succès,
// -1 en cas d'échec.
int accept_connection(int s);

// Tente de créer une socket côté client, en essayant de
// se connecter sur l'host et au port passés en paramètres.
// Retourne la socket crée en cas de succès,
// -1 en cas d'échec.
int create_client_socket(const char *host, const char* port);

// Reçoit une chaîne de caractères du type string sur la
// socket s, jusqu'à rencontrer le caractère c, qui par défaut
// est le caractère de retour à la ligne '\n'. Ce caractère est
// consommé sur la socket, et il n'est pas recopié dans la chaîne
// résultat. Retourne :
//  * -1 en cas d'échec,
//  *  0 si l'on n'a rien lu sur la socket car le descripteur
//     de fichier a été fermé par l'hôte distant,
//  * le nombre de caractères lus sinon.
int recv_line(int s, std::string &line, char c = '\n');

// Envoi d'une chaîne de caractères du type string sur la
// socket s. L'utilisateur doit prévoir un caractère spécial
// pour marquer la fin de chaîne à la réception ('\n' par
// exemple).  Retourne 0 en cas de succès, -1 en cas d'échec.
int send_str(int s, const std::string &str);

// Récupère le nom et le port utilisé par le processus à l'autre
// bout de la socker.
void get_peer_info(int sock, std::string &adresse, std::string &port);



#endif
