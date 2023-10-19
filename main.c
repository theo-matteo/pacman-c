#include <stdio.h>
#include "tMapa.h"
#include "tMovimento.h"
#include "tPacman.h"
#include "tPosicao.h"
#include "tTunel.h"

int main (int argc, char *argv[]) {
    
    if (argc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
        exit(1);
    }

    tMapa *mapa = CriaMapa(argv[1]);

    printf("%d %d\n", mapa->nColunas, mapa->nLinhas);

    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            printf("%c", mapa->grid[i][j]);
        }
        printf("\n");
    }

    DesalocaMapa(mapa);
    return 0;
}