#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define PERMS 0666

int main(int argc, char* argv[]) {
    //argv[1] = putanja do izvrsne datoteke (Program koji treba da se pozove)
    //argv[2] = putanja do tekstualne datoteke (Datoteka u koju treba da se upise standardni izlaz pozvanog programa)
    if (argc != 3) {
        fprintf(stderr, "%s takes in 2 parameters!\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Could not create pipefd!\n");
        exit(EXIT_FAILURE);
    }
    int fd = creat(argv[2], PERMS);
    if (fd == -1) {
        fprintf(stderr, "An error occured: [creat() has failed]. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    int tmp_fd = dup2(fd, 1);
    if (tmp_fd == -1) {
        fprintf(stderr, "An error occured: [dup2() has failed]. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    int execlpStatus = execlp(argv[1], argv[1], NULL);
    if (execlpStatus < 0) {
        fprintf(stderr, "An error occure: [execlp() has failed].\n");
    }
    return 0;
}