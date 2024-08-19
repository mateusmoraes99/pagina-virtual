# pagina-virtual
O código que aloca uma página de memória virtual utilizando mmap, escreve um texto nessa memória, e depois exibe o conteúdo armazenado. Vou destacar os pontos importantes e explicar como ele funciona:

# Explicação do Código

    1.Tamanho da Página de Memória:
    
    size_t page_size = sysconf(_SC_PAGESIZE);
    
O código obtém o tamanho de uma página de memória do sistema usando sysconf(_SC_PAGESIZE). Esse valor é geralmente 4096 bytes (4 KB) em muitos sistemas, mas pode variar.



