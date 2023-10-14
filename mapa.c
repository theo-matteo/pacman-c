#include "tMapa.h"

tMapa* CriaMapa(const char* caminhoConfig) {

    char diretorio[1000];
    sprintf(diretorio, "%s/mapa.txt", caminhoConfig);

    // Abre o arquivo contendo informacoes do mapa, finaliza o programa se houver erro na leitura
    FILE *file = fopen(diretorio, "r");
    if (file == NULL) { 
        printf("ERRO: nao foi possível ler o arquivo %s", diretorio);
        exit(1);
    }

    // Alocaca memoria para o mapa e verifica se foi bem sucedido
    tMapa *mapa = (tMapa *) malloc(sizeof(tMapa));
    if (mapa == NULL) {
        exit(1);
    }

    // Obter o numero maximo de movimentos
    fscanf(file, "%d\n", &mapa->nMaximoMovimentos);
    mapa->nLinhas = 0;
    mapa->nColunas = 0;

    // Obter o numero de linhas e colunas varrendo o arquivo
    char caractere;
    int flagTerminoContagem = 0;
    while ((caractere = fgetc(file)) != EOF) {
        if (caractere == '\n') {
            mapa->nLinhas++;
            flagTerminoContagem = 1;
        }
        else {
            if (!flagTerminoContagem) {
                mapa->nColunas++;
            }
        } 
    }
    mapa->nLinhas++;

    // Retorna o Ponteiro para o Inicio
    rewind(file);

    // Consome a Primeira Linha do Arquivo
    fscanf(file, "%*[^\n]\n"); 

    // Alocacao do Grid
    mapa->grid = (char **) malloc(sizeof(char *) * mapa->nLinhas);
    if (mapa->grid == NULL) {
        exit(1);
    }

    // Alocacao e Obtencao de Linhas do Mapa
    for (int i = 0; i < mapa->nLinhas; i++) {
        mapa->grid[i] = (char *) calloc((mapa->nColunas + 1), sizeof(char));
        if (mapa->grid[i] == NULL) {
            exit(1);
        }
        fscanf(file,"%[^\n]\n", mapa->grid[i]); // Consome uma linha inteira do mapa
    }   
    
    return mapa;
}