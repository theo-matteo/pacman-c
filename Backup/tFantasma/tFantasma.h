#ifndef TFANTASMA_H_
#define TFANTASMA_H_

#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"


typedef enum {
    ESQUERDA = 0, 
    CIMA = 1, 
    BAIXO = 2, 
    DIREITA = 3
} sentidoMovimento;


typedef struct tFantasma {

    /* Skin do Fantasma (B, P, I ou C)*/
    char skin;

    /* Posicao Atual do Fantasma (linha, coluna)*/
    tPosicao *posicaoAtual;

    /* Sentido de Movimentacao Inicial */
    sentidoMovimento sentido;

} tFantasma;

/* Cria um Fantasma Dinamicamente*/
tFantasma* CriaFantasma (tMapa* mapa, char skin);

/* Move o Fantasma pelo Mapa */
void MoveFantasma (tFantasma* fantasma, tMapa* mapa);

/* Altera o sentido do Fantasma quando encontra parede */
void AlteraSentidoFantasma (tFantasma *fantasma);

/* Retorna o sentido Inicial de Acordo com a Skin*/
sentidoMovimento ObtemSentidoInicialFantasma(char skin);

tPosicao* ObtemPosicaoFantasma (tFantasma* fantasma);

tPosicao* PosicaoPosMovimentoFantasma (tFantasma* fantasma);

sentidoMovimento ObtemSentidoFantasma(tFantasma* fantasma);

char ObtemSkinFantasma (tFantasma* fantasma);

void DesalocaFantasma (tFantasma* fantasma);

#endif