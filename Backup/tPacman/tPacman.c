#include "tPacman.h"

tPacman* CriaPacman(tPosicao* posicao) {

    if (posicao == NULL) {
        return NULL;
    }

    tPacman *pacman = (tPacman *) malloc(sizeof(tPacman));
    if (pacman == NULL) {
        exit(1);
    }

    pacman->posicaoAtual = posicao;
    pacman->estaVivo = 1;

    /* Inicializa Variaveis de Colisao*/
    pacman->nColisoesParedeBaixo = 0;
    pacman->nColisoesParedeCima = 0;
    pacman->nColisoesParedeDireita = 0;
    pacman->nColisoesParedeEsquerda = 0;

    /* Inicializa Variaveis de Movimento */
    pacman->nMovimentosBaixo = 0;
    pacman->nMovimentosCima = 0;
    pacman->nMovimentosDireita = 0;
    pacman->nMovimentosEsquerda = 0;

    /* Inicializa Variaveis de Comida */
    pacman->nFrutasComidasBaixo = 0;
    pacman->nFrutasComidasCima = 0;
    pacman->nFrutasComidasDireita = 0;
    pacman->nFrutasComidasEsquerda = 0;

    /* Inicializa Variaveis de Historico */
    pacman->nMovimentosSignificativos = 0;
    pacman->historicoDeMovimentosSignificativos = NULL;

    /* Inicializa Variaveis da Trilha */
    pacman->nLinhasTrilha = 0;
    pacman->nColunasTrilha = 0;
    pacman->trilha = NULL;

    return pacman;
}

tPacman* ClonaPacman(tPacman* pacman) {

    tPosicao *posicaoClonada = ClonaPosicao(ObtemPosicaoPacman(pacman));
    tPacman *pacmanClonado = CriaPacman(posicaoClonada);
    
    return pacmanClonado;
}

tMovimento** ClonaHistoricoDeMovimentosSignificativosPacman(tPacman* pacman) {
    
    tMovimento **historicoClonado = (tMovimento **) malloc(sizeof(tMovimento *) * pacman->nMovimentosSignificativos);
    if (historicoClonado == NULL) {
        exit(1);
    }

    for (int i = 0; i < pacman->nMovimentosSignificativos; i++) {

        /* Obtem atributos do movimento original */
        int numeroMovimento = ObtemNumeroMovimento(pacman->historicoDeMovimentosSignificativos[i]);
        COMANDO comando = ObtemComandoMovimento(pacman->historicoDeMovimentosSignificativos[i]);
        char *acao = ObtemAcaoMovimento(pacman->historicoDeMovimentosSignificativos[i]);

        /* Passa atributos para o historico clonado*/
        historicoClonado[i] = CriaMovimento(numeroMovimento, comando, acao);
    }

    return historicoClonado;
}

tPosicao* ObtemPosicaoPacman(tPacman* pacman) {
    return pacman->posicaoAtual;
}

int EstaVivoPacman(tPacman* pacman) {
    return pacman->estaVivo;
}

