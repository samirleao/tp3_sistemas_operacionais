
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
    
    //inicializar memoria
    inicializa_sistema_memoria(&memoria, &tabela, tamanhoPagina, tamanhoMemoria);
    
    //ler entrada para teste
    //le_entrada(&memoria, log, tamanhoPagina);
	
    //liberar memoria usada
    libera_sistema_memoria(&memoria, &tabela);
    
	fclose(log);
    
    
    
	return 0;
	
}