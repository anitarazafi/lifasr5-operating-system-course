#include "socklib.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int create_server_socket(const char* port) {
  int s = -1;                // la socket que l'on essaie d'obtenir
  struct addrinfo hints;     // structure pour faire la demande de port
  struct addrinfo *addr, *p; // pointeurs pour parcourir les résultats de getaddrinfo
  int res;                   // gestion des erreurs
  int optval = 1;            // argument de setsockopt
  char hname[NI_MAXHOST];    // pour récupérer le nom d'hôte
  char sname[NI_MAXSERV];    // pour récupérer le nom de service

  std::cerr << __func__ << ": tentative de création d'une socket serveur sur le port " << port << std::endl;

  // on initialise la structure hints permettant de demander
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;     // IPv4 ou IPv6
  hints.ai_socktype = SOCK_STREAM; // socket flux connectée
  hints.ai_flags = AI_PASSIVE;     // toutes les addresses de la machine seront utilisables
  hints.ai_protocol = 0;           // n'importe quel protocole

  // on effectue la demande pour le port demandé
  res = getaddrinfo(NULL, port, &hints, &addr);
  if (res != 0) { // cas d'erreur
    std::cerr << __func__ << ", getaddrinfo: << gai_strerror(res)" << std::endl;
    return(-1);
  }

  // si res = 0, alors addr pointe désormais vers le premier
  // élément d'une liste chaînée de struct addrinfo, et le chaînage
  // est établie par le champ ai_next ; il faut parcourir la liste
  // d'adresses jusqu'à en trouver une que l'on arrive à associer à
  // une socket avec bind (cf. man getaddrinfo)

  p = addr;           // on utilise p pour parcourir la liste
  while (p != NULL) { // on parcourt la liste
    // tentative de création de la socket serveur
    s = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
    if (s != -1) { // la création de la socket a fonctionné
      std::cerr << __func__ << ": la socket a été créée" << std::endl;

      // on souhaite pouvoir réutiliser la socket (optval = 1);
      res = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
      if (res == -1) {
        std::cerr << __func__ << ", setsockopt: " << strerror(errno) << std::endl;
        close(s);
        return(-1);
      }

      // on essaye de réserver le port
      res = bind(s, p->ai_addr, p->ai_addrlen);
      if(res == 0 ) break; // cela a fonctionné, la socket est à l'écoute
      else { // le bind a échoué, il faut fermer la socket et continuer
        std::cerr << __func__ << ": impossible de lui attribuer l'adresse courante" << std::endl;
        close(s);
      }
    }
    p = p->ai_next; // on passe à l'adresse suivante
  }

  // si p == NULL, c'est que l'on est sorti de la boucle sans réussir la création
  if(p == NULL) {
    std::cerr << __func__ << ": impossible de réserver le port " << port << "  demandé, abandon définitif" << std::endl;
    return(-1);
  }

  // on tente de mettre la socket à l'écoute de connexions entrantes
  res = listen (s, 5);
  if(res == -1) {
    std::cerr << __func__ << ", listen: " << strerror(errno) << std::endl;
    close(s);
    return(-1);
  }

  // on récupère des informations affichables sur l'adresse utilisée
  res = getnameinfo(p->ai_addr, p->ai_addrlen,
		    hname, NI_MAXHOST,
		    sname, NI_MAXSERV,
		    NI_NUMERICSERV|NI_NUMERICHOST);
  if(res != 0) {
    std::cerr << __func__ << ", getnameinfo: " << gai_strerror(res) << std::endl;
    return(-1);
  }
  std::cerr << __func__ << ": la socket " << s << " est maintenant en attente à l'adresse " << hname << " sur le port " << sname << std::endl; 

  // on libère la structure devenue inutile
  freeaddrinfo(addr);

  return s;
}

int create_client_socket(const char *host, const char* port) {
  int s = -1;                // la socket que l'on essaie d'obtenir
  struct addrinfo hints;     // structure pour faire la demande de port
  struct addrinfo *addr, *p; // pointeurs pour parcourir les résultats de getaddrinfo
  int res;                   // pour la gestion des erreurs

  // on remplit la structure hints de demande d'adresse
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;     // IPv4 ou IPv6
  hints.ai_socktype = SOCK_STREAM; // socket flux connectée
  hints.ai_flags = 0;
  hints.ai_protocol = 0;           // pas de protocole particulier

  res = getaddrinfo(host, port, &hints, &addr);
  if (res != 0) { // c'est un cas d'erreur
    std::cerr << __func__ << ", getaddrinfo: " << gai_strerror(res) << std::endl;
    return(-1);
  }

  // il faut parcourir la liste d'adresses produites par getaddrinfo,
  // jusqu'à en trouver une à laquelle on arrive à se connecter avec
  // connect (cf. man getaddrinfo)

  p = addr;
  while(p != NULL) {
    // on essaye d'abord de créer la socket demandée
    s = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
    if(s != -1) { // la création de socket a réussi
      // on essaye de se connecter à l'hôte distant
      res = connect(s, p->ai_addr, p->ai_addrlen);
      if(res == 0 ) break;  // cela a fonctionné, on est connecté
      else close(s);        // le connect a échoué, on ferme cette socket
    }
    p = p->ai_next; // on passe à l'adresse suivante
  }

  // si p == NULL, c'est que l'on est sorti de la boucle sans réussir la connexion
  if(p == NULL) {
    std::cerr << __func__ << ": aucune connexion n'est possible" << std::endl;
    return(-1);
  }

  freeaddrinfo(addr);

  return(s);
}


