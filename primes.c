#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PRIME 1000
#define MAX_DEPTH 60  // Prevent excessive recursion

void sieve(int p_left[], int depth) __attribute__((noreturn));

void sieve(int p_left[], int depth) {
    if (depth > MAX_DEPTH) {
        close(p_left[0]);
        exit(0);
    }

    close(p_left[1]);
    int prime;

    if (read(p_left[0], &prime, sizeof(int)) == 0) {
        close(p_left[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p_right[2];
    if (pipe(p_right) < 0) {
        fprintf(2, "Pipe failed\n");
        close(p_left[0]);
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Fork failed\n");
        close(p_left[0]);
        close(p_right[0]);
        close(p_right[1]);
        exit(1);
    } else if (pid == 0) {
        close(p_right[1]);
        sieve(p_right, depth + 1);
    } else {
        close(p_right[0]);
        int num;
        while (read(p_left[0], &num, sizeof(int)) > 0) {
            if (num % prime != 0) {
                if (write(p_right[1], &num, sizeof(int)) <= 0) {
                    break;  // Prevent infinite loops if pipe fails
                }
            }
        }
        close(p_left[0]);
        close(p_right[1]);
        wait(0);
        exit(0);
    }
}

int main() {
    int p_first[2];
    if (pipe(p_first) < 0) {
        fprintf(2, "Pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        close(p_first[1]);
        sieve(p_first, 1);  // Start with depth = 1
    } else {
        close(p_first[0]);
        for (int i = 2; i <= MAX_PRIME; i++) {
            write(p_first[1], &i, sizeof(int));
        }
        close(p_first[1]);
        wait(0);
        exit(0);
    }
}

