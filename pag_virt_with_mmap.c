#include <stdio.h>      // Inclui a biblioteca padrão de entrada e saída
#include <stdlib.h>     // Inclui a biblioteca padrão para funções utilitárias (como malloc, exit, etc.)
#include <string.h>     // Inclui a biblioteca para manipulação de strings
#include <fcntl.h>      // Inclui a biblioteca para manipulação de arquivos (open, O_RDWR, etc.)
#include <sys/mman.h>   // Inclui a biblioteca para manipulação de memória (mmap, munmap, etc.)
#include <sys/stat.h>   // Inclui a biblioteca para obter informações sobre arquivos (stat, fstat, etc.)
#include <unistd.h>     // Inclui a biblioteca para funções do POSIX, como close, ftruncate, etc.
#include <stdint.h>     // Inclui a biblioteca para tipos de dados de largura fixa (como uintptr_t)

#define PAGESIZE 4096   // Define o tamanho da página de memória (geralmente 4096 bytes)

int main() {
    // Abre o arquivo kjv.txt para leitura e escrita. O arquivo deve existir previamente.
    int fd = open("kjv.txt", O_RDWR);
    if (fd == -1) {  // Verifica se o arquivo foi aberto corretamente
        perror("Erro ao abrir kjv.txt");  // Exibe uma mensagem de erro se o arquivo não puder ser aberto
        exit(EXIT_FAILURE);  // Encerra o programa em caso de erro
    }

    // Obtém informações sobre o arquivo, como o tamanho, usando fstat
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {  // Verifica se as informações foram obtidas corretamente
        perror("Erro ao obter o tamanho do arquivo");  // Exibe uma mensagem de erro se não puder obter as informações do arquivo
        close(fd);  // Fecha o arquivo para liberar recursos
        exit(EXIT_FAILURE);  // Encerra o programa em caso de erro
    }
    size_t file_size = file_stat.st_size;  // Armazena o tamanho do arquivo

    // Mapeia o arquivo para a memória. MAP_SHARED permite que outras operações de mapeamento vejam as alterações.
    void *map = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {  // Verifica se o mapeamento foi realizado com sucesso
        perror("Erro ao mapear o arquivo para a memória");  // Exibe uma mensagem de erro se o mapeamento falhar
        close(fd);  // Fecha o arquivo para liberar recursos
        exit(EXIT_FAILURE);  // Encerra o programa em caso de erro
    }

    // Exibe informações sobre a memória mapeada
    uintptr_t map_address = (uintptr_t)map;  // Converte o endereço da memória mapeada para um tipo inteiro para exibição
    printf("Memória mapeada:\n");
    printf("Endereço inicial do mapeamento: 0x%lx\n", map_address);  // Exibe o endereço inicial do mapeamento
    printf("Tamanho do mapeamento: %zu bytes\n", file_size);  // Exibe o tamanho do mapeamento

    // Cria ou abre um arquivo de saída para escrever os dados mapeados. O_TRUNC limpa o conteúdo anterior.
    int output_fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd == -1) {  // Verifica se o arquivo de saída foi aberto ou criado com sucesso
        perror("Erro ao abrir output.txt");  // Exibe uma mensagem de erro se falhar
        munmap(map, file_size);  // Desmapeia o arquivo da memória para liberar recursos
        close(fd);  // Fecha o arquivo original
        exit(EXIT_FAILURE);  // Encerra o programa em caso de erro
    }

    // Ajusta o tamanho do arquivo de saída para ser igual ao tamanho do arquivo original
    if (ftruncate(output_fd, file_size) == -1) {  // Verifica se o ajuste foi bem-sucedido
        perror("Erro ao ajustar o tamanho do arquivo de saída");  // Exibe uma mensagem de erro se falhar
        close(output_fd);  // Fecha o arquivo de saída
        munmap(map, file_size);  // Desmapeia o arquivo da memória
        close(fd);  // Fecha o arquivo original
        exit(EXIT_FAILURE);  // Encerra o programa em caso de erro
    }

    // Escreve o conteúdo da memória mapeada no arquivo de saída
    ssize_t written_size = write(output_fd, map, file_size);
    if (written_size != file_size) {  // Verifica se todo o conteúdo foi escrito corretamente
        perror("Erro ao escrever no arquivo de saída");  // Exibe uma mensagem de erro se falhar
        close(output_fd);  // Fecha o arquivo de saída
        munmap(map, file_size);  // Desmapeia o arquivo da memória
        close(fd);  // Fecha o arquivo original
        exit(EXIT_FAILURE);  // Encerra o programa em caso de erro
    }

    // Exibe os primeiros 100 caracteres do conteúdo mapeado como uma amostra do que foi lido
    printf("Dados escritos na memória:\n");
    printf("Conteúdo inicial do kjv.txt:\n");
    printf("%.100s\n", (char *)map);  // Exibe uma amostra dos dados mapeados

    // Sincroniza o conteúdo da memória mapeada com o arquivo no disco
    if (msync(map, file_size, MS_SYNC) == -1) {  // Verifica se a sincronização foi bem-sucedida
        perror("Erro ao sincronizar a memória mapeada com o arquivo");  // Exibe uma mensagem de erro se falhar
    }

    // Desmapeia o arquivo da memória virtual
    if (munmap(map, file_size) == -1) {  // Verifica se o desmapeamento foi bem-sucedido
        perror("Erro ao desmapear o arquivo da memória");  // Exibe uma mensagem de erro se falhar
    }

    // Fecha os arquivos para liberar recursos
    close(output_fd);  // Fecha o arquivo de saída
    close(fd);  // Fecha o arquivo original

    printf("Conteúdo de kjv.txt mapeado para a memória e salvo em output.txt.\n");

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso
}