void MovePacman(tPacman* pacman, tMapa* mapa, COMANDO comando) {

    int linha = ObtemLinhaPosicao(ObtemPosicaoPacman(pacman));
    int coluna = ObtemColunaPosicao(ObtemPosicaoPacman(pacman)); 

    tPosicao *posicaoNova = NULL;

    if (comando == MOV_ESQUERDA) {

        posicaoNova = CriaPosicao(linha, coluna - 1);
        pacman->nMovimentosEsquerda++;

        if (EncontrouParedeMapa(mapa, posicaoNova)) {
            pacman->nColisoesParedeEsquerda++;
            InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
        }

        else {

            AtualizaPosicao(ObtemPosicaoPacman(pacman), posicaoNova);

            if (EncontrouComidaMapa(mapa, ObtemPosicaoPacman(pacman))) {
                pacman->nFrutasComidasEsquerda++;
                InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
            }
        }
    }
    else if (comando == MOV_DIREITA) {

        posicaoNova = CriaPosicao(linha, coluna + 1);
        pacman->nMovimentosDireita++;

        if (EncontrouParedeMapa(mapa, posicaoNova)) {
            pacman->nColisoesParedeDireita++;
            InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
        }
        else {
            
            AtualizaPosicao(ObtemPosicaoPacman(pacman), posicaoNova);

            if (EncontrouComidaMapa(mapa, ObtemPosicaoPacman(pacman))) {
                pacman->nFrutasComidasDireita++;
                InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
            }
        }
    }
    
    else if (comando == MOV_CIMA) {

        posicaoNova = CriaPosicao(linha - 1, coluna);
        pacman->nMovimentosCima++;

        if (EncontrouParedeMapa(mapa, posicaoNova)) {
            pacman->nColisoesParedeCima++;
            InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
        }
        else {
            AtualizaPosicao(ObtemPosicaoPacman(pacman), posicaoNova);

            if (EncontrouComidaMapa(mapa, ObtemPosicaoPacman(pacman))) {
                pacman->nFrutasComidasCima++;
                InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
            }
        }
    }

    else {

        posicaoNova = CriaPosicao(linha + 1, coluna);
        pacman->nMovimentosBaixo++;

        if (EncontrouParedeMapa(mapa, posicaoNova)) {
            pacman->nColisoesParedeBaixo++;
            InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
        }
        else {

            AtualizaPosicao(ObtemPosicaoPacman(pacman), posicaoNova);

            if (EncontrouComidaMapa(mapa, ObtemPosicaoPacman(pacman))) {
                pacman->nFrutasComidasBaixo++;
                InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
            }
        }
    }

    AtualizaTrilhaPacman(pacman);

    if (PossuiTunelMapa(mapa)) {

        /* Obtem a Posicao Atual do Pacman, antes de 'Movimentar' */
        tPosicao *posicaoAtualPacman = CriaPosicao(linha, coluna);

        /* Verifica se o Pacman saiu da uma Posicao de Tunel */
        if (AcessouTunelMapa(mapa, posicaoAtualPacman)) {
            if (SaoIguaisPosicao(posicaoAtualPacman, ObtemPosicaoPacman(pacman)) == false) {
                AtualizaItemMapa(mapa, posicaoAtualPacman, '@'); 
            } 
        }

        DesalocaPosicao(posicaoAtualPacman);

        /* Verifica se Após Movimentar, Entrou em um Tunel */
        if (AcessouTunelMapa(mapa, posicaoNova)) {
            EntraTunelMapa(mapa, ObtemPosicaoPacman(pacman));
            AtualizaTrilhaPacman(pacman);
        }
    }

    AtualizaItemMapa(mapa, ObtemPosicaoPacman(pacman), '>'); 
    DesalocaPosicao(posicaoNova);
}

void CriaTrilhaPacman(tPacman* pacman, int nLinhas, int nColunas) {

    pacman->nLinhasTrilha = nLinhas;
    pacman->nColunasTrilha = nColunas;

    if (pacman->trilha != NULL) {
        return;
    }

    pacman->trilha = (int **) malloc(sizeof(int *) * nLinhas);
    if (pacman->trilha == NULL) {
        exit(1);
    }

    for (int i = 0; i < nLinhas; i++) {
        pacman->trilha[i] = (int *) malloc(sizeof(int) * nColunas);
        if (pacman->trilha[i] == NULL) {
            exit(1);
        }
    }

    /* Inicializa Valores da Trilha*/
    for (int i = 0; i < nLinhas; i++) {
        for (int j = 0; j < nColunas; j++) {
            pacman->trilha[i][j] = -1;
        }
    }

}

void AtualizaTrilhaPacman(tPacman* pacman) {
    
    int linha = ObtemLinhaPosicao(ObtemPosicaoPacman(pacman));
    int coluna = ObtemColunaPosicao(ObtemPosicaoPacman(pacman));

    pacman->trilha[linha][coluna] = ObtemNumeroAtualMovimentosPacman(pacman);
}

