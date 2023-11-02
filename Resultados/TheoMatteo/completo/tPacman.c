#include "tPacman.h"
#define PORTAL '@'

/* Atualiza o Numero de Colisoes de Acordo com o Comando */
void AtualizaNumeroColisoes (COMANDO comando, tPacman* pacman) {
    if (comando == MOV_ESQUERDA) {
        pacman->nColisoesParedeEsquerda++;
    }
    else if (comando == MOV_DIREITA) {
        pacman->nColisoesParedeDireita++;
    }
    else if (comando == MOV_CIMA) {
        pacman->nColisoesParedeCima++;
    }
    else {
        pacman->nColisoesParedeBaixo++;
    }
}

/* Cria uma posicao nova de acordo com o Comando */
tPosicao* CriaPosicaoMovimento (COMANDO comando, tPacman* pacman) {

    int linha = ObtemLinhaPosicao(ObtemPosicaoPacman(pacman));
    int coluna = ObtemColunaPosicao(ObtemPosicaoPacman(pacman));

    if (comando == MOV_ESQUERDA) {
       return CriaPosicao(linha, coluna - 1); 
    }
    else if (comando == MOV_DIREITA) {
        return CriaPosicao(linha, coluna + 1); 
    }
    else if (comando == MOV_CIMA) {
        return CriaPosicao(linha - 1, coluna); 
    }
    else if (comando == MOV_BAIXO) {
        return CriaPosicao(linha + 1, coluna); 
    }

    return NULL;
}

/* Atualiza o Numero de Movimentos de Acordo com o Comando */
void AtualizaNumeroMovimentos (COMANDO comando, tPacman* pacman) {
    if (comando == MOV_ESQUERDA) {
       pacman->nMovimentosEsquerda++; 
    }
    else if (comando == MOV_DIREITA) {
        pacman->nMovimentosDireita++; 
    }
    else if (comando == MOV_CIMA) {
        pacman->nMovimentosCima++;
    }
    else if (comando == MOV_BAIXO) {
        pacman->nMovimentosBaixo++;
    }
}

/* Atualiza o Numero de Comidas de Acordo com o Comando */
void AtualizaNumeroComidasObtidas (COMANDO comando, tPacman* pacman) {
     if (comando == MOV_ESQUERDA) {
       pacman->nFrutasComidasEsquerda++;
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

    /* Refere a posicao antes de realizar a movimentacao */
    tPosicao *posicaoAtualPacman = ClonaPosicao(ObtemPosicaoPacman(pacman));

    /* Refere a posicao apos o movimento*/
    tPosicao *posicaoNova = CriaPosicaoMovimento(comando, pacman);

    /* Flag para Verificar se o Movimento foi bem sucedido */
    bool movimentou = false;

    /* Atualiza o Numero de Movimentos de acordo com o Comando */
    AtualizaNumeroMovimentos(comando, pacman);
    
    if (EncontrouParedeMapa(mapa, posicaoNova)) {
        AtualizaNumeroColisoes(comando, pacman);
        InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
    }
    else {

        /* Atualiza a posicao do Pacman*/
        AtualizaPosicao(ObtemPosicaoPacman(pacman), posicaoNova);
        AtualizaItemMapa(mapa, posicaoAtualPacman, ' ');

        movimentou = true;

        /* Verifica se o Pacman Encontrou Comida */
        if (EncontrouComidaMapa(mapa, ObtemPosicaoPacman(pacman))) {
            AtualizaNumeroComidasObtidas(comando, pacman);
            InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
        }

    }

    /* Atualiza a trilha do Pacman com o numero do movimento atual */
    AtualizaTrilhaPacman(pacman);


    /* Verifica se o Pacman Entrou ou Saiu do Tunel (se tiver o Tunel)*/
    if (PossuiTunelMapa(mapa) && movimentou) {

        /* Verifica se o Pacman saiu do Tunel, Devolvendo Tunel ao Mapa */
        if (AcessouTunelMapa(mapa, posicaoAtualPacman)) {
            AtualizaItemMapa(mapa, posicaoAtualPacman, PORTAL); 
        }

        /* Verifica se Após Movimentar, Entrou em um Tunel */
        if (AcessouTunelMapa(mapa, posicaoNova)) {
            EntraTunelMapa(mapa, ObtemPosicaoPacman(pacman));
            AtualizaTrilhaPacman(pacman);
        }
        
    }

    DesalocaPosicao(posicaoNova);
    DesalocaPosicao(posicaoAtualPacman);
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

    /* Incrementa o numero de Movimentos Significativos */
    pacman->nMovimentosSignificativos++;

    /* Obtem o Numero de Movimentos Significativos atual */
    int qtdMovimentosSig = ObtemNumeroMovimentosSignificativosPacman(pacman);

    /* Obtem o Numero do Movimento Atual */
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