
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 999999999

//estrutura da pagina
typedef struct {
    short carregada;            //1 se a pagina esta na memoria e 0 se nao esta
    unsigned quadro;            //indice do quadro de memoria no qual a pagina esta
    int instanteReferencia;     //instante de tempo no qual a pagina foi referenciada
    int numReferencias;         //numero de referencias feitas a essa pagina enquanto na memoria
} pagina_t;

//estrutura da tabela de paginas
typedef struct {
    pagina_t *paginas;          //vetor de paginas
    unsigned numeroPaginas;     //tamanho do vetor
    unsigned tamanhoPaginas;    //tamanho das paginas em kb
} tabela_paginas_t;

//estrutura do quadro de pagina
typedef struct {
    short vago;                //1 se o quadro esta vago e 0 se nao esta
    unsigned idPagina;         //id da pagina que esta carregada no quadro
} quadro_t;

//estrutura da memoria
typedef struct {
    quadro_t *quadros;         //vetor de quadros (divisao da memoria)
    int paginasLidas;          //numero de paginas lidas
    int paginasEscritas;       //numero de paginas que foram carregadas na memoria
    int relogio;               //contador de tempo (incrementado a cada acesso a memoria)
    int numQuadros;            //numero de quadros total da memoria
    unsigned tamanhoMemoria;   //tamanho da memoria em kb
} memoria_t;

/* funcoes de inicializacao e liberacao do sistema de gerencia */

//inicializa a memoria
void inicializa_memoria(memoria_t *memoria, unsigned tamanhoPagina, unsigned tamanhoMemoria);

//inicializa a tabela de paginas
void inicializa_tabela_paginas(tabela_paginas_t *tabela, unsigned tamanhoPagina);

//faz inicializacoes necessarias no sistema de gerencia de memoria
void inicializa_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela, unsigned tamanhoPagina, unsigned tamanhoMemoria);

//libera memoria usada pelo sistema de gerencia de memoria
void libera_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela);


/* funcoes de uso e organizacao do sistema de gerencia */

//funcao auxiliar usada no calculo do id da pagina a partir do endereco
unsigned deslocamento_bits(unsigned tamanhoPagina);

//converte um endereco hexadecimal de acesso em um id de pagina
unsigned idPagina(unsigned endereco, unsigned s);

//le o arquivo log com enderecos de acesso e computa os dados de acordo com o algoritmo de subtituicao usado
void processa_entrada(memoria_t *memoria, tabela_paginas_t *tabela, char *algoritmo, FILE *log);


/* algoritmos de susbtituicao de paginas */

//seleciona um quadro para ser substituido de acordo com a politica lru
unsigned seleciona_quadro_lru(memoria_t *memoria, tabela_paginas_t *tabela);

//seleciona um quadro para ser substituido de acordo com a politica fifo
unsigned seleciona_quadro_fifo(memoria_t *memoria, tabela_paginas_t *tabela);

//seleciona um quadro para ser substituido de acordo com a politica aleatoria
unsigned seleciona_quadro_aleatorio(memoria_t *memoria, tabela_paginas_t *tabela);



