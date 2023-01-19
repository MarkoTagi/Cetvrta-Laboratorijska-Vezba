#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#define MAX_LEN 1024

int main() {
    int pipefd1[2], pipefd2[2];
    if (pipe(pipefd1) == -1) {
        fprintf(stderr, "Could not create pipefd1!\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd2) == -1) {
        fprintf(stderr, "Could not create pipefd2!\n");
        exit(EXIT_FAILURE);
    }
    int pid1 = fork();
    if (pid1 > 0) { //Parent
        int pid2 = fork();
        if (pid2 > 0) { //Parent
            close(pipefd1[0]);
            close(pipefd2[0]);
            close(pipefd2[1]);
            char userInput[MAX_LEN];
            do {
                printf("Enter something (or type \"END\" to exit): ");
                fgets(userInput, MAX_LEN, stdin); 
                int inputLength = strlen(userInput);
                userInput[inputLength - 1] = '\0';
                write(pipefd1[1], userInput, inputLength);
            } while (strcmp(userInput, "END") != 0);
            wait(NULL);
            close(pipefd1[1]);
            printf("Parent> Exiting...\n");
        }
        else { //Child1
            close(pipefd1[1]);
            close(pipefd2[0]);
            char userInput[MAX_LEN];
            do {
                read(pipefd1[0], userInput, MAX_LEN);
                int inputLength = strlen(userInput);
                if (strcmp(userInput, "END") != 0) {
                    bool isLowercase = userInput[0] >= 'a' && userInput[0] <= 'z';
                    userInput[0] -= isLowercase ? 32 : 0;
                    printf("Child1> Modified string: (%s)\n", userInput);  
                }
                write(pipefd2[1], userInput, ++inputLength);
            } while (strcmp(userInput, "END") != 0);
            wait(NULL);
            close(pipefd1[0]);
            close(pipefd2[1]);
            printf("Child1> Exiting...\n");    
        }
    }
    else { //Child2
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[1]);
        char userInput[MAX_LEN];
        do {
            read(pipefd2[0], userInput, MAX_LEN);
            if (strcmp(userInput, "END") != 0) {
                int inputLength = strlen(userInput);
                if (userInput[inputLength - 1] != '.') {
                    if (inputLength != MAX_LEN) {
                        userInput[inputLength] = '.';
                        userInput[++inputLength] = '\0';
                    }
                }
                printf("Child2> Modified string: (%s)\n", userInput);
            }
        } while (strcmp(userInput, "END") != 0);
        close(pipefd2[0]);
        printf("Child2> Exiting...\n");       
    }
    return 0;    
}