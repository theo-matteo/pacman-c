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
        // Mensagem
        exit(1);
    }

    fscanf(file, "%d %d %d\n", &mapa->nMaximoMovimentos);
    mapa->nLinhas = 0;
    mapa->nColunas = 0;

    mapa->grid = (char **) malloc(sizeof(char *) * (mapa->nLinhas + 1));
    *(mapa->grid) = NULL;

    int size = 0;
    char caractere;

    
    while( (caractere = fgetc(file)) != EOF) {

        if (caractere == '\n') {
            mapa->nLinhas++;
            mapa->grid = realloc(mapa->grid, mapa->nLinhas + 1);
            continue;
        }

    }


    return mapa;
}