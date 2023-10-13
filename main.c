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

}