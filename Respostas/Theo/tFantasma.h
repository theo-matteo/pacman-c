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

typedef enum {
    B = 0, 
    P = 1, 
    I = 2, 
    C = 3
} skinFantasma;


typedef struct tFantasma {

    /* Skin do Fantasma (B, P, I ou C)*/
    skinFantasma skin;

    /* Posicao Atual do Fantasma (linha, coluna)*/
    tPosicao *posicaoAtual;

    /* Sentido de Movimentacao Inicial */
    sentidoMovimento sentido;

    bool ocupouComida;

} tFantasma;

/* Cria um Fantasma Dinamicamente. Realiza uma varredura no mapa
    buscando o fantasma. Se o fantasma estiver no mapa, realiza alocacao.
    Caso nao esteja, retorna NULL
*/
tFantasma* CriaFantasma (tMapa* mapa, skinFantasma skin);

/*
    Obtem o sentido Inicial do Fantasma quando inicia o jogo. Por exemplo,
    o fantasma P retorna CIMA
*/
sentidoMovimento ObtemSentidoInicialFantasma(skinFantasma skin);

/* Retorna o caractere representante do Fantasma de acordo com a skin, 
   Por exemplo, o fantasma B retorna 'B'
*/
char ObtemCaractereSkin (skinFantasma skin);

/* Retorna a skin do Fantasma*/
skinFantasma ObtemSkinFantasma (tFantasma* fantasma);

/* Realiza movimentacao no Mapa, alem de verificar Sobreposicoes. 
   Se o fantasma for NULL, apenas fecha a funcao prematuramente
*/
void MoveFantasma (tFantasma* fantasma, tMapa* mapa);

/* Retorna a posicao do Fantasma*/
tPosicao* ObtemPosicaoFantasma (tFantasma* fantasma);

/* Retorna a posicao resultante da Movimentacao do Fantasma, Para
   Isso, e realizado uma verificao do sentido de movimentacao do Fantasma
*/
tPosicao* PosicaoPosMovimentoFantasma (tFantasma* fantasma);

/* Altera o sentido de movimentacao do fantasma quando encontra uma parede*/
void AlteraSentidoFantasma (tFantasma *fantasma);

/* Realiza a desalocacao de memoria do fantasma*/
void DesalocaFantasma (tFantasma* fantasma);

/* Retorna o Sentido de Movimentacao Atual do Fantasma */
sentidoMovimento ObtemSentidoFantasma (tFantasma* fantasma);

#endif