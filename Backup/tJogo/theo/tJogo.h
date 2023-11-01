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

typedef struct {
    tPacman* pacman;
    tFantasma** fantasmas;
    tMapa* mapa;
    int NumeroComidasObtidas;
} tJogo;


void AtualizaSkinFantasma (tFantasma** fantasmas, tMapa* mapa);

/* Fantasma Ocupando Comida */
bool fantasmaOcupandoComida (tFantasma* fantasma);

/* Realiza Movimentacao dos Fantasmas */
void MovimentaFantasmas (tFantasma** fantasmas, tMapa* mapa);

/* Movimenta Pacman */
void MovimentaPacmanMapa (tPacman* pacman, tMapa* mapa, COMANDO comando);

bool PosicoesDivergiram (COMANDO comando, sentidoMovimento sentido);

char ConverteComandoChar (COMANDO comando);

bool PacmanPegouComida (tPacman* pacman);

void AtualizaComidasObtidas (tJogo* jogo);

void VerificaColisao (COMANDO comando, tFantasma** fantasmas, tMapa* mapa, tPacman* pacman);

void PreencheArquivoSaida (FILE* file, tJogo* jogo, COMANDO comando);

tJogo* CriaJogo (const char* caminhoConfig);

void ExecutaJogo (tJogo* jogo);

COMANDO LeComandoTeclado();

void MataPacmanJogo (tMapa* mapa, tPacman* pacman, COMANDO comando);

bool AcabouJogo (tJogo* jogo);

tMapa* ObtemMapaJogo (tJogo* jogo);

tPacman* ObtemPacmanJogo (tJogo* jogo);

tFantasma** ObtemFantasmasJogo (tJogo* jogo);

void GeraArquivoInicializacao(tJogo* jogo);

void GeraArquivoResumo (tPacman* pacman);

void FinalizaArquivoSaida(FILE* file, tJogo* jogo);

void  GeraArquivoEstatisticas(tPacman* pacman);

void GeraArquivoRanking(tPacman* pacman);

void DesalocaJogo (tJogo* jogo);



#endif