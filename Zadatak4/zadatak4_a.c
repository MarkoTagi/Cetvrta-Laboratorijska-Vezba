#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s takes in 1 parameter!\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "pipe() has failed!\nExiting...\n");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork() has failed!\nExiting...\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        close(pipefd[0]);
        int tmp_pipefd = dup2(pipefd[1], 1);
        if (tmp_pipefd == -1) {
            fprintf(stderr, "Child> dup2() has failed!\nExiting...\n");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        int execlStatus = execl(argv[1], argv[1], NULL);
        if (execlStatus < 0) {
            fprintf(stderr, "Child> execl() has failed!\nExiting...\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        close(pipefd[1]);
        int tmp_pipefd = dup2(pipefd[0], 0);
        close(pipefd[0]);
        if (tmp_pipefd == -1) {
            fprintf(stderr, "Parent> dup2() has failed!\nExiting...\n");
            exit(EXIT_FAILURE);
        }
        execlp("more", "more", NULL);
    }
    return 0;
}