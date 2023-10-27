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
    tPacman *pacman;
    tFantasma **fantasmas;
    tMapa *mapa;
} tJogo;

tJogo* CriaJogo (const char* caminhoConfig);

void ExecutaJogo (tJogo* jogo);

int AcabouJogo (tJogo* jogo);

void DesalocaJogo (tJogo* jogo);

tMapa* ObtemMapaJogo (tJogo* jogo);

tMapa* ObtemPacmanJogo (tJogo* jogo);

COMANDO LeComandoTeclado ();


#endif