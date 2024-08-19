#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    // Tamanho da página de memória (em bytes)
    size_t page_size = sysconf(_SC_PAGESIZE);
    printf("Tamanho da página de memória: %zu bytes\n", page_size);

    // Número de páginas que você deseja alocar
    int num_pages = 1;

    // Tamanho total da memória alocada
    size_t total_size = num_pages * page_size;

    // Alocando memória virtual com mmap
    char *memory = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memory == MAP_FAILED) {
        perror("mmap falhou");
        return 1;
    }

    // Dados a serem escritos na memória
    const char *text = "Este é um teste de escrita em memória virtual.";

    // Certificando-se de que o texto não exceda o tamanho da memória alocada
    if (strlen(text) >= total_size) {
        fprintf(stderr, "O texto é muito grande para a memória alocada.\n");
        munmap(memory, total_size);
        return 1;
    }

    // Escrevendo os dados na memória alocada
    strncpy(memory, text, total_size);

    // Mostrando o conteúdo da memória
    printf("Dados escritos na memória: %s\n", memory);

    // Liberando a memória alocada
    if (munmap(memory, total_size) == -1) {
        perror("munmap falhou");
        return 1;
    }

    return 0;
}
