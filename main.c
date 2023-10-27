#include <stdio.h>
#include "tMapa.h"
#include "tMovimento.h"
#include "tPacman.h"
#include "tPosicao.h"
#include "tTunel.h"
#include "tFantasma.h"

int main (int argc, char *argv[]) {
    
    if (argc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
        exit(1);
    }

    tMapa *mapa = CriaMapa(argv[1]);

    tFantasma **fantasmas = (tFantasma **) malloc (sizeof(tFantasma *) * 4);

    for (int i = 0; i < 4; i++) {
        fantasmas[i] = CriaFantasma(mapa, i);
    } 

    int j = 0;

    for (int i = 0; i < mapa->nLinhas; i++) {
        printf("%s\n", mapa->grid[i]);
    }

    while (j < 5) {
        for (int i = 0; i < 4; i++) {
            MoveFantasma(fantasmas[i], mapa);
        }

        printf("\nPOS MOVIMENTO: \n");

        for (int i = 0; i < mapa->nLinhas; i++) {
            printf("%s\n", mapa->grid[i]);
        }
        j++;
    }


    DesalocaMapa(mapa);
    for (int i = 0; i < 4; i++) {
        DesalocaFantasma(fantasmas[i]);
    }
    free(fantasmas);


    
    
    
    return 0;
}