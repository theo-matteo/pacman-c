#include "tJogo.h"
#define NUM_FANTASMAS 4

tJogo* CriaJogo (const char* caminhoConfig) {

    tJogo *jogo = (tJogo *) malloc (sizeof(tJogo));
    if (jogo == NULL) {
        exit(1);
    }

    /* Cria Mapa do Jogo */
    jogo->mapa = CriaMapa(caminhoConfig);

    /* Cria o Pacman */
    jogo->pacman = CriaPacman(ObtemPosicaoItemMapa(ObtemMapaJogo(jogo), '>'));

    /* Alocacao da Lista de Fantasmas */
    jogo->fantasmas = (tFantasma **) malloc (sizeof(tFantasma *) * NUM_FANTASMAS);
    if (jogo->fantasmas == NULL) {
        exit(1);
    }
    
    /* Aloca cada fantasma se ele estiver no Mapa */
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        jogo->fantasmas[i] = CriaFantasma(ObtemMapaJogo(jogo), i);
    }

    return jogo;
}

void ExecutaJogo (tJogo* jogo) {

    /* Flag para Indicar fim de jogo */
    bool colidiuFantasma = false;

    tFantasma **fantasmas = ObtemFantasmasJogo(jogo);
    tPacman *pacman = ObtemPacmanJogo(jogo);
    tMapa *mapa = ObtemMapaJogo(jogo);

    /* Cria Arquivo de Inicializacao Jogo */
    GeraArquivoInicializacao(jogo);

    /* Cria e Inicializa Trilha do Pacman */
    CriaTrilhaPacman(pacman, ObtemNumeroLinhasMapa(mapa), ObtemNumeroColunasMapa(mapa));

    while (1) {

        PrintaMapaJogo(mapa);

        /* Obtem Comando do Jogador */
        COMANDO comando = LeComandoTeclado();

        /* Clona Posicao do Pacman antes da Movimentacao */
        tPosicao *posAnteriorPacman = ClonaPosicao(ObtemPosicaoPacman(pacman));

        /* Realiza Movimentacao do Pacman e Atualiza no Mapa */
        MovePacman(pacman, mapa, comando);

        /* Realiza a Movimentacao dos Fantasmas Presentes no Mapa */
        for (int i = 0; i < NUM_FANTASMAS; i++) {

            if (fantasmas[i] == NULL) {
                continue;
            }

            /* Clona Posicao do Fantasma antes da Movimentacao */
            tPosicao *posAnteriorFantasma = ClonaPosicao(ObtemPosicaoFantasma(fantasmas[i]));

            /* Realiza Movimentacao do Fantasma */
            MoveFantasma(fantasmas[i], mapa);

            /* Verifica se houve colisao entre o Pacman e o Fantasma */
            if (!colidiuFantasma && VerificaColisao(posAnteriorPacman, posAnteriorFantasma, fantasmas[i], pacman)) {
                MataPacmanJogo(mapa, pacman);
                colidiuFantasma = true;
            }

            DesalocaPosicao(posAnteriorFantasma);
        }

        /* Desaloca Posicao Anterior do Pacman */
        DesalocaPosicao(posAnteriorPacman);

        /* Verifica se o Fantasma Devolveu Comida na Posicao Presente do Pacman */
        AtualizaPacMapaFantDevolveComida (pacman, mapa, comando);


        if (AcabouJogo(jogo) || colidiuFantasma) {
            if (colidiuFantasma) {
                printf("MORREU PRO GHOST\n");
            }
            break;
        }

    }

    PrintaMapaJogo(mapa);

}


bool AcabouJogo (tJogo* jogo) {

    tPacman* pacman = ObtemPacmanJogo(jogo);
    tMapa* mapa = ObtemMapaJogo(jogo);

    if (ObtemNumeroAtualMovimentosPacman(pacman) > ObtemNumeroMaximoMovimentosMapa(mapa)) {
        return true;
    }
    else if (mapa->nFrutasAtual == 0) {
        return true;
    }

    return false;
}

