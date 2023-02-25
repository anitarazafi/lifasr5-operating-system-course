// g++ -g -Wall -std=c++11 shell.cpp -o monshell

#if (!defined(__cplusplus) || (__cplusplus != 201103L))
#error Il faut utiliser c++11. Apprenez à configurer votre IDE favori ou à lire les informations en début de fichier.
#endif


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <regex>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

/* ****************************************************************************** */
/* Fonctions pour le découpage d'une ligne de commandes                           */
/* ****************************************************************************** */

// Fonction utilitaire pour découper une chaîne de caractères en morceaux.
// s est la chaîne à découper, e la regex pour détecter les séparateurs,
// et la fonction retourne un tableau de chaînes de caractères terminé par null.
// Le tableau retourné a été réservé par malloc et doit être libéré par fonction
// freeTabChar. Attention la fonction ignore les tokens vides.
// L'implémentation de ces fonctions se trouve à la fin du fichier.
char **splitToChar(string s,const std::regex &e);
void freeTabChar(char **t);

/* ****************************************************************************** */
/* Gestion de l'attente d'un processus fils                                       */
/* ****************************************************************************** */

// Gestionnaire de signal pour le signal SIGCHLD.
// Pour se mettre en attente de la terminaison d'un fils spécifique, le père doit
// placer le PID de ce fils dans la variable globale pid_attendu. Rappelons que
// lorsqu'un fils (quelconque) se termine, son père reçoit le signal SIGCHLD :
// si la fonction traitement_signal_fils() a bien été installée comme gestionnaire
// du signal SIGCHLD dans le père, elle doit donc être appelée lorsqu'un fils
// se termine. Le gestionnaire de signal doit entériner (avec waitpid) la
// terminaison de m'importe quel fils, et remettre pid_attendu à 0 si c'est
// le fils que l'on attendait qui s'est terminé.

pid_t pid_attendu = 0;

void traitement_signal_fils(int sig) {
  // TODO 4 : compléter le gestionnaire du signal SIGCHLD.
  pid_t res = waitpid(-1, NULL, 0);
  if(res==-1) {
    cerr << "L'un de mes fils est terminé, mais je le laisse à l'état de zombie..." << endl << flush;
  } else {
    cout<<"#processus "<<res<<"vient de se terminer"<<endl;
  } 
  if(res==pid_attendu){
    pid_attendu=0; 
  }
}

/* ****************************************************************************** */
/* Le programme principal : le mini-shell                                         */
/* ****************************************************************************** */

