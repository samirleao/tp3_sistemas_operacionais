
#include "virtual.h"
#include <string.h>

unsigned deslocamento_bits(unsigned tamanhoPagina) {
    unsigned tmp = tamanhoPagina * pow(2, 10);
    unsigned s = 0;
	while( tmp > 1 ) {
		tmp = tmp >> 1;
		s++;
	}
    return s;
}

unsigned pagina(unsigned endereco, unsigned s) {
	return (endereco >> s);
}

void processa_entrada(memoria_t *memoria, tabela_paginas_t *tabela, char *algoritmo, FILE *log){
    unsigned endereco;
    char rw;
    unsigned idPagina;
    unsigned s;
    unsigned quadroSelecionado;
    
    s = deslocamento_bits(tabela->tamanhoPaginas);
    
    //ler cada linha do arquivo de log
    while( !feof(log) ) {
        fscanf(log, "%x %c", &endereco, &rw);
        idPagina = pagina(endereco, s);
        
        //para o algoritmo de substituicao lru
        if( strcmp(algoritmo, "lru") == 0 ) {
            
            //se o acesso for de leitura
            if( rw == 'R' ) {
                if( tabela->paginas[idPagina].carregada == 1 ) {
                    tabela->paginas[idPagina].numReferencias++;
                    tabela->paginas[idPagina].instanteReferencia = memoria->relogio;
                } else {
                    quadroSelecionado = seleciona_quadro_lru(memoria, tabela);
                    tabela->paginas[idPagina].quadro = quadroSelecionado;
                    tabela->paginas[idPagina].carregada = 1;
                    tabela->paginas[idPagina].instanteReferencia = memoria->relogio;
                    tabela->paginas[idPagina].numReferencias = 1;
                }
            }
        }
        
        //incrementar o relogio da memoria
        memoria->relogio++;
    }
    
}


void inicializa_memoria(memoria_t *memoria, unsigned tamanhoPagina, unsigned tamanhoMemoria) {
    int i;
    memoria->numQuadros = tamanhoMemoria/tamanhoPagina;
    memoria->tamanhoMemoria = tamanhoMemoria;
    memoria->quadros = (quadro_t *) malloc( memoria->numQuadros * sizeof(quadro_t) );
    if( memoria->quadros == NULL ) {
        perror("Erro: calloc");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < memoria->numQuadros; i++) {
        memoria->quadros[i].vago = 1;
        memoria->quadros[i].idPagina = -1;
    }
    memoria->paginasLidas = 0;
    memoria->paginasEscritas = 0;
    memoria->relogio = 0;
}

void inicializa_tabela_paginas(tabela_paginas_t *tabela, unsigned tamanhoPagina) {
    int i;
    tabela->numeroPaginas = tamanhoPagina * pow(2, 10);
    tabela->tamanhoPaginas = tamanhoPagina;
    tabela->paginas = (pagina_t *) malloc( tabela->numeroPaginas * sizeof(pagina_t) );
    for(i = 0; i < tabela->numeroPaginas; i++) {
        tabela->paginas[i].carregada = 0;
        tabela->paginas[i].instanteReferencia = -1;
        tabela->paginas[i].numReferencias = 0;
        tabela->paginas[i].quadro = -1;
    }
}

void inicializa_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela, unsigned tamanhoPagina, unsigned tamanhoMemoria) {
    
    //inicializar memoria
    inicializa_memoria(memoria, tamanhoPagina, tamanhoMemoria);
    
    //inicializar tabela de paginas
    inicializa_tabela_paginas(tabela, tamanhoPagina);
    
}

void libera_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela) {
    free(memoria->quadros);
    free(tabela->paginas);
}


