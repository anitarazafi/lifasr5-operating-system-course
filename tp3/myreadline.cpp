#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

void myreadline(string &s){
    s.clear();
    int nbrd, nbrem;
    char *t[100];
    do{
        nbrd=read(STDIN_FILENO, &t, sizeof(t));
        s.push_back(t);
        if(t=="\n"){
            break;
        }
        if(nbrd<0){
            cerr<errno<<endl;
            exit EXIT_FAILURE;
        }
        nbrem=-nbrd;
    } (while nbrem>0);
}

int main(void){
    string s;
    cout<<"entre une ligne de texte"<<endl;
    cin>>s;
    myreadline(s);

    return 0;
}