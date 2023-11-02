#include "tJogo.h"
#define NUM_FANTASMAS 4
#define PACMAN '>'
#define COMIDA '*'
#define PORTAL '@'

tJogo* CriaJogo (const char* caminhoConfig) {

    tJogo *jogo = (tJogo *) malloc (sizeof(tJogo));
    if (jogo == NULL) {
        return NULL;
    }   

    /* Inicializa Numero Comidas Obtidas */
    jogo->numeroComidasObtidas = 0;

    /* Cria Mapa do Jogo */
    jogo->mapa = CriaMapa(caminhoConfig);

    /* Cria o Pacman */
    jogo->pacman = CriaPacman(ObtemPosicaoItemMapa(ObtemMapaJogo(jogo), PACMAN));

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

    tFantasma **fantasmas = ObtemFantasmasJogo(jogo);
    tPacman *pacman = ObtemPacmanJogo(jogo);
    tMapa *mapa = ObtemMapaJogo(jogo);

    GeraArquivoInicializacao(jogo);

    /* Cria e Inicializa Trilha do Pacman */
    CriaTrilhaPacman(pacman, ObtemNumeroLinhasMapa(mapa), ObtemNumeroColunasMapa(mapa));
    AtualizaTrilhaPacman(pacman);

    FILE* arquivoSaida = fopen("saida.txt", "w");

    while (!AcabouJogo(jogo)) {

        /* Obtem Comando do Jogador */
        COMANDO comando = LeComandoTeclado();

        /* Movimentacao Fantasmas */
        MovimentaFantasmas(fantasmas, mapa, comando, pacman);

        if (EstaVivoPacman(pacman)) {

            MovimentaPacmanMapa(pacman, mapa, comando);
            VerificaColisaoComFantasmas(fantasmas, pacman, mapa, comando);

            /* Atualiza o Numero de Comidas */
            if (PacmanPegouComida(pacman)) {
                AtualizaComidasObtidas(jogo);
            }

        }

        /* Caso o Fantasma tenha Ocupado Comida, Devolve sua Skin ao Mapa */
        AtualizaSkinFantasma(fantasmas, mapa);

        /* Atualiza o Arquivo de Saida */
        PreencheArquivoSaida(arquivoSaida, jogo, comando);
    }



    /* Salva Arquivos */
    GeraArquivoResumo(pacman);
    SalvaTrilhaPacman(pacman);
    FinalizaArquivoSaida(arquivoSaida, jogo);
    GeraArquivoEstatisticas(pacman);
    GeraArquivoRanking(pacman);
    GeraArquivoRanking(pacman);

    fclose(arquivoSaida);
}

void VerificaColisaoComFantasmas (tFantasma** fantasmas, tPacman* pacman, tMapa* mapa, COMANDO comando) {
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        if (fantasmas[i] != NULL) {
            if (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoFantasma(fantasmas[i]))) {
                AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasmas[i]), ObtemCaractereSkin(ObtemSkinFantasma(fantasmas[i])));
                MataPacmanJogo(mapa, pacman, comando);
                return;
            }
        }
    }
}

void MovimentaFantasmas (tFantasma** fantasmas, tMapa* mapa, COMANDO comando, tPacman* pacman) {


    for (int i = 0; i < NUM_FANTASMAS; i++) {

        if (fantasmas[i] != NULL) {

            tPosicao* posAt = ClonaPosicao(ObtemPosicaoFantasma(fantasmas[i]));

            /* Realiza Movimentacao do Fantasma*/
            MoveFantasma(fantasmas[i], mapa);
            char item = ObtemItemMapa(mapa, posAt);

            /* Verifica se Houve Colisao com Pacman */
            if (SaoIguaisPosicao(ObtemPosicaoPacman(pacman), ObtemPosicaoFantasma(fantasmas[i]))) {

                if (PosicoesDivergiram(comando, ObtemSentidoFantasma(fantasmas[i]))) {

                    if (item == COMIDA) {
                        AtualizaItemMapa(mapa, posAt, ' ');
                    }

                    MovimentaPacmanMapa(pacman, mapa, comando);
                    MataPacmanJogo(mapa, pacman, comando);

                    if (item == COMIDA) {
                        AtualizaItemMapa(mapa, posAt, COMIDA);
                    }
                    else {
                        AtualizaItemMapa(mapa, posAt, ' ');
                    }

                    AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasmas[i]), ObtemCaractereSkin(ObtemSkinFantasma(fantasmas[i])));
            
                }
            }

            /* Atualiza o Fantasma para Comida Temporariamente () */
            if (fantasmaOcupandoComida(fantasmas[i])) {
                AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasmas[i]), COMIDA);
            }

            DesalocaPosicao(posAt);
        }
    }
}

