#include <stdio.h>
#include "tMapa.h"
#include "tMovimento.h"
#include "tPacman.h"
#include "tPosicao.h"
#include "tTunel.h"
#include "tFantasma.h"
#include "tJogo.h"


int main (int argc, char *argv[]) {
    
    if (argc <= 1) {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
        exit(1);
    }

    tJogo *jogo = CriaJogo(argv[1]);
    ExecutaJogo(jogo);
    DesalocaJogo(jogo);


    return 0;
}

FILE* RedirecionamentoEntrada (const char* caminho) {

    char diretorio[1000];
    sprintf(diretorio, "%s/entrada.txt", caminho);

    FILE *file = freopen(diretorio, "r", stdin);
    if (file == NULL) {
        printf("ERRO: nao foi possível ler o arquivo %s", diretorio);
        exit(1);
    }

    return file;
}

