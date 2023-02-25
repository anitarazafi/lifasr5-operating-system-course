#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(void){
    int pid = fork();
    if(pid > 0 ) {
        for(int i; i<10; i++) {
            sleep(1);
            kill(pid, SIGCONT);
            cout << "pere: ping";
        }
        waitpid(pid, NULL, 0);
    } else {
        for(int i; i<10; i++){
            kill(getpid(), SIGSTOP);
            cout << "fils : pong";
        }
    }
    return 0;
}