int accept_connection(int s) {
  int sc;                       // socket de connexion pour dialoguer avec un éventuel client
  int res;                      // gestion des erreurs
  struct sockaddr_storage addr; // pour récupérer les infos sur le client
  char hname[NI_MAXHOST];       // pour récupérer le nom du client
  char sname[NI_MAXSERV];       // pour récupérer le nom de service
  socklen_t addr_len = sizeof(struct sockaddr_storage);

  // on reste en attente de la connexion d'un client
  sc = accept(s, (struct sockaddr*) &addr, &addr_len);
  if(sc == -1) {
    fprintf(stderr, "%s, accept: %s\n", __func__, strerror(errno));
    return -1;
  }

  res = getnameinfo((struct sockaddr*) &addr,
		    addr_len,
		    hname, NI_MAXHOST,
		    sname, NI_MAXSERV,
		    NI_NUMERICSERV);
  if(res!=0){
    fprintf(stderr, "%s, getnameinfo: %s\n", __func__, gai_strerror(res));
    return -1;
  }

  printf("%s: connexion depuis %s, depuis le port %s\n", __func__, hname, sname);

  return sc;
}

#define RECV_LINE_SIZE_BUF 16

int recv_line(int s, std::string &line, char c) {
  char buf[RECV_LINE_SIZE_BUF]; // buffer pour les recv
  ssize_t nb_read = 0;          // nombre de caractères lus par le recv de l'itération
  ssize_t nb_extr = 0;          // nombre de caractères à extraire du buffer
  ssize_t tot_read = 0;         // nombre total de caractères lus

  line.clear();

  bool found = false;
  while(!found) {
    nb_read = recv(s, buf, RECV_LINE_SIZE_BUF, MSG_PEEK);
    if(nb_read == -1) {
      std::cerr << "recv_line, 1st recv : " << strerror(errno) << std::endl;
      return -1;
    }

    // si nb_read == 0, la connexion a été fermée pas l'hôte distant
    // avant que l'on ne reçoive le caractère final ; mais tout ce
    // que l'on a pu lire est déjà dans line...
    if(nb_read == 0) break;

    // on calcule le nombre de caractères nb_extr à extraire, qui sera :
    // - soit nb_read si le buffer ne contient pas c,
    // - soit l'indice suivant le caractère c, et dans ce cas on fait
    //   passer found à true.
    for(nb_extr = 0; (nb_extr < nb_read) && (buf[nb_extr] != c); nb_extr++)
      line.push_back(buf[nb_extr]);
    if(nb_extr < nb_read) { // dans ce cas, buf[nb_extr] == c
      found = true;
      nb_extr++;
    }

    // nb_extr est le nombre de caractères que l'on doit recopier
    // du buffer vers line, et que l'on doit extraire de la socket.
    tot_read += nb_extr;
    if(recv(s, buf, nb_extr, 0) != nb_extr) {
      std::cerr << "recv_line, 2nd recv : " << strerror(errno) << std::endl;
      return -1;
    }
  }

  return tot_read;
}

int send_str(int sock, const std::string &str) {
  const char *p = str.data();
  size_t  rem = str.length(); // nombre de caractères restant
  ssize_t nst = 0;            // nombre de caractères envoyés

  while(rem > 0) {
    nst = send(sock, p, rem, 0);
    if(nst < 0) {
      std::cerr << __func__ << ", send: " << strerror(errno) << std::endl;
      return -1;
    }
    p += nst;
    rem -= nst;
  }

  return rem;
}

void get_peer_info(int sock, std::string &adresse, std::string &port) {
  struct addrinfo info;
  socklen_t tailleinfo = sizeof(info);
  char hname[NI_MAXHOST], sname[NI_MAXSERV];
  
  int res = getpeername(sock, (struct sockaddr *)&info, &tailleinfo);
  if (res == -1) {
    perror("getsockname");
    exit(1);
  }
  res = getnameinfo((struct sockaddr *)&info, tailleinfo,
		    hname, NI_MAXHOST,
		    sname, NI_MAXSERV,
		    NI_NUMERICSERV|NI_NUMERICHOST);
  if (res != 0) {
    fprintf(stdout, "getnameinfo: %s\n", gai_strerror(res));
    exit(1);
  }
  adresse = hname;
  port = sname;
}


