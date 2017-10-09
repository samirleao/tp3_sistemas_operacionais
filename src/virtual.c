
#include <string.h>
#include <time.h>
#include <math.h>
#include "virtual.h"

int deslocamento_bits(unsigned tamanhoPagina) {
    unsigned tmp = tamanhoPagina * 1024;
    int s = 0;
	while( tmp > 1 ) {
		tmp = tmp >> 1;
		s++;
	}
    return s;
}

unsigned long pagina(unsigned endereco, int s) {
	return (endereco >> s);
}

int pagina_carregada(unsigned long idPagina, tabela_paginas_t *tabela) {
	return tabela->paginas[idPagina].carregada;
}

int quadro_ocupado(unsigned long idQuadro, memoria_t *memoria) {
	return memoria->quadros[idQuadro].ocupado;
}

void carrega_pagina(memoria_t *memoria, tabela_paginas_t *tabela, unsigned long pagina, unsigned long quadro) {
    
    //atualizar informacoes na tabela de paginas
    tabela->paginas[pagina].carregada = 1;
    tabela->paginas[pagina].quadro = quadro;
    tabela->paginas[pagina].suja = 0;
	tabela->paginas[pagina].instanteCarregamento = memoria->relogio;
    tabela->paginas[pagina].instanteAcesso = memoria->relogio;
    
    //atualizar informacoes na memoria
	memoria->quadros[quadro].pagina = pagina;
	memoria->quadros[quadro].ocupado = 1;
    
	memoria->relogio++;
	memoria->numQuadrosOcupados++;
}

void libera_quadro(memoria_t *memoria, tabela_paginas_t *tabela, unsigned long quadro) {
	
	if( quadro_ocupado(quadro, memoria) ) {
		unsigned long pagina = memoria->quadros[quadro].pagina;
		if( tabela->paginas[pagina].suja ) memoria->paginasSujasRemovidas++;
    
		//atualizar informacoes da tabela
		tabela->paginas[pagina].carregada = 0;
		tabela->paginas[pagina].instanteAcesso = 0;
		tabela->paginas[pagina].instanteCarregamento = -1;
		tabela->paginas[pagina].quadro = 0;
		tabela->paginas[pagina].suja = 0;
    
		//atualizar informacoes da memoria
		memoria->quadros[quadro].ocupado = 0;
		memoria->quadros[quadro].pagina = 0;
	
		memoria->numQuadrosOcupados--;
	}
}

void processa_entrada(memoria_t *memoria, tabela_paginas_t *tabela, char *algoritmo, FILE *log){
    int endereco;
    char rw;
    int s;
    unsigned long quadro;
	unsigned long idPagina;
    
    s = deslocamento_bits(tabela->tamanhoPaginas);
    
    //ler cada linha do arquivo de log
    while( !feof(log) ) {
        fscanf(log, "%x %c", &endereco, &rw);
        idPagina = pagina(endereco, s);
        if( !feof(log) ) {
			
			//se a pagina ja esta carregada entao faca o acesso normalmente
			if( pagina_carregada(idPagina, tabela) ) {
				if( rw == 'W' ) tabela->paginas[idPagina].suja = 1;
				tabela->paginas[idPagina].instanteAcesso = memoria->relogio;
			}
        
			//se a pagina nao esta na memoria entao aloque um quadro para a mesma
			else {
				memoria->paginasLidas++;

				//politica fifo
				if( strcmp(algoritmo, "fifo") == 0 ) {
					quadro = aloca_quadro_fifo(memoria, tabela);
				}
				
				//politica lru
				else if( strcmp(algoritmo, "lru") == 0 ) {
					quadro = aloca_quadro_lru(memoria, tabela);
				}

				//politica aleatoria
				else if( strcmp(algoritmo, "random") == 0 ) {
					quadro = aloca_quadro_random(memoria);
				}

				//liberar o quadro
				libera_quadro(memoria, tabela, quadro);
        
				//carregue a pagina no quadro
				carrega_pagina(memoria, tabela, idPagina, quadro);
                if( rw == 'W' ) tabela->paginas[idPagina].suja = 1;
			}
		}
    }
    
}

