#include "tTunel.h"

tTunel* CriaTunel(int linhaAcesso1, int colunaAcesso1, int linhaAcesso2, int colunaAcesso2) {

    tTunel *tunel = (tTunel *) malloc(sizeof(tTunel));
    if (tunel == NULL) {
        exit(1);
    }

    tunel->acesso1 = CriaPosicao(linhaAcesso1, colunaAcesso1);
    tunel->acesso2 = CriaPosicao(linhaAcesso2, colunaAcesso2);

    return tunel;
}

bool EntrouTunel(tTunel* tunel, tPosicao* posicao) {
    if (SaoIguaisPosicao(tunel->acesso1, posicao) || SaoIguaisPosicao(tunel->acesso2, posicao)) {
        return true;
    }
    return false;
}

void LevaFinalTunel(tTunel* tunel, tPosicao* posicao) {

    if (SaoIguaisPosicao(tunel->acesso1, posicao)) {
        AtualizaPosicao(posicao, tunel->acesso2);
        return;
    }

   AtualizaPosicao(posicao, tunel->acesso1);
}

void DesalocaTunel(tTunel* tunel) {

    if (tunel == NULL) {
        return;
    }

    DesalocaPosicao(tunel->acesso1);
    DesalocaPosicao(tunel->acesso2);
    free(tunel);
}