void SalvaTrilhaPacman(tPacman* pacman) {

    FILE *file = fopen("trilha.txt", "w");

    if (file == NULL) {
        exit(1);
    }   

    for (int i = 0; i < pacman->nLinhasTrilha; i++) {
        
        for (int j = 0; j < pacman->nColunasTrilha; j++) {

            if (pacman->trilha[i][j] == -1) {
                fprintf(file, "#");
            }
            else {
                fprintf(file, "%d", pacman->trilha[i][j]);
            }

            if (j != pacman->nColunasTrilha - 1) { 
                fprintf(file, " ");
            }

        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void InsereNovoMovimentoSignificativoPacman(tPacman* pacman, COMANDO comando, const char* acao) {

    // Incrementa o numero de Movimentos Significativos
    pacman->nMovimentosSignificativos++;

    int qtdMovimentosSig = ObtemNumeroMovimentosSignificativosPacman(pacman);
    int nMovimento = ObtemNumeroAtualMovimentosPacman(pacman);

    tMovimento *movimento = CriaMovimento(nMovimento, comando, acao);

    if (pacman->historicoDeMovimentosSignificativos == NULL) {
        pacman->historicoDeMovimentosSignificativos = (tMovimento **) malloc(sizeof(tMovimento *));
        if (pacman->historicoDeMovimentosSignificativos == NULL) {
            exit(1);
        }
    }
    else {
        pacman->historicoDeMovimentosSignificativos = realloc(pacman->historicoDeMovimentosSignificativos, sizeof(tMovimento *) * qtdMovimentosSig);
    }
    
    pacman->historicoDeMovimentosSignificativos[qtdMovimentosSig - 1] = movimento;
}

void MataPacman(tPacman* pacman) {
    pacman->estaVivo = 0;
}

void DesalocaPacman(tPacman* pacman) {

    if (pacman == NULL) {
        return;
    }

    DesalocaPosicao(pacman->posicaoAtual);

    for (int i = 0; i < pacman->nMovimentosSignificativos; i++) {
        DesalocaMovimento(pacman->historicoDeMovimentosSignificativos[i]);
    }

    free(pacman->historicoDeMovimentosSignificativos);


    for (int i = 0; i < pacman->nLinhasTrilha; i++) {
        free(pacman->trilha[i]);
    }

    free(pacman->trilha);
    free(pacman);
}

int ObtemNumeroAtualMovimentosPacman(tPacman* pacman) {

    int qtdMovimentos = ObtemNumeroMovimentosBaixoPacman(pacman);
    qtdMovimentos += ObtemNumeroMovimentosCimaPacman(pacman);
    qtdMovimentos += ObtemNumeroMovimentosEsquerdaPacman(pacman);
    qtdMovimentos += ObtemNumeroMovimentosDireitaPacman(pacman);

    return qtdMovimentos;
}

int ObtemNumeroMovimentosSemPontuarPacman(tPacman* pacman) {
    return ObtemNumeroAtualMovimentosPacman(pacman) - ObtemPontuacaoAtualPacman(pacman);
}

int ObtemNumeroColisoesParedePacman(tPacman* pacman) {

    int total = ObtemNumeroColisoesParedeBaixoPacman(pacman);
    total += ObtemNumeroColisoesParedeCimaPacman(pacman);
    total += ObtemNumeroColisoesParedeDireitaPacman(pacman);
    total += ObtemNumeroColisoesParedeEsquerdaPacman(pacman);

    return total;
}

int ObtemNumeroMovimentosBaixoPacman(tPacman* pacman) {
    return pacman->nMovimentosBaixo;
}

int ObtemNumeroFrutasComidasBaixoPacman(tPacman* pacman) {
    return pacman->nFrutasComidasBaixo;
}

int ObtemNumeroColisoesParedeBaixoPacman(tPacman* pacman) {
    return pacman->nColisoesParedeBaixo;
}

int ObtemNumeroMovimentosCimaPacman(tPacman* pacman) {
    return pacman->nMovimentosCima;
}

int ObtemNumeroFrutasComidasCimaPacman(tPacman* pacman) {
    return pacman->nFrutasComidasCima;
}

int ObtemNumeroColisoesParedeCimaPacman(tPacman* pacman) {
    return pacman->nColisoesParedeCima;
}

int ObtemNumeroMovimentosEsquerdaPacman(tPacman* pacman) {
    return pacman->nMovimentosEsquerda;
}

int ObtemNumeroFrutasComidasEsquerdaPacman(tPacman* pacman) {
    return pacman->nFrutasComidasEsquerda;
}

int ObtemNumeroColisoesParedeEsquerdaPacman(tPacman* pacman) {
    return pacman->nColisoesParedeEsquerda;
}

int ObtemNumeroMovimentosDireitaPacman(tPacman* pacman) {
    return pacman->nMovimentosDireita;
}

int ObtemNumeroFrutasComidasDireitaPacman(tPacman* pacman) {
    return pacman->nFrutasComidasDireita;
}

int ObtemNumeroColisoesParedeDireitaPacman(tPacman* pacman) {
    return pacman->nColisoesParedeDireita;
}

int ObtemNumeroMovimentosSignificativosPacman(tPacman* pacman) {
    return pacman->nMovimentosSignificativos;
}

int ObtemPontuacaoAtualPacman(tPacman* pacman) {

    int pontuacao = ObtemNumeroFrutasComidasDireitaPacman(pacman);
    pontuacao += ObtemNumeroFrutasComidasEsquerdaPacman(pacman);
    pontuacao += ObtemNumeroFrutasComidasBaixoPacman(pacman);
    pontuacao += ObtemNumeroFrutasComidasCimaPacman(pacman);

    return pontuacao;
}