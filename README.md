# pagina-virtual
O código que aloca uma página de memória virtual utilizando mmap, escreve um texto nessa memória, e depois exibe o conteúdo armazenado. Vou destacar os pontos importantes e explicar como ele funciona:

## Explicação do Código

 Tamanho da Página de Memória:
    
 size_t page_size = sysconf(_SC_PAGESIZE);
    
O código obtém o tamanho de uma página de memória do sistema usando sysconf(_SC_PAGESIZE). Esse valor é geralmente 4096 bytes (4 KB) em muitos sistemas, mas pode variar.

## Alocação de Memória Virtual:

char *memory = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

O mmap é usado para alocar total_size bytes de memória virtual. As flags PROT_READ | PROT_WRITE permitem leitura e escrita na memória. MAP_PRIVATE | MAP_ANONYMOUS indica que a memória não estará associada a nenhum arquivo e será privada (não compartilhada com outros processos).

O retorno de mmap é um ponteiro para a área de memória alocada.

## Verificação do Sucesso da Alocação:

if (memory == MAP_FAILED) {
    perror("mmap falhou");
    return 1;
}

Se mmap falhar, MAP_FAILED é retornado, e o código imprime uma mensagem de erro e termina.


## Escrita de Dados na Memória:

const char *text = "Este é um teste de escrita em memória virtual.";
strncpy(memory, text, total_size);

O código copia o texto text para a memória alocada utilizando strncpy. A função strncpy assegura que o texto não exceda o tamanho da memória alocada.

## Exibição do Conteúdo da Memória:

printf("Dados escritos na memória: %s\n", memory);

## Liberação da Memória Alocada:

if (munmap(memory, total_size) == -1) {
    perror("munmap falhou");
    return 1;
}

A memória alocada é liberada usando munmap, evitando vazamentos de memória.


# Como Compilar e Executar:

Salve o Código:
        Salve o código em um arquivo chamado, por exemplo, memoria.c.

Compile o Código:
        Compile o código usando o GCC:
        gcc -std=c99 -o memoria memoria.c

Execute o Programa:
       ./memoria


## Saída Esperada

Ao executar o programa, a saída esperada seria algo como: 
               Tamanho da página de memória: 4096 bytes
               Dados escritos na memória: Este é um teste de escrita em memória virtual.


Este programa demonstra como trabalhar com a memória virtual no C, uma habilidade útil em sistemas operacionais, desenvolvimento de software de baixo nível, e em casos onde o gerenciamento de memória eficiente é crucial.


repositorio no replit para ver melhor codigo pag_virt_with_mmap.c ###https://replit.com/@mateusmoraes4/c





        




