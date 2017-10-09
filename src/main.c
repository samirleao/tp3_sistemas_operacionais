
#include "virtual.h"
#include <string.h>

memoria_t memoria;
tabela_paginas_t tabela;

int main(int argc, char **argv) {
	
    char algoritmo[10];
	FILE *log;
	unsigned tamanhoPagina;
	unsigned tamanhoMemoria;
    
    //pegar argumentos
    strcpy(algoritmo, argv[1]);
    tamanhoPagina = atoi(argv[3]);
    tamanhoMemoria = atoi(argv[4]);
    
    if( (log = fopen(argv[2], "r")) == NULL ) {
		perror("Erro: fopen");
		exit(EXIT_FAILURE);
	}

	printf("prompt> tp3virtual %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4]);
	printf("Executando o simulador...\n");
    
    //inicializar memoria
    inicializa_sistema_memoria(&memoria, &tabela, tamanhoPagina, tamanhoMemoria);
    
    //ler entrada para teste
    processa_entrada(&memoria, &tabela, algoritmo, log);

	//imprimir resultados
	printf("Arquivo de entrada: %s\n", argv[2]);
	printf("Tamanho da memoria: %d KB\n", tamanhoMemoria);
	printf("Tamanho das paginas: %d KB\n", tamanhoPagina);
	printf("Tecnica de reposicao: %s\n", algoritmo);
	printf("Paginas lidas: %d\n", memoria.paginasLidas);
	printf("Paginas escritas: %d\n", memoria.paginasSujasRemovidas);
	
    //liberar memoria usada
    libera_sistema_memoria(&memoria, &tabela);
    
	fclose(log);
    
	return 0;
	
}