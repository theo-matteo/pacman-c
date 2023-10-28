#include "tJogo.h"
#define NUM_FANTASMAS 4

tJogo* CriaJogo (const char* caminhoConfig) {

    tJogo *jogo = (tJogo *) malloc (sizeof(tJogo));
    if (jogo == NULL) {
        return NULL;
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

    tFantasma **fantasmas = ObtemFantasmasJogo(jogo);
    tPacman *pacman = ObtemPacmanJogo(jogo);
    tMapa *mapa = ObtemMapaJogo(jogo);

    /* Realiza Redirecionamento da Saida para Arquivo */
    FILE* arquivoSaida = RedirecionamentoSaida();

    /* Cria Arquivo de Inicializacao Jogo */
    GeraArquivoInicializacao(jogo);

    /* Cria e Inicializa Trilha do Pacman */
    CriaTrilhaPacman(pacman, ObtemNumeroLinhasMapa(mapa), ObtemNumeroColunasMapa(mapa));

    while (!AcabouJogo(jogo)) {

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
            if (VerificaColisao(posAnteriorPacman, posAnteriorFantasma, fantasmas[i], pacman)) {
                MataPacmanJogo(mapa, pacman);
            }

            DesalocaPosicao(posAnteriorFantasma);
        }

        /* Desaloca Posicao Anterior do Pacman */
        DesalocaPosicao(posAnteriorPacman);

        if (ObtemItemMapa(mapa, ObtemPosicaoPacman(pacman)) == '*') {
            AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '>');
            AtualizaComidasPacman(pacman, comando);
        }

        if (PacmanPegouComida(pacman)) {
            AtualizaComidasAtuaisMapa(mapa);
        }
        if (!EstaVivoPacman(pacman)) {
            InsereNovoMovimentoSignificativoPacman(pacman, comando, "fim de jogo por encostar em um fantasma");
        }

        PreencheArquivoSaida(arquivoSaida, jogo, comando);
    }

    /* Geracao de Arquivos */
    GeraArquivoResumo(pacman);
    SalvaTrilhaPacman(pacman);
    PreencheDesalocaArquivoSaida(arquivoSaida, jogo);
    GeraArquivoEstatisticas(pacman);
    GeraArquivoRanking(pacman);

    fclose(arquivoSaida);
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

bool AcabouJogo (tJogo* jogo) {

    tPacman* pacman = ObtemPacmanJogo(jogo);
    tMapa* mapa = ObtemMapaJogo(jogo);

    if (ObtemNumeroAtualMovimentosPacman(pacman) == ObtemNumeroMaximoMovimentosMapa(mapa)) {
        return true;
    }
    else if (mapa->nFrutasAtual == 0) {
        return true;
    }
    else if (!EstaVivoPacman(pacman)) {
        return true;
    }

    return false;
}

COMANDO LeComandoTeclado () {

    char caractere;
    scanf("%c\n", &caractere);

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

tMapa* ObtemMapaJogo (tJogo* jogo) {
    return jogo->mapa;
}

tPacman* ObtemPacmanJogo (tJogo* jogo) {
    return jogo->pacman;
}

tFantasma** ObtemFantasmasJogo (tJogo* jogo) {
    return jogo->fantasmas;
}

void PrintaMapaJogo (tMapa* mapa) {

    int linha = ObtemNumeroLinhasMapa(mapa);

    for (int i = 0; i < linha; i++) {
        printf("%s\n", mapa->grid[i]);
    }

    printf("\n");
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

void AtualizaComidasAtuaisMapa (tMapa* mapa) {
    mapa->nFrutasAtual--;
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

FILE* RedirecionamentoSaida () {

    FILE *file = freopen("saida.txt", "w", stdout);
    if (file == NULL) {
        exit(1);
    }

    return file;
}

void PreencheArquivoSaida (FILE* file, tJogo* jogo, COMANDO comando) {

    tMapa* mapa = ObtemMapaJogo(jogo);
    int nLinhasMapa = ObtemNumeroLinhasMapa(mapa);

    fprintf(file, "Estado do jogo apos o movimento '%c':\n", ConverteComandoChar(comando));

    for (int i = 0; i < nLinhasMapa; i++) {
        fprintf(file, "%s\n", mapa->grid[i]);
    }

    fprintf(file, "Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(ObtemPacmanJogo(jogo)));

}

void PreencheDesalocaArquivoSaida (FILE* file, tJogo* jogo) {

    tMapa* mapa = ObtemMapaJogo(jogo);

    if (mapa->nFrutasAtual == 0) {
        fprintf(file, "Voce venceu!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(ObtemPacmanJogo(jogo)));
    }
    else {
        fprintf(file, "Game over!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(ObtemPacmanJogo(jogo)));
    }
    
    fclose (file);
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