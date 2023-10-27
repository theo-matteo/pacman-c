#ifndef TJOGO_H_
#define TJOGO_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tFantasma.h"
#include "tMapa.h"
#include "tPacman.h"
#include "tMovimento.h"
#include "tPosicao.h"
#include "tTunel.h"

typedef struct{
    char diretorio_jogo[1000];
    tPacman *pacman;
    tFantasma **fantasmas;
    tMapa *mapa;
} tJogo;

tJogo* CriaJogo (const char* caminhoConfig);

void ExecutaJogo (tJogo* jogo);

COMANDO LeComandoTeclado ();

tMapa* ObtemMapaJogo (tJogo* jogo);

tPacman* ObtemPacmanJogo (tJogo* jogo);

tFantasma** ObtemFantasmasJogo (tJogo* jogo);

bool VerificaColisao (tPosicao *posAnteriorPacman, tPosicao *posAnteriorFantasma, tFantasma* fantasma, tPacman* pacman);

void MataPacmanJogo (tMapa* mapa, tPacman* pacman);

void AtualizaComidasPacman (tPacman* pacman, COMANDO comando);

void PrintaMapaJogo (tMapa* mapa);

void DesalocaJogo (tJogo* jogo);


#endif