void MovimentaPacmanMapa (tPacman* pacman, tMapa* mapa, COMANDO comando) {

    tPosicao *posAtualPac = ClonaPosicao(ObtemPosicaoPacman(pacman));
    char item = ObtemItemMapa(mapa, posAtualPac);

    MovePacman(pacman, mapa, comando);

    if (!SaoIguaisPosicao(posAtualPac, ObtemPosicaoPacman(pacman))) {
        if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, posAtualPac)) {
            AtualizaItemMapa(mapa, posAtualPac, PORTAL); 
        }
        else {
            if (item != PACMAN) {
                AtualizaItemMapa(mapa, posAtualPac, item);
            }
            else {
                AtualizaItemMapa(mapa, posAtualPac, ' ');
            }
        }
    }

    AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), PACMAN);
    DesalocaPosicao(posAtualPac);
}

COMANDO LeComandoTeclado() {

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
    else {
        return MOV_DIREITA;
    }

}

bool AcabouJogo (tJogo* jogo) {

    tPacman* pacman = ObtemPacmanJogo(jogo);
    tMapa* mapa = ObtemMapaJogo(jogo);

    if (ObtemNumeroAtualMovimentosPacman(pacman) == ObtemNumeroMaximoMovimentosMapa(mapa)) {
        return true;
    }
    else if (ObtemComidasObtidasJogador(jogo) == ObtemQuantidadeFrutasIniciaisMapa(mapa)) {
        return true;
    }
    else if (!EstaVivoPacman(pacman)) {
        return true;
    }

    return false;
}

void AtualizaComidasObtidas (tJogo* jogo) {
    jogo->numeroComidasObtidas++;
}

bool PacmanPegouComida (tPacman* pacman) {

    int nMovimentosig = ObtemNumeroMovimentosSignificativosPacman(pacman);
    int NumeroMovimentoAtual = ObtemNumeroAtualMovimentosPacman(pacman);

    if (nMovimentosig != 0) {

        char *acao = ObtemAcaoMovimento(pacman->historicoDeMovimentosSignificativos[nMovimentosig - 1]);
        int numeroMovimento = ObtemNumeroMovimento(pacman->historicoDeMovimentosSignificativos[nMovimentosig - 1]);
        
        if (!strcmp(acao, "pegou comida")) {
            if (NumeroMovimentoAtual == numeroMovimento) {
                return true;
            }
        }
    }

    return false;

}

bool fantasmaOcupandoComida (tFantasma* fantasma) {
    return fantasma->ocupouComida;
}
  
void AtualizaSkinFantasma (tFantasma** fantasmas, tMapa* mapa) {
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        if (fantasmas[i] != NULL) {
            if (ObtemItemMapa(mapa, ObtemPosicaoFantasma(fantasmas[i])) == COMIDA) {
                AtualizaItemMapa(mapa, ObtemPosicaoFantasma(fantasmas[i]), ObtemCaractereSkin(ObtemSkinFantasma(fantasmas[i])));
            }
        }
    }
}

bool PosicoesDivergiram (COMANDO comando, sentidoMovimento sentido) {

    if (comando == MOV_ESQUERDA && sentido == DIREITA) {
        return true;
    }
    else if (comando == MOV_DIREITA && sentido == ESQUERDA) {
        return true;
    }
    else if (comando == MOV_BAIXO && sentido == CIMA) {
        return true;
    }
    else if (comando == MOV_CIMA && sentido == BAIXO) {
        return true;
    }

    return false;  
}

void MataPacmanJogo (tMapa* mapa, tPacman* pacman, COMANDO comando) {
    InsereNovoMovimentoSignificativoPacman(pacman, comando, "fim de jogo por encostar em um fantasma");
    MataPacman(pacman);
}

