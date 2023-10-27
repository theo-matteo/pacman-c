#include "tJogo.h"
#define NUM_FANTASMAS 4

tJogo* CriaJogo (const char* caminhoConfig) {

    tJogo *jogo = (tJogo *) malloc (sizeof(tJogo));
    if (jogo == NULL) {
        exit(1);
    }

    /* Cria Mapa do Jogo */
    jogo->mapa = CriaMapa(caminhoConfig);

    /* Cria o Pacman */
    jogo->pacman = CriaPacman(ObtemPosicaoItemMapa(ObtemMapaJogo(jogo), '>'));

    /* Alocacao da Lista de Fantasmas */
    jogo->fantasmas = (tFantasma **) malloc (sizeof(tFantasma *) * NUM_FANTASMAS);
    if (jogo->fantasmas == NULL) {
        exit(1);
    }
    
    /* Aloca cada fantasma se ele estiver no Mapa */
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        jogo->fantasmas[i] = CriaFantasma(ObtemMapaJogo(jogo), i);
    }

    return jogo;
}

void ExecutaJogo (tJogo* jogo) {

    while (1) {

        COMANDO comando = LeComandoTeclado();
        MovePacman(ObtemPacmanJogo(jogo), ObtemMapaJogo(jogo), comando);

    }

}

COMANDO LeComandoTeclado () {

    char caractere;
    scanf("%c\n", &caractere);

    if (caractere == 'w') {
        return MOV_CIMA;
    }
    else if (caractere == 's') {
        return MOV_BAIXO;
    }
    else if (caractere == 'a') {
        return MOV_ESQUERDA;
    }
    else if (caractere == 'd') {
        return MOV_DIREITA;
    }

}

tMapa* ObtemMapaJogo (tJogo* jogo) {
    return jogo->mapa;
}

tMapa* ObtemPacmanJogo (tJogo* jogo) {
    return jogo->pacman;
}