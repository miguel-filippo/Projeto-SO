#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BABY_BLUE "\033[38;5;117m"
#define RESET "\033[0m"

/*
A constante BUFFER_SIZE possui relação direta com o CPU-BOUND/IO-BOUND desse programa.

Quando BUFFER_SIZE é pequeno, o programa realiza muitas chamadas de sistema
(read/write), o que o torna mais I/O-bound, pois passa mais tempo esperando
operações de entrada/saída.

Quando BUFFER_SIZE é grande, o programa faz menos chamadas e realiza mais
operações por vez em memória, utilizando mais CPU e tornando-se menos
I/O-bound.
*/

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    // Verifica a utilizacao correta do script
    if (argc != 3) {
        write(STDERR_FILENO, "Uso: ./backup origem destino\n", 30);
        exit(1);
    }

    // Abre os arquivos de origem e destino com os modos de leitura e escrita
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        perror("Erro ao abrir o arquivo de origem");
        exit(1);
    }

    // Abre o arquivo de destino para escrita, criando-o caso não exista e truncando-o caso já exista.
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Erro ao criar o arquivo de destino");
        close(src_fd);
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    printf(BABY_BLUE "Fazendo backup de maneira CPU-Bound -> Buffer Size = %d\n" RESET, BUFFER_SIZE);

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Erro na escrita do arquivo");
            close(src_fd);
            close(dest_fd);
            exit(1);
        }
    }

    if (bytes_read < 0) {
        perror("Erro na leitura do arquivo");
    }

    printf(BABY_BLUE "Backup CPU-bound feito com sucesso!\n" RESET);

    close(src_fd);
    close(dest_fd);

    return 0;
}
