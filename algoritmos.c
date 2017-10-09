
#include "virtual.h"

unsigned seleciona_quadro_lru(memoria_t *memoria, tabela_paginas_t *tabela) {
    unsigned i;
    unsigned id;
    unsigned menosRecente = INF;
    unsigned paginaMenosRecente;
    unsigned quadroSelecionado;
    
    //alocar um quadro na memoria
    for(i = 0; i < memoria->numQuadros; i++) {
        if( memoria->quadros[i].vago == 1 ) return i;
        id = memoria->quadros[i].idPagina;
        if( tabela->paginas[id].instanteReferencia < menosRecente ) {
            paginaMenosRecente = id;
            menosRecente = tabela->paginas[id].instanteReferencia;
        }
    }
    
    quadroSelecionado = tabela->paginas[paginaMenosRecente].quadro;
    tabela->paginas[paginaMenosRecente].carregada = 0;
    memoria->quadros[quadroSelecionado].vago = 0;
    return quadroSelecionado;
}