COMANDO LeComandoTeclado () {

    char caractere;
    scanf(" %c", &caractere);

    if (caractere == 'w') {
        return MOV_CIMA;
    }
    else if (caractere == 's') {
        return MOV_BAIXO;
    }
    else if (caractere == 'a') {
        return MOV_ESQUERDA;
    }
    else if (caractere == 'd') {
        return MOV_DIREITA;
    }

}

tMapa* ObtemMapaJogo (tJogo* jogo) {
    return jogo->mapa;
}

tPacman* ObtemPacmanJogo (tJogo* jogo) {
    return jogo->pacman;
}

tFantasma** ObtemFantasmasJogo (tJogo* jogo) {
    return jogo->fantasmas;
}

bool VerificaColisao (tPosicao *posAnteriorPacman, tPosicao *posAnteriorFantasma, tFantasma* fantasma, tPacman* pacman) {

    if (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoFantasma(fantasma))) {
        return true;
    }
    else if (SaoIguaisPosicao(ObtemPosicaoFantasma(fantasma), posAnteriorPacman)) {
        if (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), posAnteriorFantasma)) {
            return true;
        }
    }

   return false;
}

void MataPacmanJogo (tMapa* mapa, tPacman* pacman) {

    if (ObtemItemMapa(mapa, ObtemPosicaoPacman(pacman)) == '>') {
        AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), ' ');
    }

    MataPacman(pacman);
}

void AtualizaComidasPacman (tPacman* pacman, COMANDO comando) {
    if (comando == MOV_ESQUERDA) {
        pacman->nFrutasComidasEsquerda++;; 
    }
    else if (comando == MOV_DIREITA) {
        pacman->nFrutasComidasDireita++;
    }
    else if (comando == MOV_CIMA) {
        pacman->nFrutasComidasCima++;
    }
    else if (comando == MOV_BAIXO) {
        pacman->nFrutasComidasBaixo++;
    }
}

void PrintaMapaJogo (tMapa* mapa) {

    int linha = ObtemNumeroLinhasMapa(mapa);

    for (int i = 0; i < linha; i++) {
        printf("%s\n", mapa->grid[i]);
    }

    printf("\n");
}

void AtualizaPacMapaFantDevolveComida (tPacman* pacman, tMapa* mapa, COMANDO comando) {
    if (ObtemItemMapa(mapa, ObtemPosicaoPacman(pacman)) == '*') {
        AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '>');
        AtualizaComidasPacman(pacman, comando);
    }
}



bool PacmanPegouComida (tPacman* pacman) {

    int nMovimentosig = ObtemNumeroMovimentosSignificativosPacman(pacman);

    if (nMovimentosig == 0) {
        return false;
    }

    char *acao = ObtemAcaoMovimento(pacman->historicoDeMovimentosSignificativos[nMovimentosig - 1]);
    if (!strcmp(acao, "pegou comida")) {
        return true;
    }
    
    return false;
}


void GeraArquivoInicializacao(tJogo* jogo) {

    FILE* file = fopen("inicializacao.txt", "w");
    if (file == NULL) {
        exit(1);
    }

    int linha = ObtemLinhaPosicao(ObtemPosicaoPacman(ObtemPacmanJogo(jogo)));
    int coluna = ObtemColunaPosicao(ObtemPosicaoPacman(ObtemPacmanJogo(jogo)));

    tMapa* mapa = ObtemMapaJogo(jogo);
    int nLinhasMapa = ObtemNumeroLinhasMapa(mapa);

    for (int i = 0; i < nLinhasMapa; i++) {
        fprintf(file, "%s\n", mapa->grid[i]);
    }

    fprintf(file, "Pac-Man comecara o jogo na linha %d e coluna %d\n", linha + 1, coluna + 1);
    fclose(file);
}



void DesalocaJogo (tJogo* jogo) {

    for (int i = 0; i < NUM_FANTASMAS; i++) {
        DesalocaFantasma(jogo->fantasmas[i]);
    }

    free(jogo->fantasmas);

    DesalocaPacman(ObtemPacmanJogo(jogo));
    DesalocaMapa(ObtemMapaJogo(jogo));
    free (jogo);
}