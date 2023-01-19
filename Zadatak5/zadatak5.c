#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Could not create pipefd!\n");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Could not fork the process!\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) { //Parent
        close(pipefd[0]); 
        char filePath[80];
        printf("File path: ");
        scanf("%s", filePath);
        FILE* fd = fopen(filePath, "r");
        if (fd == NULL) {
            fprintf(stderr, "Could not open (%s)!\n", filePath);
            exit(EXIT_FAILURE);
        }
        char line[80];
        while (fgets(line, 80, fd) != NULL) {
            //int lineLength = strlen(line);
            //line[lineLength - 1] = '\0'; 
            printf("Parent> Read: (%s)\n", line);
            printf("Parent> Sending (%s)...\n", line);
            fflush(stdout);
            write(pipefd[1], line, strlen(line) + 1);
        }
        fclose(fd);
        close(pipefd[1]);
        printf("Parent> Exiting...\n");
    }
    else { //Child
        close(pipefd[1]);
        char line[80];
        int bytesReceived;
        do {
            bytesReceived = read(pipefd[0], line, 80);
            for (int i = 0; i < bytesReceived - 1; ++i) {
                putc(line[i], stdout);
            }
        } while (bytesReceived > 0);
        close(pipefd[0]);
        printf("Child> Exiting...\n");
    }
    return 0;
}