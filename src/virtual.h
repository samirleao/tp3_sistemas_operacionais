
#include <stdio.h>
#include <stdlib.h>

#define INF 999999999

//estrutura da pagina
typedef struct {
    short		suja;
    short 		carregada;
    unsigned long	quadro;
    unsigned long	instanteCarregamento;
    long int		instanteAcesso;
} pagina_t;

//estrutura de um frame
typedef struct {
	short		ocupado;
	unsigned long	pagina;
} quadro_t;

//estrutura da tabela de paginas
typedef struct {
    pagina_t		*paginas;         //vetor de paginas
    unsigned long	numeroPaginas;    //tamanho do vetor
    unsigned		tamanhoPaginas;   //tamanho das paginas em kb
} tabela_paginas_t;

//estrutura da memoria
typedef struct {
    unsigned paginasLidas;          //numero de paginas lidas
    unsigned paginasEscritas;       //numero de paginas que foram escritas
    unsigned paginasSujasRemovidas; //paginas sujas que devem ser regravadas no disco
    unsigned numQuadros;			//numero de quadros total da memoria
    unsigned numQuadrosOcupados;    //quantidade de quadros ocupados no momento
    unsigned tamanhoMemoria;        //tamanho da memoria em kb
    unsigned relogio;               //contador de acessos da memoria
    quadro_t *quadros;				//quadros da memoria
} memoria_t;

/* funcoes de inicializacao e liberacao do sistema de gerencia */

//faz inicializacoes necessarias no sistema de gerencia de memoria
void inicializa_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela, int tamanhoPagina, int tamanhoMemoria);

//libera memoria usada pelo sistema de gerencia de memoria
void libera_sistema_memoria(memoria_t *memoria, tabela_paginas_t *tabela);


/* funcoes de uso e organizacao do sistema de gerencia */

int deslocamento_bits(unsigned tamanhoPagina);

unsigned long pagina(unsigned endereco, int s);

void processa_entrada(memoria_t *memoria, tabela_paginas_t *tabela, char *algoritmo, FILE *log);

void carrega_pagina(memoria_t *memoria, tabela_paginas_t *tabela, unsigned long pagina, unsigned long quadro);

void libera_quadro(memoria_t *memoria, tabela_paginas_t *tabela, unsigned long quadro);

int pagina_carregada(unsigned long pagina, tabela_paginas_t *tabela);

int quadro_ocupado(unsigned long quadro, memoria_t *memoria);


/* algoritmos de susbtituicao de paginas */

//seleciona um quadro para ser substituido de acordo com a politica fifo
unsigned long aloca_quadro_fifo(memoria_t *memoria, tabela_paginas_t *tabela);

//seleciona um quadro para ser substituido de acordo com a politica lru
unsigned long aloca_quadro_lru(memoria_t *memoria, tabela_paginas_t *tabela);

//seleciona um quadro para ser substituido de acordo com a politica aleatoria
unsigned long aloca_quadro_random(memoria_t *memoria);



