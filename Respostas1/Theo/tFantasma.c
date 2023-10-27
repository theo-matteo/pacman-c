#include "tFantasma.h"


tFantasma* CriaFantasma (tMapa* mapa, skinFantasma skin) {

    /* Verifica se o Fantasma esta no Mapa */
    tPosicao *posicao = ObtemPosicaoItemMapa(mapa, ObtemCaractereSkin(skin));
    if (posicao == NULL) {
        return NULL;
    }

    tFantasma *fantasma = (tFantasma *) malloc(sizeof(tFantasma));
    if (fantasma == NULL) {
        exit(1);
    }

    /* Inicializa Atributos do Fantasma */
    fantasma->posicaoAtual = posicao;
    fantasma->sentido = ObtemSentidoInicialFantasma(skin);
    fantasma->skin = skin;
    fantasma->ocupouComida = false;

    return fantasma;
}

char ObtemCaractereSkin (skinFantasma skin) {

    if (skin == B) {
        return 'B';
    }
    else if (skin == P) {
        return 'P';
    }
    else if (skin == I) {
        return 'I';
    }
    else if (skin == C) {
        return 'C';
    }

    return '\0';
}

sentidoMovimento ObtemSentidoInicialFantasma(skinFantasma skin) {

    if (skin == B) {
        return ESQUERDA;
    }
    else if (skin == P) {
        return CIMA;
    }
    else if (skin == I) {
        return BAIXO;
    }
    else if (skin == C) {
        return DIREITA;
    }

}

void MoveFantasma (tFantasma* fantasma, tMapa* mapa) {

    /* Verifica se o Fantasma nao foi Alocado (ou seja, nao esta no mapa)*/
    if (fantasma == NULL) {
        return;
    }

     /* Posicao atual do fantasma clonada */
    tPosicao *posicaoAtualFantasma = ClonaPosicao(ObtemPosicaoFantasma(fantasma));

    /* Posicao pos Movimento */
    tPosicao *posicaoNova = PosicaoPosMovimentoFantasma(fantasma);

    /* Altera Sentido do Fantasma se Colidir com uma Parede*/
    if (EncontrouParedeMapa(mapa, posicaoNova)) {
        AlteraSentidoFantasma(fantasma);
        DesalocaPosicao(posicaoNova);
        posicaoNova = PosicaoPosMovimentoFantasma(fantasma);
    }

    /* Verifica se o Fantasma Ocupou Posicao de Comida*/
    if (fantasma->ocupouComida) {
        AtualizaItemMapa(mapa, posicaoAtualFantasma, '*');
        fantasma->ocupouComida = false;
    }
    else {
        if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, posicaoAtualFantasma)) {
            AtualizaItemMapa(mapa, posicaoAtualFantasma, '@'); 
        }
        else if (ObtemItemMapa(mapa, posicaoAtualFantasma) != '>') {
            AtualizaItemMapa(mapa, posicaoAtualFantasma, ' ');
        }
    }

    /* Verifica se o Fantasma (apos Movimentar) ocupou Posicao de Comida*/
    if (EncontrouComidaMapa(mapa, posicaoNova)) {
        fantasma->ocupouComida = true;
    }

    /* Atualiza Posicao do Fantasma e Mapa*/
    AtualizaPosicao(ObtemPosicaoFantasma(fantasma), posicaoNova);
    AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasma), ObtemCaractereSkin(ObtemSkinFantasma(fantasma)));

    /* Desaloca posicoes auxiliares */
    DesalocaPosicao(posicaoAtualFantasma);
    DesalocaPosicao(posicaoNova);

}

tPosicao* PosicaoPosMovimentoFantasma (tFantasma* fantasma) {

    int linha = ObtemLinhaPosicao(ObtemPosicaoFantasma(fantasma));
    int coluna = ObtemColunaPosicao(ObtemPosicaoFantasma(fantasma));
    sentidoMovimento sentido = ObtemSentidoFantasma(fantasma);

    if (sentido == ESQUERDA) {
        return CriaPosicao (linha, coluna - 1);
    }
    else if (sentido == DIREITA) {
        return CriaPosicao(linha, coluna + 1);
    }
    else if (sentido == CIMA) {
        return CriaPosicao(linha - 1, coluna);
    }
    else {
        return CriaPosicao(linha + 1, coluna);
    }
}

sentidoMovimento ObtemSentidoFantasma (tFantasma* fantasma) {
    return fantasma->sentido;
}

tPosicao* ObtemPosicaoFantasma (tFantasma* fantasma) {
    return fantasma->posicaoAtual;
}

skinFantasma ObtemSkinFantasma (tFantasma* fantasma) {
    return fantasma->skin;
}

void AlteraSentidoFantasma (tFantasma *fantasma) {

    sentidoMovimento sentido = ObtemSentidoFantasma(fantasma);

    if (sentido == ESQUERDA) {
        fantasma->sentido = DIREITA;
    }
    else if (sentido == DIREITA) {
        fantasma->sentido = ESQUERDA;
    }
    else if (sentido == CIMA) {
        fantasma->sentido = BAIXO;
    }
    else {
        fantasma->sentido = CIMA;
    }
}

void DesalocaFantasma (tFantasma* fantasma) {

    if (fantasma == NULL) {
        return;
    }
    
    DesalocaPosicao(ObtemPosicaoFantasma(fantasma));
    free(fantasma);
}
