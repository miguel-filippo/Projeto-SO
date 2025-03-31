#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#define BABY_BLUE "\033[38;5;117m"
#define RESET "\033[0m"

int main() {
    // Usamos getpagesize() como terceira primitiva de memória
    int page_size = getpagesize();
    printf(BABY_BLUE "\nTamanho da página de memória: %d bytes\n" RESET, page_size);

    // Vamos alocar múltiplas páginas para um array de inteiros
    size_t num_pages = 100;
    size_t size = num_pages * page_size;
    size_t num_integers = size / sizeof(int);

    printf(BABY_BLUE "\nAlocando %zu bytes (%zu páginas) com mmap()...\n" RESET, size, num_pages);

    // Aloca memória anônima (sem arquivo associado), leitura e escrita
    int *array = mmap(NULL, size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (array == MAP_FAILED) {
        perror("Erro ao alocar memória com mmap");
        exit(1);
    }

    printf(BABY_BLUE "\nMemória alocada com sucesso! Preenchendo com dados...\n" RESET);

    // Preenche o array com valores simples
    for (size_t i = 0; i < num_integers; i++) {
        array[i] = i % 100;
    }

    printf(BABY_BLUE "\nRealizando soma de todos os elementos...\n" RESET);

    long long sum = 0;
    for (size_t i = 0; i < num_integers; i++) {
        sum += array[i];
    }

    printf(BABY_BLUE "Soma total: %lld\n" RESET, sum);

    // Libera a memória
    if (munmap(array, size) != 0) {
        perror("Erro ao liberar memória com munmap");
        exit(1);
    }

    printf(BABY_BLUE "Memória liberada com sucesso.\n" RESET);

    return 0;
}
