#include "tPosicao.h"

tPosicao* CriaPosicao(int linha, int coluna) {

    tPosicao *posicao = (tPosicao *) malloc(sizeof(tPosicao));
    if (posicao == NULL) {
        exit(1);
    }

    posicao->linha = linha;
    posicao->coluna = coluna;

    return posicao;
}

tPosicao* ClonaPosicao(tPosicao* posicao) {
    tPosicao *posicaoClonada = CriaPosicao(posicao->linha, posicao->coluna);
    return posicaoClonada;
}

int ObtemLinhaPosicao(tPosicao* posicao) {
    return posicao->linha;
}

int ObtemColunaPosicao(tPosicao* posicao) {
    return posicao->coluna;
}

void AtualizaPosicao(tPosicao* posicaoAtual, tPosicao* posicaoNova) {

    // Armazena ponteiro da posicao atual
    tPosicao *temp = posicaoAtual;

    // Atualiza posicao com novo ponteiro
    posicaoAtual = posicaoNova;

    // Desaloca ponteiro da posicao atual (que agora e a anterior)
    DesalocaPosicao(temp);
}

bool SaoIguaisPosicao(tPosicao* posicao1, tPosicao* posicao2) {
    if (posicao1->linha == posicao2->linha) {
        if (posicao1->coluna == posicao2->coluna) {
            return true;
        }
    }
    return false;
}

void DesalocaPosicao(tPosicao* posicao) {
    free(posicao);
}