#include "tFantasma.h"
#define COMIDA '*'


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
    else {
        return DIREITA;
    }

}

void MoveFantasma (tFantasma* fantasma, tMapa* mapa) {

    /* Verifica se o Fantasma nao foi Alocado (ou seja, nao esta no mapa) */
    if (fantasma == NULL) {
        return;
    }

    /* Verifica se o fantasma ocupou anteriormente posicao de comida e devolve ao mapa */
    DevolveComidaMapaFantasma(fantasma, mapa);

    /* Posicao pos Movimento */
    tPosicao *posicaoNova = PosicaoPosMovimentoFantasma(fantasma);

    /* Altera Sentido do Fantasma se Colidir com uma Parede*/
    if (EncontrouParedeMapa(mapa, posicaoNova) || (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, posicaoNova))) {
        AlteraSentidoFantasma(fantasma);
        DesalocaPosicao(posicaoNova);
        posicaoNova = PosicaoPosMovimentoFantasma(fantasma);
    }

    /* Atualiza posicao do fantasma */
    AtualizaPosicao(ObtemPosicaoFantasma(fantasma), posicaoNova);

    /* Verifica se o fantasma apos movimentar ocupou posicao de comida, se nao atualiza skin do fantasma no mapa */
    if (EncontrouComidaMapa(mapa, ObtemPosicaoFantasma(fantasma))) {
        fantasma->ocupouComida = true;
    }

    /* Desaloca Posicao Auxiliar */
    DesalocaPosicao(posicaoNova);
}

void DevolveComidaMapaFantasma (tFantasma* fantasma, tMapa* mapa) {

    if (fantasma->ocupouComida) {
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasma), COMIDA);
        fantasma->ocupouComida = false;
    }
    else {
        AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasma), ' ');
    }

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