void inicializa_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela, int tamanhoPagina, int tamanhoMemoria) {
    
    //inicializar memoria
    unsigned long i;
    memoria->numQuadros = tamanhoMemoria/tamanhoPagina;
    memoria->numQuadrosOcupados = 0;
    memoria->tamanhoMemoria = tamanhoMemoria;
    memoria->paginasLidas = 0;
    memoria->paginasEscritas = 0;
    memoria->relogio = 0;
    memoria->paginasSujasRemovidas = 0;
    memoria->quadros = (quadro_t *) malloc(memoria->numQuadros * sizeof(quadro_t) );
    for(i = 0; i < memoria->numQuadros; i++) {
		memoria->quadros[i].ocupado = 0;
	}
    
    //inicializar tabela de paginas
    tabela->numeroPaginas = (pow(2,32)/(tamanhoPagina * pow(2, 10)));
    tabela->tamanhoPaginas = tamanhoPagina;
    tabela->paginas = (pagina_t *) malloc( tabela->numeroPaginas * sizeof(pagina_t) );
    for(i = 0; i < tabela->numeroPaginas; i++) {
        tabela->paginas[i].carregada = 0;
        tabela->paginas[i].suja = 0;
        tabela->paginas[i].quadro = 0;
        tabela->paginas[i].instanteAcesso = -1;
        tabela->paginas[i].instanteCarregamento = 0;
    }
    
}

void libera_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela) {
    free(tabela->paginas);
    free(memoria->quadros);
}

unsigned long aloca_quadro_fifo(memoria_t *memoria, tabela_paginas_t *tabela) {
    unsigned long	i;
    unsigned long	tempoMinimo = INF;
    unsigned long	quadroEscolhido = 0;
	unsigned long	tmp;
    
    //se a memoria nao estiver cheia aloca o primeiro quadro livre
    if( memoria->numQuadrosOcupados < memoria->numQuadros ) {
        for(i = 0; i < memoria->numQuadros; i++) {
			if( !memoria->quadros[i].ocupado ) return i;
        }
    }
    
    //se estiver cheia entao escolhe quadro com pagina carregada a mais tempo
    for(i = 0; i < memoria->numQuadros; i++) {
		tmp = memoria->quadros[i].pagina;
        if( tabela->paginas[tmp].instanteCarregamento < tempoMinimo ) {
            tempoMinimo = tabela->paginas[tmp].instanteCarregamento;
            quadroEscolhido = i;
        }
    }
    
    return quadroEscolhido;
    
}

unsigned long aloca_quadro_lru(memoria_t *memoria, tabela_paginas_t *tabela) {
	unsigned long	i;
    unsigned long	tempoMinimo = INF;
    unsigned long	quadroEscolhido = 0;
	unsigned long	tmp;
    
    //se a memoria nao estiver cheia aloca o primeiro quadro livre
    if( memoria->numQuadrosOcupados < memoria->numQuadros ) {
        for(i = 0; i < memoria->numQuadros; i++) {
			if( !memoria->quadros[i].ocupado ) return i;
        }
    }
    
    //se estiver cheia entao escolhe quadro com pagina que foi usada a mais tempo
    for(i = 0; i < memoria->numQuadros; i++) {
		tmp = memoria->quadros[i].pagina;
		if( tabela->paginas[tmp].instanteAcesso < tempoMinimo ) {
			tempoMinimo = tabela->paginas[tmp].instanteAcesso;
            quadroEscolhido = i;
        }
    }
    
    return quadroEscolhido;
    
}

unsigned long aloca_quadro_random(memoria_t *memoria) {
	srand( time(NULL) );
	return ( rand() % memoria->numQuadros );
}

