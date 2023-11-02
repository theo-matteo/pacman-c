#include "tMapa.h"
#define COMIDA '*'
#define PORTAL '@'
#define PAREDE '#'

/* Realiza Contagem de Comidas Iniciais do Mapa*/
void ContagemNumeroComidas (tMapa *mapa) {
    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            if (mapa->grid[i][j] == COMIDA) {
                mapa->nFrutasAtual++;
            }
        }
    }
}

/* Verifica se o Mapa possui Tunel, se tiver Realiza Alocacao */
void ProcuraCriaTunelMapa(tMapa *mapa) {

    int linha1 = -1, linha2 = -1;
    int coluna1 = -1, coluna2 = -1;
    bool found = false;

    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            if (mapa->grid[i][j] == PORTAL) {
                if (linha1 == -1 && coluna1 == -1) {
                    linha1 = i;
                    coluna1 = j;
                }
                else {
                    linha2 = i;
                    coluna2 = j;
                    found = true;
                }
            } 
        }
    }

    if (found == true) {
        mapa->tunel = CriaTunel(linha1, coluna1, linha2, coluna2);
    }

}

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

    // Inicializacao Variaveis do Mapa
    mapa->nLinhas = 0;
    mapa->nColunas = 0;
    mapa->nFrutasAtual = 0;
    mapa->tunel = NULL;

   
    /* Realiza Contagem de Linhas e Colunas */
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

    // Retorna o Ponteiro para o Inicio
    rewind(file);

    // Consome a Primeira Linha do Arquivo
    fscanf(file, "%*[^\n]\n"); 

    // Alocacao do Grid
    mapa->grid = (char **) malloc(sizeof(char *) * mapa->nLinhas);
    if (mapa->grid == NULL) {
        exit(1);
    }

    // Aloca as informacoes do mapa.txt no grid
    for (int i = 0; i < mapa->nLinhas; i++) {
        mapa->grid[i] = (char *) calloc((mapa->nColunas + 1), sizeof(char));
        if (mapa->grid[i] == NULL) {
            exit(1);
        }
        fscanf(file,"%[^\n]\n", mapa->grid[i]); 
    }   

   
    ContagemNumeroComidas(mapa);
    ProcuraCriaTunelMapa(mapa);
    fclose(file);
    
    return mapa;
}

tPosicao* ObtemPosicaoItemMapa(tMapa* mapa, char item) {

    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            if (mapa->grid[i][j] == item) {
                tPosicao *posicao = CriaPosicao(i, j);
                return posicao;
            } 
        }
    }

    return NULL;
}

tTunel* ObtemTunelMapa(tMapa* mapa) {
    return mapa->tunel;
}   

char ObtemItemMapa(tMapa* mapa, tPosicao* posicao) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    if (mapa == NULL || mapa->grid == NULL) {
        return '\0';
    }
    else if (linha >= ObtemNumeroLinhasMapa(mapa) || linha < 0) {
        return '\0';
    }
    else if (coluna >= ObtemNumeroColunasMapa(mapa) || coluna < 0) {
        return '\0';
    }

    return mapa->grid[linha][coluna];
}

int ObtemNumeroLinhasMapa(tMapa* mapa) {
    return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa* mapa) {
    return mapa->nColunas;
}

int ObtemQuantidadeFrutasIniciaisMapa(tMapa* mapa) {
    return mapa->nFrutasAtual;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa* mapa) {
    return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa* mapa, tPosicao* posicao) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    if (mapa == NULL || mapa->grid == NULL) {
        return false;
    }
    else if (linha >= ObtemNumeroLinhasMapa(mapa) || linha < 0) {
        return false;
    }
    else if (coluna >= ObtemNumeroColunasMapa(mapa) || coluna < 0) {
        return false;
    }
    else if (mapa->grid[linha][coluna] != COMIDA) {
        return false;
    }

    return true;
}

bool EncontrouParedeMapa(tMapa* mapa, tPosicao* posicao) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    if (mapa == NULL || mapa->grid == NULL) {
        return false;
    }
    else if (linha >= ObtemNumeroLinhasMapa(mapa) || linha < 0) {
        return false;
    }
    else if (coluna >= ObtemNumeroColunasMapa(mapa) || coluna < 0) {
        return false;
    }
    else if (mapa->grid[linha][coluna] != PAREDE) {
        return false;
    }

    return true;
}

bool AtualizaItemMapa(tMapa* mapa, tPosicao* posicao, char item) {

    int linha = ObtemLinhaPosicao(posicao);
    int coluna = ObtemColunaPosicao(posicao);

    if (mapa == NULL || mapa->grid == NULL) {
        return false;
    }
    else if (linha >= ObtemNumeroLinhasMapa(mapa) || linha < 0) {
        return false;
    }
    else if (coluna >= ObtemNumeroColunasMapa(mapa) || coluna < 0) {
        return false;
    }

    mapa->grid[linha][coluna] = item;   
    return true;
}

bool PossuiTunelMapa(tMapa* mapa) {
    
    if (ObtemTunelMapa(mapa) == NULL) {
        return false;
    }

    return true;
}

bool AcessouTunelMapa(tMapa* mapa, tPosicao* posicao) {
    return EntrouTunel(ObtemTunelMapa(mapa), posicao);
}

void EntraTunelMapa(tMapa* mapa, tPosicao* posicao) {
    LevaFinalTunel(ObtemTunelMapa(mapa), posicao);
}

void DesalocaMapa(tMapa* mapa) {

    // Verifica se o mapa foi alocado
    if (mapa == NULL) {
        return;
    }

    // Desaloca Grid
    if (mapa->grid != NULL) {
        for (int i = 0; i < mapa->nLinhas; i++) {
            free(mapa->grid[i]);
        }
        free(mapa->grid);
    }


    // Desaloca Tunel do Mapa
    if (PossuiTunelMapa(mapa)) {
        DesalocaTunel(ObtemTunelMapa(mapa));
    }

    // Desaloca Mapa
    free(mapa);
}