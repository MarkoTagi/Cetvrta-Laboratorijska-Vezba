#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#define PARAMS 0666

int main() {
    int pipefd1[2], pipefd2[2];
    if (pipe(pipefd1) == -1) {
        fprintf(stderr, "pipe(pipefd1) has failed!\nThe process will now exit...\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd2) == -1) {
        fprintf(stderr, "pipe(pipefd2) has failed!\nThe process will now exit...\n");
        exit(EXIT_FAILURE);
    }
    int pid1 = fork();
    if (pid1 > 0) { //Process_1(Parent) 
        int pid2 = fork();
        if (pid2 > 0) { //Process_1(Parent)
            close(pipefd1[0]);
            close(pipefd2[0]);
            char randomString[20];
            srand(time(0));
            int asciiValue = 0;
            for (int i = 0; i < 19; ++i) {
                randomString[i] = (rand() % 95) + 32;
                asciiValue += randomString[i];
            }
            randomString[19] = '\0';
            printf("Process_1> Generated string: (%s).\n", randomString);
            printf("Process_1> ASCII value: %d.\n", asciiValue);
            if ((asciiValue % 2) == 0) write(pipefd1[1], randomString, 20);
            else write(pipefd2[1], randomString, 20);
            close(pipefd1[1]);
            close(pipefd2[1]);
            printf("Process_1> Exiting...\n");
        }
        else { //Process_2(Child2)
            close(pipefd1[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);
            char buffer[20];
            int bytesReceived = read(pipefd1[0], buffer, 20);
            int fd = creat("KARAKTERI.txt", PARAMS);
            int standardOutput = dup(1);
            int tmp_fd = dup2(fd, 1);
            close(fd);
            if (tmp_fd == -1) {
                fprintf(stderr, "Process_2> dup2() has failed.\nThe process will now exit...\n");
                exit(EXIT_FAILURE);
            }
            if (bytesReceived > 0) printf("Process_2 wrote (%s) into this file.\n", buffer);
            else printf("The value was odd, sorry. :(\n");
            fflush(stdout); //MUST FLUSH STANDARD OUTPUT STREAM FOR IT TO WORK!
            close(pipefd1[0]);
            close(fd);
            dup2(standardOutput, 1);
            close(standardOutput);
            printf("Process_2> Exiting...\n");
        }
    }
    else { //Process_3(Child1)
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[1]);
        char buffer[20];
        int bytesReceived = read(pipefd2[0], buffer, 20);
        if (bytesReceived > 0) printf("Process_3> Received: (%s).\n", buffer);
        close(pipefd2[0]);
        printf("Process_3> Exiting...\n");
    }
    return 0;
}