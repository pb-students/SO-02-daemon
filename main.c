#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>


int main() {
    pid_t pid = fork();
    if (pid == -1) {
        printf("Error while forking: ");
        switch (errno) {
            case ENOMEM: printf("The process requires more space than the system can supply. \n"); break;
            case EAGAIN: printf("Too many processes running.\n"); break;
        }
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("Successfully forked: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    umask(0);

    // New sid for cp
    pid_t sid = setsid();
    if (sid < 0) {
        printf("Error while requesting new session: ");
        switch (errno) {
            case EPERM: printf("The process group ID of any process equals the PID of the calling process."); break;
        }
        exit(EXIT_FAILURE);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        pid_t pid = fork();
        if (pid == 0) {
            char* argv[] = {"notify-send", "hej", NULL};
            execvp("notify-send", argv);
        }

        if (pid == -1) {
            _exit(EXIT_FAILURE);
        }

        sleep(20);
    }
}
