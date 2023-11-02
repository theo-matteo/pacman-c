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

/**
 *  @brief Cria um Fantasma Dinamicamente
 *  Verifica se o Fantasma esta no mapa. Se nao estiver no mapa, retorna NULL
 *  @param mapa 
 *  @param skin 
 *  @return 
*/
tFantasma* CriaFantasma (tMapa* mapa, skinFantasma skin);

/**
 *  @brief Obtem o sentido Inicial do Fantasma quando inicia o jogo
 *  @param skin skin do fantasma
 *  @return 
*/
sentidoMovimento ObtemSentidoInicialFantasma(skinFantasma skin);

/**
 *  @brief Retorna o caractere representante do Fantasma de acordo com a skin
 *  @param skin skin do fantasma
 *  @return caractere que eh a skin do fantasma
*/
char ObtemCaractereSkin (skinFantasma skin);

/**
 *  @brief Obtem skin do fantasma
 *  @param fantasma fantasma
 *  @return Retorna a skin do Fantasma
*/
skinFantasma ObtemSkinFantasma (tFantasma* fantasma);

/**
 *  @brief Move Fantasma, atulizando sua posicao e atualizando o mapa
 *  @param fantasma fantasma
 *  @param mapa mapa do jogo
*/
void MoveFantasma (tFantasma* fantasma, tMapa* mapa);

/**
 *  @brief Obtem a posicao do fantasma
 *  @param fantasma fantasma
 *  @return Retorna a posicao do Fantasma
*/
tPosicao* ObtemPosicaoFantasma (tFantasma* fantasma);

/**
 *  @brief Cria uma posicao de acordo com o movimento do fantasma
 *  @param fantasma fantasma
 *  @return Retorna a posicao resultante da movimentacao do fantasma
*/
tPosicao* PosicaoPosMovimentoFantasma (tFantasma* fantasma);

/**
 *  @brief Altera o sentido de movimentacao do fantasma quando encontra uma parede
 *  @param fantasma fantasma
*/
void AlteraSentidoFantasma (tFantasma *fantasma);

/**
 *  @brief Realiza a desalocacao de memoria do fantasma
 *  @param fantasma fantasma
*/
void DesalocaFantasma (tFantasma* fantasma);

/**
 *  @brief Obtem o sentido de movimentacao do fantasma
 *  @param fantasma fantasma
 *  @return Retorna o sentido de movimentacao atual do fantasma
*/
sentidoMovimento ObtemSentidoFantasma (tFantasma* fantasma);

#endif