#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int i;
    int j = 0;
    int k;
    int l,m = 0;
    char block[32]; // Block = it should contain arguments of first command
    char buf[32]; // buf is for moving from bloc to split
    
    char *p = buf;
    char *split[32]; // Split = it should contain arguments of xargs

    for(i = 1; i < argc; i++){ // xargs arguments
        split[j++] = argv[i];
    }

    while( (k = read(0, block, sizeof(block))) > 0){
        for(l = 0; l < k; l++){
            if(block[l] == '\n'){
                buf[m] = 0;
                m = 0;
                split[j++] = p;
                p = buf;
                split[j] = 0;
                j = argc - 1;
                if(fork() == 0){
                    exec(argv[1], split); // Final command with all arguments as it should be
                }
                wait(0);
            }else if(block[l] == ' ') { // switch to another argument after ' ' from last command
                buf[m++] = 0;
                split[j++] = p;
                p = &buf[m];
            }else {
                buf[m++] = block[l]; // Arguments of command before xargs (first command arguments assign by one char)
            }
        }
    }
    exit(0);
}
