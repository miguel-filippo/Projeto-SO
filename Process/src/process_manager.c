#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define GREEN "\033[0;32m"
#define RESET "\033[0m"

int main() {
    pid_t pid_io, pid_mem;
    int status;
    char *time = "/usr/bin/time";

    // ---------- Processo IO-Bound ----------
    pid_io = fork();
    if (pid_io < 0) {
        perror("Erro ao criar processo IO-bound");
        exit(1);
    } else if (pid_io == 0) {
        printf(GREEN "\nFilho IO-bound iniciado (PID: %d)\n" RESET, getpid());

        char *io_program = "/home/gsograd13/checkpoint_1/IO_Files/bin/io_primitives_iobound";
        char *input_file = "/home/gsograd13/checkpoint_1/IO_Files/data";
        char *backup_file = "/home/gsograd13/checkpoint_1/IO_Files/io_backup";
        char *io_args[] = {time, io_program, input_file, backup_file, NULL};

        execve(time, io_args, NULL);
        perror("Erro no execve do processo IO-bound");
        exit(1);
    }

    // ---------- Processo Memória (CPU-bound com mmap/munmap) ----------
    pid_mem = fork();
    if (pid_mem < 0) {
        perror("Erro ao criar processo de memória");
        exit(1);
    } else if (pid_mem == 0) {
        printf(GREEN "\nFilho MEMÓRIA iniciado (PID: %d)\n" RESET, getpid());

        char *mem_program = "/home/gsograd13/checkpoint_1/Memory/bin/mem_cpu_bound";
        char *mem_args[] = {time, mem_program, NULL};

        execve(time, mem_args, NULL);
        perror("Erro no execve do processo de memória");
        exit(1);
    }

    // ---------- Processo Pai ----------
    printf(GREEN "\nProcesso pai aguardando filhos...\n" RESET);

    for (int i = 0; i < 2; i++) {
        pid_t finished_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf(GREEN "\nFilho PID %d terminou com código de saída %d\n" RESET, finished_pid, WEXITSTATUS(status));
        } else {
            printf(GREEN "\nFilho PID %d terminou de forma anormal\n" RESET, finished_pid);
        }
    }

    printf(GREEN "\nTodos os filhos finalizaram.\n" RESET);
    return 0;
}
