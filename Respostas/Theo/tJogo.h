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
    int numeroComidasObtidas;
} tJogo;


/**
 * @brief Aloca a estrutura do jogo dinamicamente 
 * @param caminhoConfig O caminho para criacao do Mapa
 * @return  Um ponteiro para a estrutura tJogo
 */
tJogo* CriaJogo (const char* caminhoConfig);

/**
 * @brief Funcao onde eh realizado o jogo 
 * @param jogo jogo 
*/
void ExecutaJogo (tJogo* jogo);

/**
 * @brief Retorna um comando de acordo com a entrada do Jogador
 * @return COMANDO do movimento
*/
COMANDO LeComandoTeclado();

/**
 * @brief Realiza Movimentacao dos Fantasmas
 * Atualizando sua Posicao e Atualizando sua Posicao no Mapa
 * @param fantasmas fantasmas do jogo
 * @param mapa mapa do jogo
 */
void MovimentaFantasmas (tFantasma** fantasmas, tMapa* mapa);

/**
 * @brief Realiza a Movimentacao do Pacman e Atualiza Posicao do Pacman no Mapa
 * @param pacman pacman
 * @param mapa mapa
 * @param comando comando do jogador
 */
void MovimentaPacmanMapa (tPacman* pacman, tMapa* mapa, COMANDO comando);

/**
 * @brief Verifica Colisao entre o Pacman e o Fantasma
 * @param comando comando realizado pelo jogador
 * @param fantasmas fantasmas do jogo
 * @param mapa mapa do jogo
 * @param pacman pacman
*/
void VerificaColisao (COMANDO comando, tFantasma** fantasmas, tMapa* mapa, tPacman* pacman);

/** 
 * @brief Atualiza a skin do fantasma no mapa, se na posicao dele estiver uma comida.
 * @param fantasmas Fantasmas do Jogo
 * @param mapa Mapa do Jogo
 */
void AtualizaSkinFantasma (tFantasma** fantasmas, tMapa* mapa);

/**
 * @brief Verifica se o fantasmas esta ocupando posicao de comida
 * @param fantasma um fantasma do jogo
 * @return Retorna true se o fantasma estiver ocupando posicao de comida
 */
bool fantasmaOcupandoComida (tFantasma* fantasma);

/**
 * @brief Verifica se o pacman e o fantasma colidiram por divergencia de movimento. 
 * Por exemplo, ambos ocupam a mesma posicao e sentido de movimento do pacman eh
   contrario ao do fantasma
 * @param comando comando do jogador
 * @param sentido sentido de movimentacao do fantasma
 * @return 
 */
bool PosicoesDivergiram (COMANDO comando, sentidoMovimento sentido);

/**
 *  @brief Verifica se o Pacman Pegou Comida
 *  Verifica o numero do movimento atual eh igual ao numero do ultimo movimento 
 *  significativo que pegou comida
 *  @param pacman 
 *  @return 
*/
bool PacmanPegouComida (tPacman* pacman);

/**
 *  @brief Atualiza o Numero de Comidas que o Pacman Pegou
 *  @param jogo jogo 
*/
void AtualizaComidasObtidas (tJogo* jogo);

/**
 *  @brief Preenche o arquivo saida.txt 
 *  @param file arquivo de saida
 *  @param jogo jogo
 *  @param comando comando do jogador
*/
void PreencheArquivoSaida (FILE* file, tJogo* jogo, COMANDO comando);

/**
 *  @brief Converte a variavel do tipo COMANDO para o tipo CHAR
 *  @param comando comando do jogador
 *  @return caractere que o jogador usou para movimentar o pacman
*/
char ConverteComandoChar (COMANDO comando);

/**
 *  @brief Mata o Pacman e insere novo movimento significativo
 *  @param mapa mapa do jogo
 *  @param pacman pacman
 *  @param comando comando do jogador
*/
void MataPacmanJogo (tMapa* mapa, tPacman* pacman, COMANDO comando);

/**
 *  @brief Verifica se as condicoes de Game Over foram Atendidas
 *  @param jogo jogo
 *  @return retorna true se o jogo acabou
*/
bool AcabouJogo (tJogo* jogo);

/**
 *  @brief Obtem Mapa do Jogo
 *  @param jogo jogo 
 *  @return retorna um ponteiro do tipo tMapa
*/
tMapa* ObtemMapaJogo (tJogo* jogo);

/**
 *  @brief Obtem Pacman do Jogo
 *  @param jogo jogo
 *  @return retorna um ponteiro do tipo tPacman
*/
tPacman* ObtemPacmanJogo (tJogo* jogo);

/**
 *  @brief Obtem fantasmas do jogo
 *  @param jogo jogo
 *  @return retorna um ponteiro de ponteiro do tipo tFantasma
*/
tFantasma** ObtemFantasmasJogo (tJogo* jogo);

/**
 *  @brief Obtem o numero de comidas obtidas 
 *  @param jogo jogo
 *  @return quantidade de comidas obtidas
*/
int ObtemComidasObtidasJogador (tJogo* jogo);

/**
 *  @brief Cria o arquivo de inicializacao
 *  @param jogo jogo
*/
void GeraArquivoInicializacao(tJogo* jogo);

/**
 *  @brief Cria o arquivo de Resumo
 *  @param pacman pacman
*/
void GeraArquivoResumo (tPacman* pacman);

/**
 *  @brief Adiciona a ultima linha do arquivo
 *  Game Over ou se o Jogador Venceu
 *  @param file 
 *  @param jogo 
*/
void FinalizaArquivoSaida(FILE* file, tJogo* jogo);

/**
 *  @brief Cria o Arquivo de Estatísticas
 *  @param pacman pacman
*/
void  GeraArquivoEstatisticas(tPacman* pacman);

/**
 *  @brief Cria o Arquivo de Ranking
 *  @param pacman pacman
*/
void GeraArquivoRanking(tPacman* pacman);

/** 
 *  @brief Orderna o Ranking de Movimentos
 *  @param qtdMovimentos Array da qtdMovimentos
 *  @param qtdComidas Array da qtdComidas
 *  @param qtdColisoes Array de qtdColisoes
 *  @param ranking Array do tipo char do ranking de Movimentos
*/
void OrdernaRanking (int qtdMovimentos[], int qtdComidas[], int qtdColisoes[], char ranking[]);

/**
 *  @brief Libera Memoria do jogo
 *  @param jogo 
*/
void DesalocaJogo (tJogo* jogo);


#endif