tMapa* ObtemMapaJogo (tJogo* jogo) {
    return jogo->mapa;
}

tPacman* ObtemPacmanJogo (tJogo* jogo) {
    return jogo->pacman;
}

int ObtemComidasObtidasJogador (tJogo* jogo) {
    return jogo->numeroComidasObtidas;
}

tFantasma** ObtemFantasmasJogo (tJogo* jogo) {
    return jogo->fantasmas;
}

void PreencheArquivoSaida (FILE* file, tJogo* jogo, COMANDO comando) {

    tMapa* mapa = ObtemMapaJogo(jogo);

    fprintf(file, "Estado do jogo apos o movimento '%c':\n", ConverteComandoChar(comando));

    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            fprintf(file, "%c", mapa->grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fprintf(file, "Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(ObtemPacmanJogo(jogo)));

}

char ConverteComandoChar (COMANDO comando) {
    if (comando == MOV_CIMA) {
        return 'w';
    }
    else if (comando == MOV_BAIXO) {
        return 's';
    }
    else if (comando == MOV_ESQUERDA) {
        return 'a';
    }
    else {
        return 'd';
    }
}

void GeraArquivoInicializacao(tJogo* jogo) {

    FILE* file = fopen("inicializacao.txt", "w");
    if (file == NULL) {
        exit(1);
    }

    int linha = ObtemLinhaPosicao(ObtemPosicaoPacman(ObtemPacmanJogo(jogo))) + 1;
    int coluna = ObtemColunaPosicao(ObtemPosicaoPacman(ObtemPacmanJogo(jogo))) + 1;

    tMapa* mapa = ObtemMapaJogo(jogo);

    for (int i = 0; i < mapa->nLinhas; i++) {
        for (int j = 0; j < mapa->nColunas; j++) {
            fprintf(file, "%c", mapa->grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fprintf(file, "Pac-Man comecara o jogo na linha %d e coluna %d\n", linha, coluna);
    
    fclose(file);
}

void GeraArquivoResumo (tPacman* pacman) {

    int qtdMov = ObtemNumeroMovimentosSignificativosPacman(pacman);

    FILE *file = fopen("resumo.txt", "w");
    if (file == NULL) {
        exit(1);
    }

    for (int i = 0; i < qtdMov; i++) {

        int numeroMovimento = ObtemNumeroMovimento(pacman->historicoDeMovimentosSignificativos[i]);
        COMANDO comando = ObtemComandoMovimento(pacman->historicoDeMovimentosSignificativos[i]);
        char *acao = ObtemAcaoMovimento(pacman->historicoDeMovimentosSignificativos[i]);

        fprintf(file, "Movimento %d (%c) %s\n", numeroMovimento, ConverteComandoChar(comando), acao);

    }   

    fclose(file);
}

void FinalizaArquivoSaida(FILE* file, tJogo* jogo) {

    tMapa* mapa = ObtemMapaJogo(jogo);

    if (jogo->numeroComidasObtidas == ObtemQuantidadeFrutasIniciaisMapa(mapa)) {
        fprintf(file, "Voce venceu!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(ObtemPacmanJogo(jogo)));
    }
    else {
        fprintf(file, "Game over!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(ObtemPacmanJogo(jogo)));
    }
    
}

void GeraArquivoEstatisticas(tPacman* pacman) {

    FILE *file = fopen("estatisticas.txt", "w");
    if (file == NULL) {
        exit(1);
    }

    fprintf(file, "Numero de movimentos: %d\n", ObtemNumeroAtualMovimentosPacman(pacman));
    fprintf(file, "Numero de movimentos sem pontuar: %d\n", ObtemNumeroMovimentosSemPontuarPacman(pacman));
    fprintf(file, "Numero de colisoes com parede: %d\n", ObtemNumeroColisoesParedePacman(pacman));
    fprintf(file, "Numero de movimentos para baixo: %d\n", ObtemNumeroMovimentosBaixoPacman(pacman));
    fprintf(file, "Numero de movimentos para cima: %d\n", ObtemNumeroMovimentosCimaPacman(pacman));
    fprintf(file, "Numero de movimentos para esquerda: %d\n", ObtemNumeroMovimentosEsquerdaPacman(pacman));
    fprintf(file, "Numero de movimentos para direita: %d\n", ObtemNumeroMovimentosDireitaPacman(pacman));

    fclose(file);
}

void GeraArquivoRanking (tPacman* pacman) {

    FILE* file = fopen("ranking.txt", "w");
    if (file == NULL) {
        exit(1);
    }

    int qtdMovimentosEsquerda = ObtemNumeroMovimentosEsquerdaPacman(pacman);
    int qtdMovimentosDireita = ObtemNumeroMovimentosDireitaPacman(pacman);
    int qtdMovimentosCima = ObtemNumeroMovimentosCimaPacman(pacman);
    int qtdMovimentosBaixo = ObtemNumeroMovimentosBaixoPacman(pacman);

    int qtdComidasEsquerda = ObtemNumeroFrutasComidasEsquerdaPacman(pacman);
    int qtdComidasDireita = ObtemNumeroFrutasComidasDireitaPacman(pacman);
    int qtdComidasCima = ObtemNumeroFrutasComidasCimaPacman(pacman);
    int qtdComidasBaixo = ObtemNumeroFrutasComidasBaixoPacman(pacman);

    int qtdColisoesEsquerda = ObtemNumeroColisoesParedeEsquerdaPacman(pacman);
    int qtdColisoesDireita = ObtemNumeroColisoesParedeDireitaPacman(pacman);
    int qtdColisoesCima = ObtemNumeroColisoesParedeCimaPacman(pacman);
    int qtdColisoesBaixo = ObtemNumeroColisoesParedeBaixoPacman(pacman);

    int qtdMovimentos[] = {qtdMovimentosEsquerda, qtdMovimentosDireita, qtdMovimentosCima, qtdMovimentosBaixo};
    int qtdComidasTotal[] = {qtdComidasEsquerda, qtdComidasDireita, qtdComidasCima, qtdComidasBaixo};
    int qtdColisoesTotal[] = {qtdColisoesEsquerda, qtdColisoesDireita, qtdColisoesCima, qtdColisoesBaixo};
    char ranking[] = {'a', 'd', 'w', 's'};


    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {

            if (qtdComidasTotal[i] > qtdComidasTotal[j]) {
                continue;
            }
            else if (qtdComidasTotal[i] == qtdComidasTotal[j]) {
                if (qtdColisoesTotal[i] < qtdColisoesTotal[j]) {
                    continue;
                }
                else if (qtdColisoesTotal[i] == qtdColisoesTotal[j]) {
                    if (qtdMovimentos[i] > qtdMovimentos[j]) {
                        continue;
                    }
                    else if (qtdMovimentos[i] == qtdMovimentos[j]) {
                        if (ranking[i] < ranking[j]) {
                            continue;
                        }
                    }
                }
            }

            char aux = ranking[i];
            ranking[i] = ranking[j];
            ranking[j] = aux;

            int auxComidas = qtdComidasTotal[i];
            qtdComidasTotal[i] = qtdComidasTotal[j];
            qtdComidasTotal[j] = auxComidas;

            int auxColisoes = qtdColisoesTotal[i];
            qtdColisoesTotal[i] = qtdColisoesTotal[j];
            qtdColisoesTotal[j] = auxColisoes;

            int auxMovimento = qtdMovimentos[i];
            qtdMovimentos[i] = qtdMovimentos[j];
            qtdMovimentos[j] = auxMovimento;

        }
    }

    for (int i = 0; i < 4; i++)  {
        if (ranking[i] == 'a') {
            fprintf(file, "%c,%d,%d,%d\n", ranking[i], qtdComidasEsquerda, qtdColisoesEsquerda, qtdMovimentosEsquerda);
        }
        else if (ranking[i] == 's') {
            fprintf(file, "%c,%d,%d,%d\n", ranking[i], qtdComidasBaixo, qtdColisoesBaixo, qtdMovimentosBaixo);
        }
        else if (ranking[i] == 'd') {
            fprintf(file, "%c,%d,%d,%d\n", ranking[i], qtdComidasDireita, qtdColisoesDireita, qtdMovimentosDireita);
        }
        else {
            fprintf(file, "%c,%d,%d,%d\n", ranking[i], qtdComidasCima, qtdColisoesCima, qtdMovimentosCima);
        }
    }

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