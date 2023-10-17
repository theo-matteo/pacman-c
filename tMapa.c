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
    mapa->tunel = NULL;

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

    // Retorna o Ponteiro para o Inicio do Arquivo
    rewind(file);

    // Consome a Primeira Linha do Arquivo
    fscanf(file, "%*[^\n]\n"); 

    // Alocacao do Grid
    mapa->grid = (char **) malloc(sizeof(char *) * mapa->nLinhas);
    if (mapa->grid == NULL) {
        exit(1);
    }

    // Alocacao do Conteudo do Mapa
    for (int i = 0; i < mapa->nLinhas; i++) {
        mapa->grid[i] = (char *) calloc((mapa->nColunas + 1), sizeof(char));
        if (mapa->grid[i] == NULL) {
            exit(1);
        }
        fscanf(file,"%[^\n]\n", mapa->grid[i]); // Consome uma linha inteira do mapa
    }   

    // Obtem o numero de comidas presentes no mapa
    mapa->nFrutasAtual = ObtemQuantidadeFrutasIniciaisMapa(mapa);


    // Verifica se ha tunel no mapa
    if (PossuiTunelMapa(mapa)) {
        
    }
    
    fclose(file);
    return mapa;
}

tPosicao* ObtemPosicaoItemMapa(tMapa* mapa, char item) {
    
    tPosicao *posicao = NULL;

    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            if (mapa->grid[i][j] == item) {
                posicao = CriaPosicao(i, j);
            } 
        }
    }

    return posicao;
}

tTunel* ObtemTunelMapa(tMapa* mapa) {
    return mapa->tunel;
}

char ObtemItemMapa(tMapa* mapa, tPosicao* posicao) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    return mapa->grid[linha][coluna];
}

int ObtemNumeroLinhasMapa(tMapa* mapa) {
    return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa* mapa) {
    return mapa->nColunas;
}

int ObtemQuantidadeFrutasIniciaisMapa(tMapa* mapa) {
    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            if (mapa->grid[i][j] == '*') {
                mapa->nFrutasAtual++;
            }
        }
    }
    return mapa->nFrutasAtual;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa* mapa) {
    return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa* mapa, tPosicao* posicao) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    if (mapa->grid[linha][coluna] == '*') {
        return true;
    }

    return false;
}

bool EncontrouParedeMapa(tMapa* mapa, tPosicao* posicao) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    if (mapa->grid[linha][coluna] == '#') {
        return true;
    }

    return false;
}

bool AtualizaItemMapa(tMapa* mapa, tPosicao* posicao, char item) {
    

}

bool PossuiTunelMapa(tMapa* mapa) {
    
    tPosicao *posicao = ObtemPosicaoItemMapa(mapa, '@');
    if (posicao == NULL) {
        return false;
    }

    return true;
}

bool AcessouTunelMapa(tMapa* mapa, tPosicao* posicao) {
    return EntrouTunel(mapa->tunel, posicao);
}

void EntraTunelMapa(tMapa* mapa, tPosicao* posicao) {

}

void DesalocaMapa(tMapa* mapa) {

    for (int i = 0; i < mapa->nLinhas; i++) {
        free(mapa->grid[i]);
    }
    free(mapa->grid);

    if (mapa->tunel != NULL) {
        DesalocaTunel(mapa->tunel);
    }
}