int main(int argc, char *argv[]) {
  // Mise en place du gestionnaire de signal pour SIGCHLD. On utilise les drapeaux :
  // SA_RESTART pour reprendre l'appel système sur lequel le processus était bloqué lors de
  // la réception du signal (ici read, dans getline), et SA_NOCLDSTOP pour prendre en compte
  // SIGCHLD lorsqu'un fils se termine, pas simplement lorsqu'il est stoppé...
  struct sigaction sa;
  sigaction(SIGCHLD, NULL, &sa);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  // TODO 3 : terminer l'installation de la fonction traitement_signal_fils()
  // comme gestionnaire pour le signal SIGCHLD. Indications : vous devez compléter
  // le champ sa_handler de la structure sa, appeler sigaction() avec les bons
  // paramètres, et vérifier sa valeur de retour.
  sa.sa_handler = &traitement_signal_fils();
  sigaction(SIGCHLD, &sa, NULL);
  
  while (1) {
    int attend;
    int num_pipes = 0;
    string text_com;
    char **arg_com, **arg_com1, **arg_com2;

    // #############################################
    // Lecture et analyse de la ligne de commandes 
    // #############################################

    // Invite de commande, et lecture d'une ligne
    cout << "Entrez une commande > " << flush;
    getline(cin, text_com);

    if (cin.eof() || cin.fail() ) {
      cerr << "## Fin du shell." << endl << flush;
      break;
    }

    // on normalise la ligne en supprimant les espaces doubles, les tabulations, ...
    text_com = std::regex_replace(text_com, std::regex("[ \t\n]+"), " "); // espaces en double, tabulations... 
    text_com = std::regex_replace(text_com, std::regex("^ +"), "");       // espaces en debut de ligne
    text_com = std::regex_replace(text_com, std::regex(" +$"), "");       // espaces en fin de ligne
    cerr << "## Lecture de la commande \"" << text_com << "\"" << endl << flush;

    if(text_com == "quit") {
      cout << "## Le shell se termine." << endl << flush;
      break;
    }

    if (text_com == "") {
      cerr << "## La commande est vide." << endl;
      continue;
    }
      
    if (text_com.back() == '&') { // la commande doit être lancée en tâche de fond
      text_com.pop_back();
      text_com = std::regex_replace(text_com, std::regex(" +$"), ""); // suppression des espaces restants
      attend = false;
    }
    else attend = true; // la commande doit être lancée au premier plan

    if(std::regex_match(text_com, std::regex(".*[|].*"))) {
      // il y a au moins un '|' dans la ligne de commandes
      std::smatch mpipe;
      if (!std::regex_search(text_com, mpipe, std::regex("^([^|]+)\b*[|]\b*([^|]+)$"))) { // il y a plus d'un pipe, on ne traite pas ce cas
        cerr << "## La commande \"" << text_com << "\" contient plusieurs pipes, on ne la traite pas." << endl << flush;
        continue;
      }      
      else {
        num_pipes = 1; // C'est bon, il n'y a qu'un seul pipe
        arg_com1 = splitToChar(mpipe[1].str(), std::regex("[ \t]"));
        arg_com2 = splitToChar(mpipe[2].str(), std::regex("[ \t]"));
      }
    }
    else { // Pas de pipe dans la ligne de commandes
      arg_com = splitToChar(text_com, std::regex(" "));
      num_pipes = 0;
    }

    // #############################################
    // Traitement de la ligne de commandes
    // #############################################
    // attend est à true si le shell doit attendre la fin de la commande, false sinon.
    // num_pipes est à 0 s'il n'y a pas de pipe sur la ligne, 1 sinon (on ne gère pas num_pipes > 1).

    if (num_pipes == 0) {
      // Cas où il n'y a pas de '|' sur la ligne de commandes.
      // Il faut juste se préoccuper de la commande dans arg_com
      
      // TODO 1 : modifiez cette partie pour exécuter une commande simple.
      // Indications : arg_com[0] est le nom de commande à passer à execvp,
      // et arg_com est le tableau des paramètres à passer à execvp.
      pid_t pid_com = fork();
      if(pid_com==0){
        execvp(arg_com[0], arg_com);
        exit(EXIT_FAILURE);
      }
      res=waitpid(pid_com, NULL, 0);

      // TODO 2 : deux cas à gérer, "arrière-plan" ou "avant-plan". Cela va
      // dépendre de la variable attend, qui a été initialisée dans le partie
      // "Lecture et analyse de la ligne de commandes" : elle a été mise à false
      // si la ligne de commande se termine par un &, true sinon.
      if(attend){
        /*waitpid(pid_com,NULL, 0);*/
        pid_attendu = pid_com;
        while(pid_attendu==pid_com){
          pause();
        } //7e question Rappel
      }
      else{
        cerr<<"##Ligne de commande lancée en tache de fond."<<endl;
      }
      cerr << "## L'exécution d'une commande n'est pas encore implantée." << endl;

      // Libération du tableau arg_com
      freeTabChar(arg_com);

    }
    else if(num_pipes == 1) { // Cas où il y a exactement un '|' sur la ligne de commandes.

      // Il y a un et un seul pipe, donc 2 commandes à traiter :
      // la première est dans arg_com1, l'autre dans arg_com2

      //TODO 5: exécution de deux commandes liées par un pipe.
      int pipefd[2];
      pid_t pid_com1, pid_t pid_com2;
      pipe(pipefd);
      pid_com1=fork();
      if(pid_com1==0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execvp(arg_com1[1], arg_com1);
        close(pipefd[1]);
      } else {
        close(pipefd[1]);
        pid_com2=fork();
        if(pid_com2==0){
          dup2(pipefd[0],STDIN_FILENO);
          execvp(arg_com2[0],arg_com2);
        }
      }
      cerr << "Exec 1 pipe: not yet implemented. "<< endl << flush;

      // Libération des tableaux arg_com1 et arg_com2
      freeTabChar(arg_com1);
      freeTabChar(arg_com2);

    } // Fin du cas où il y a exactement un '|' sur la ligne de commande
    else {
      // Cas avec plusieurs pipe. Ce code est du code mort pour l'instant.
      cerr << "Erreur : il n'est pas possible de lancer une commmande avec plus d'un pipe..." << endl << flush;
    }
  }
  return EXIT_SUCCESS;
}

/* ************************************************************************************ */
/* Fonctions utilitaires, dont il n'est pas nécessaire de se préoccuper pour l'exercice */
/* ************************************************************************************ */

// Pour découper la ligne de commandes
char **splitToChar(string s,const std::regex &e) {
  unsigned int i;
  std::vector<string> spl;
  std::smatch m;
  // découpage
  while (std::regex_search(s, m, e)) {
    string pre = m.prefix().str();
    if (pre.size() != 0) {
      spl.push_back(pre);
    }
    s = m.suffix();
  }
  if (s.size() != 0) {
    spl.push_back(s);
  }

  // conversion 
  char **res = new char*[spl.size()+1];
  for (i=0; i<spl.size(); i++) {
    
    res[i]=new char[spl[i].size()+1];
    strcpy(res[i], spl[i].c_str());
  }
  res[i] = NULL;

  return res;
}

// Pour libérer le tableau de chaînes de caractères produit par splitToChar
void freeTabChar(char **t) {
  int i;
  for (i=0; t[i]!= NULL; i++) {
    delete[] t[i];
    t[i] = NULL;
  }
  delete[] t;
}

