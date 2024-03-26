#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    char buf[MAXARG];
    char *args[MAXARG];
    int args_idx = 0;

    for (int i = 1; i < argc; i++) {
        args[args_idx++] = argv[i];
    }

    while (1) {
        int n = read(0, buf, sizeof(buf)); // Read input line
        if (n == 0) {
            break;
        }

        buf[n] = '\0'; // Null terminate

        int i = 0;
        while (i < n) {
            int start = i;
            while (i < n && buf[i] != '\n') {
                i++;
            }
            buf[i] = '\0'; // Null terminate the line

            args[args_idx] = &buf[start];
            args[args_idx + 1] = 0; // Null terminate

            int pid = fork();
            if (pid < 0) {
                fprintf(2, "Fork failed\n");
                exit(1);
            } else if (pid == 0) {
                exec(args[0], args);
                fprintf(2, "Exec failed\n");
                exit(1);
            } else {
                wait(0);
            }

            i++; // Move to the next character after '\n'
        }
    }

    exit(0);
}
