#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(){
    int ret, p[2];
    pipe(p);
    ret = fork();
    if(ret > 0) {
        close(p[0]);
        for(char c='0'; c<='9';c++) {
            write(p[1], &c, 1);
        }
        close(p[1]);
        waitpid(ret, NULL, 0);
    } else {
        char c;
        int i = 0;
        close(p[1]);
        while(read(p[0], &c, 1)==1) {
            cout<<"fils lit "<<endl;
            i++;
        }
        close(p[0]); 
        cout<<"fils a lu"<< i <<endl;
    }
    return 0;
}