#include "tFantasma.h"

sentidoMovimento ObtemSentidoInicialFantasma(char skin) {

    if (skin == 'B') {
        return ESQUERDA;
    }
    else if (skin == 'P') {
        return CIMA;
    }
    else if (skin == 'I') {
        return BAIXO;
    }
    else if (skin == 'C') {
        return DIREITA;
    }

}

tFantasma* CriaFantasma (tMapa* mapa, char skin) {

    /* Verifica se o Fantasma esta no Mapa */
    tPosicao *posicao = ObtemPosicaoItemMapa(mapa, skin);
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

    return fantasma;
}

void MoveFantasma (tFantasma* fantasma, tMapa* mapa) {

    /* Posicao atual do fantasma clonada */
    tPosicao *posicaoAtualFantasma = ClonaPosicao(ObtemPosicaoFantasma(fantasma));

    /* Posicao pos Movimento */
    tPosicao *posicaoNova = PosicaoPosMovimentoFantasma(fantasma);

    bool ocupouTunel = false;

    if (EncontrouParedeMapa(mapa, posicaoNova)) {

        /* Altera o sentido de Movimentacao do Fantasma*/
        AlteraSentidoFantasma(fantasma);
        
        /* Desaloca Posicao Nova e Aloca outra Posicao com Sentido Novo*/
        DesalocaPosicao(posicaoNova);
        posicaoNova = PosicaoPosMovimentoFantasma(fantasma);
    }

    /* Atualiza Posicao do Fantasma */
    AtualizaPosicao(ObtemPosicaoFantasma(fantasma), posicaoNova); 

    /* Verifica se a Posicao Fantasma das Movimentacao sobrepos algum item do Mapa e 
    Devolve ao Mapa */
    if (EncontrouComidaMapa(mapa, posicaoAtualFantasma)) {
        AtualizaItemMapa(mapa, posicaoAtualFantasma, '*');
    }
    else if (PossuiTunelMapa(mapa)) {
        if (AcessouTunelMapa(mapa, posicaoAtualFantasma)) {
           AtualizaItemMapa(mapa, posicaoAtualFantasma, '@'); 
           ocupouTunel = true;
        }
    }
    else if (ocupouTunel == false) {
        AtualizaItemMapa(mapa, posicaoAtualFantasma, ' ');
    }
    
    AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasma), ObtemSkinFantasma(fantasma)); 
    DesalocaPosicao(posicaoAtualFantasma);
    DesalocaPosicao(posicaoNova);
}

tPosicao* ObtemPosicaoFantasma (tFantasma* fantasma) {
    return fantasma->posicaoAtual;
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

sentidoMovimento ObtemSentidoFantasma(tFantasma* fantasma) {
    return fantasma->sentido;
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

char ObtemSkinFantasma (tFantasma* fantasma) {
    return fantasma->skin;
}

void DesalocaFantasma (tFantasma* fantasma) {

    if (fantasma == NULL) {
        return;
    }
    
    DesalocaPosicao(fantasma);
    free(fantasma);
}