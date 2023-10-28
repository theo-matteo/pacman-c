#ifndef TJOGO_H_
#define TJOGO_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tFantasma.h"
#include "tMapa.h"
#include "tPacman.h"
#include "tMovimento.h"
#include "tPosicao.h"
#include "tTunel.h"

typedef struct{
    tPacman* pacman;
    tFantasma** fantasmas;
    tMapa* mapa;
} tJogo;

void GeraArquivoInicializacao(tJogo* jogo);

tJogo* CriaJogo (const char* caminhoConfig);

void ExecutaJogo (tJogo* jogo);

FILE* RedirecionamentoSaida ();

void PreencheArquivoSaida (FILE* file, tJogo* jogo, COMANDO comando);

void PreencheDesalocaArquivoSaida (FILE* file, tJogo* jogo);

COMANDO LeComandoTeclado ();

bool AcabouJogo (tJogo* jogo);

tMapa* ObtemMapaJogo (tJogo* jogo);

tPacman* ObtemPacmanJogo (tJogo* jogo);

tFantasma** ObtemFantasmasJogo (tJogo* jogo);

void VerificaColisao (COMANDO comando, tFantasma** fantasmas, tMapa* mapa, tPacman* pacman);

bool PosicoesDivergiram (COMANDO comando, sentidoMovimento sentido);

void MataPacmanJogo (tMapa* mapa, tPacman* pacman, COMANDO comando);

void GeraArquivoEstatisticas(tPacman* pacman);

void AtualizaComidasAtuaisMapa (tMapa* mapa);

bool PacmanPegouComida (tPacman* pacman);

void PrintaMapaJogo (tMapa* mapa);

void GeraArquivoResumo (tPacman* pacman);

void GeraArquivoRanking (tPacman* pacman);

char ConverteComandoChar (COMANDO comando);

void DesalocaJogo (tJogo* jogo);





#endif