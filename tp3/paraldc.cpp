#include <iostream>
#include <cstdlib>

using namespace std;

/*Code source pour le TP3 de Système -LIFASR5- 2019-20*/

int main(int argc, char *argv[]) {

  /* Le code ci-dessous sera à commenter par la suite*/

  if(argc>2){
    cerr<<"Il faut exactement un argument apres la commande.";
    return 0;
  }
  
  cout << "la commande lancée est " << argv[0] << " avec " << argc << " argument(s): " << endl;

  // impression de la suite d'arguments.
  for (int i = 0 ; i < argc; i++){
    cout << argv[i] << ",";
  }
  cout << endl;
  
  return 0;
}
