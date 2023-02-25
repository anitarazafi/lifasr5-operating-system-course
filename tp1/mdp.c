#include <iostream>
#include <crypt.h>

using namespace std;

int main(void){
    char clair[] = "Toto2001FaitDuVelo";
    char *hache = crypt(clair, "66");
    cout<<"clair: "<<clair<<endl;
    cout<<"hache: "<<hache<<endl;

    return 0;
}