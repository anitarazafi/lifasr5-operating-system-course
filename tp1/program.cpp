#include <iostream>
#include <ctime>
#include <string>

using namespace std;

int main(void){
    time_t now = time(nullptr);
    char date[100];
    char heure[100];

    strftime(date, 50, "Date du jour: %d/%m/%Y", localtime(&now));
    strftime(heure, 50, "Heure courante: %Hh%M", localtime(&now));

    cout<<date<<endl;
    cout<<heure<<endl;

    return 0;
}