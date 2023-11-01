#include <stdio.h>

typedef struct {
  int linha;
  int coluna;
} tPosicao; 

typedef struct {
  int vida; // 1 para vivo e 0 para morto
  int pontuacao; // Pontuacao do Jogador
  int flagColisao; // Verifica se o Pacman colidiu na parede
  int flagComida; // Verifica se o Pacman pegou comida
  int flagPortal; // Verificas se o Pacman entrou em um Portal
  int qtdMovimentos; // Contador de Movimentos do Pacman
  int qtdColisoes; // Contador de Colisoes do PacMan
  tPosicao tunel[2]; // Armazena Coordernadas do Tunel
  tPosicao posicao; // Armazena Coordernadas do Pacman
} tPac;

typedef struct {
  char id; // Identificador do Fantasma
  int sentido; // 1 para Sentido Normal e 2 para Sentido Oposto
  int comida; // Variavel acionada quando o fantasma ocupa a posicao de uma comida
  int estaNoMapa; // Verifica se o Fantasma esta no Mapa
  tPosicao posicao; // Armazena Coordernadas do Fantasma
  int achouPacman; // Verifica se o fantasma topou com o PacMan
  int flagMovimento; // Verifica se o fantasma movimentou
} tFantasma;

typedef struct {
  char mov; // Movimento realizado pelo Jogador
  int qtd_comida; // Quantidade de Comida de cada Movimento que o PacMan pegou
  int qtdColisoes; // Quantidade de Colisoes  de cada Movimentos
  int qtdMov; // Quantidade realizada por cada movimento
} tRanking;

void criaMapa(FILE * file, int linha, int coluna, char mapa[linha][coluna]);
void inicializaMapa(char diretorio[], tPac pacman, int l, int c, char mapa[l][c]);
tPac inicializaPacman(int l, int c, char mapa[l][c]);
void inicializaFantasmas(tFantasma fantasma[], int l, int c, char mapa[l][c]);
void inicializaRanking(tRanking ranking[]);
tPac ReiniciaFlagsPacman(tPac pacman);
int eh_fantasma(char c);
tPac movimentaPacman(char mov, tPac pacman, int l, int c, char mapa[l][c]);
tPac movPacPortal(tPac pacman, int a, int b, int l, int c, char mapa[l][c]);
void movePacman(tPac pacman, int a, int b, int l, int c, char mapa[l][c]);
void fazMovimentacoes(tFantasma fantasma[], int l, int c, char mapa[l][c]);
void moveFantasma(tFantasma fantasma[], int k, int a, int b, int l, int c, char mapa[l][c]);
int verificaVidaPacman(tPac pacman, int l, int c, char mapa[l][c]);
void mudaSentidoFantasma(tFantasma fantasma[], int k);
int identificaMovFantasma(int k, tFantasma fantasma[]);
int verificaMovimentoPacman(tFantasma fantasma[], int k, char mov);
void ocupouComida(int k, tFantasma fantasma[], int l, int c, char mapa[l][c]);
tPac ocupouPortal(tPac pacman, int l, int c, char mapa[l][c]);
void arquivoJogo(FILE * file, tPac pacman, int qtd_comida, char mov, int lim_mov, int l, int c, char mapa[l][c]);
int qtdComidas(int l, int c, char mapa[l][c]);
void preencheRanking(char movimento, tPac pacman, tRanking rank[]);
void arquivoRanking(char diretorio[], tRanking rank[]);
void arquivoResumo(FILE * file, tPac pacman, char mov);
void arquivoEstatisticas(char diretorio[], tRanking rank[], tPac pacman);
void inicializaTrilha(tPac pacman, int l, int c, int trilha[l][c]);
void preencheTrilha(tPac pacman, int l, int c, int trilha[l][c]);
void arquivoTrilha(char diretorio[], int l, int c, int trilha[l][c]);
int fimJogo(tPac pacman, int qtd_comida, int lim_mov);
int todosMovimentaram(tFantasma fantasma[]);
void ReiniciaFlagsFantasma(tFantasma[]);

int main(int argc, char * argv[]) {

  FILE * file;
  char dir_mapa[1000], dir_jogadas[1000], dir_saida[1000], dir_resumo[1000]; // declaracao dos diretorios
  char dir_inicializacao[1000], dir_estatisticas[1000], dir_ranking[1000], dir_trilha[1000];
  int linha, coluna, limite_movimentos; 

  if (argc <= 1) { 
    printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
    return 1;
  }

  sprintf(dir_mapa, "%s/mapa.txt", argv[1]); // inicializacao dos diretorios
  sprintf(dir_jogadas, "%s/jogadas.txt", argv[1]);
  sprintf(dir_saida, "%s/saida/saida.txt", argv[1]);
  sprintf(dir_resumo, "%s/saida/resumo.txt", argv[1]);
  sprintf(dir_inicializacao, "%s/saida/inicializacao.txt", argv[1]);
  sprintf(dir_estatisticas, "%s/saida/estatisticas.txt", argv[1]);
  sprintf(dir_ranking, "%s/saida/ranking.txt", argv[1]);
  sprintf(dir_trilha, "%s/saida/trilha.txt", argv[1]);

  file = fopen(dir_mapa, "r"); // Abre o Arquivo do Mapa

  if (!file) { // Se caso nao consiga ler o arquivo
    printf("Nao foi possivel ler os arquivos no diretorio %s", dir_mapa);
    return 1;
  }

  fscanf(file, "%d %d %d\n", & linha, & coluna, & limite_movimentos); // Leitura da Primeira Linha
  coluna++; // Inclui o \0

  char mapa[linha][coluna], movimento; 
  tFantasma fantasma[4];
  tPac pacman;
  tRanking rank[4];
  int trilha[linha][coluna], qtd_comida;

  criaMapa(file, linha, coluna, mapa); 
  fclose(file); // Fecha o Arquivo

  pacman = inicializaPacman(linha, coluna, mapa); // Inicializacoes
  inicializaFantasmas(fantasma, linha, coluna, mapa);
  inicializaMapa(dir_inicializacao, pacman, linha, coluna, mapa);
  inicializaRanking(rank);
  inicializaTrilha(pacman, linha, coluna, trilha);
  qtd_comida = qtdComidas(linha, coluna, mapa); // Verifica quantidade de comida do mapa

  FILE * arqv_entrada = freopen(dir_jogadas, "r", stdin); // Redirecionamento Arquivo de Entrada
  FILE * arq_saida = freopen(dir_saida, "w", stdout);
  FILE * arq_resumo = fopen(dir_resumo, "w");

  while (scanf("%c\n", & movimento) == 1) {

    fazMovimentacoes(fantasma, linha, coluna, mapa); // movimenta os fantasmas
    pacman = movimentaPacman(movimento, pacman, linha, coluna, mapa); // movimenta o pacman

    if (!todosMovimentaram(fantasma)) { // verifica se todos os fantasmas movimentaram
      fazMovimentacoes(fantasma, linha, coluna, mapa);
      pacman.vida = verificaVidaPacman(pacman, linha, coluna, mapa); // verifica se o pacman esta vivo
    }

    arquivoJogo(arq_saida, pacman, qtd_comida, movimento, limite_movimentos, linha, coluna, mapa);
    preencheRanking(movimento, pacman, rank);
    arquivoResumo(arq_resumo, pacman, movimento);
    preencheTrilha(pacman, linha, coluna, trilha);

    pacman = ReiniciaFlagsPacman(pacman); // Reinicializa flags
    ReiniciaFlagsFantasma(fantasma);

    if (fimJogo(pacman, qtd_comida, limite_movimentos)) { // Verifica as condicoes de game over
      break;
    }
  }

  fclose(arq_saida);
  fclose(arq_resumo);
  fclose(arqv_entrada);
  arquivoRanking(dir_ranking, rank); // Preenchimento de Arquivos
  arquivoEstatisticas(dir_estatisticas, rank, pacman);
  arquivoTrilha(dir_trilha, linha, coluna, trilha);

  return 0;
}

void criaMapa(FILE * file, int linha, int coluna, char mapa[linha][coluna]) {

  int j = 0;

  while (!feof(file)) {
    fscanf(file, "%[^\n]\n", mapa[j]);
    j++;
  }
}

tPac inicializaPacman(int l, int c, char mapa[l][c]) {

  int i, j, k = 0;
  tPac pacman;

  pacman.pontuacao = 0; // inicializa variaveis do pacman
  pacman.flagColisao = 0;
  pacman.flagComida = 0;
  pacman.flagPortal = 0;
  pacman.vida = 1;
  pacman.qtdMovimentos = 0;
  pacman.qtdColisoes = 0;

  for (i = 0; i < l; i++) {
    for (j = 0; j < c; j++) { // Armazena as Coordernadas do PacMan
      if (mapa[i][j] == '>') {
        pacman.posicao.linha = i;
        pacman.posicao.coluna = j;
      } else if (mapa[i][j] == '@' && k < 2) { // Armazena as Coordernadas dos Tuneis
        pacman.tunel[k].linha = i;
        pacman.tunel[k].coluna = j;
        k++;
      }
    }
  }

  return pacman;
}

void inicializaFantasmas(tFantasma fantasma[], int l, int c, char mapa[l][c]) {

  int i, j, k;

  fantasma[0].id = 'B';
  fantasma[1].id = 'C';
  fantasma[2].id = 'P';
  fantasma[3].id = 'I';

  for (i = 0; i < 4; i++) {

    fantasma[i].sentido = 1; // Sentido 1 e o sentido original do Fantasma
    fantasma[i].comida = 0; // Flag que verifica que o pacman ocupou posicao do fantasma
    fantasma[i].achouPacman = 0; // verifica o fantasma encontrou o pacman no seu caminho
    fantasma[i].flagMovimento = 0; // verifica se o fantasma movimentou
    fantasma[i].estaNoMapa = 0;

    for (j = 0; j < l; j++) {
      for (k = 0; k < c; k++) {
        if (mapa[j][k] == fantasma[i].id) {
          fantasma[i].posicao.linha = j; // armazenada coordenadas
          fantasma[i].posicao.coluna = k;
          fantasma[i].estaNoMapa = 1; // flag para sinalizar que o fantasma esta no mapa
        }
      }
    }
  }
}

void inicializaMapa(char diretorio[], tPac pacman, int l, int c, char mapa[l][c]) {

  FILE * arqv;
  int i, linha, coluna;

  linha = pacman.posicao.linha + 1; // atribui 1 por conta do indice da Matriz comecar em 0
  coluna = pacman.posicao.coluna + 1;

  arqv = fopen(diretorio, "w");

  for (i = 0; i < l; i++) {
    fprintf(arqv, "%s\n", mapa[i]);
  }

  fprintf(arqv, "Pac-Man comecara o jogo na linha %d e coluna %d\n", linha, coluna);
  fclose(arqv);
}

void inicializaRanking(tRanking rank[]) {

  int i;

  rank[0].mov = 'w'; 
  rank[1].mov = 's';
  rank[2].mov = 'a';
  rank[3].mov = 'd';

  for (i = 0; i < 4; i++) { // inicializacao das variaveis de cada movimento
    rank[i].qtdColisoes = 0;
    rank[i].qtd_comida = 0;
    rank[i].qtdMov = 0;
  }

}

void inicializaTrilha(tPac pacman, int l, int c, int trilha[l][c]) {

  int i, j;

  for (i = 0; i < l; i++) {
    for (j = 0; j < c - 1; j++) {
      if (i == pacman.posicao.linha && j == pacman.posicao.coluna) { // posicao Inicial do PacMan comeca em 0
        trilha[i][j] = 0;
      } else {
        trilha[i][j] = -1; // as demais celulas sao preenchidas com -1
      }
    }
  }

}

int qtdComidas(int l, int c, char mapa[l][c]) {

  int i, j, cont = 0;

  for (i = 0; i < l; i++) {
    for (j = 0; j < c; j++) {
      if (mapa[i][j] == '*') {
        cont++;
      }
    }
  }

  return cont; // retorna qtd de comidas do mapa
}

void fazMovimentacoes(tFantasma fantasma[], int l, int c, char mapa[l][c]) {

  int i, linha, coluna; // linha e coluna do fantasma
  char conteudo; // conteudo da celula 
  int a, b; // a e b irao manipular as linhas e as colunas

  for (i = 0; i < 4; i++) {

    if (fantasma[i].estaNoMapa && !fantasma[i].flagMovimento) {

      if (i <= 1) {
        a = identificaMovFantasma(i, fantasma);
        b = 0;
      } else {
        b = identificaMovFantasma(i, fantasma);
        a = 0;
      }

      linha = fantasma[i].posicao.linha; // Linha do Fantasma
      coluna = fantasma[i].posicao.coluna; // Coluna do Fantasma
      conteudo = mapa[linha + b][coluna + a]; // Conteudo da posicao relativa ao fantasma

      if (conteudo == '#') { // o fantasma mudara de sentido
        mudaSentidoFantasma(fantasma, i);
        a *= -1; // Altera os Sentidos
        b *= -1;
        conteudo = mapa[linha + b][coluna + a];
      }

      if (conteudo == ' ') {
        moveFantasma(fantasma, i, a, b, l, c, mapa);
        ocupouComida(i, fantasma, l, c, mapa); // Verifica se o Fantasma ocupou Comida
      } else if (conteudo == '*') { 

        moveFantasma(fantasma, i, a, b, l, c, mapa);
        fantasma[i].comida++;

        if (fantasma[i].comida > 1) {
          ocupouComida(i, fantasma, l, c, mapa);
        }

      } else if (conteudo == '>') { // casos de possivel colisao

        if (fantasma[i].achouPacman == 1) {
          moveFantasma(fantasma, i, a, b, l, c, mapa);
          ocupouComida(i, fantasma, l, c, mapa);
        } else {
          fantasma[i].achouPacman = 1; // Vai para o proximo Fantasma e espera o movimento do pacman
          continue; 
        }

      }

      fantasma[i].flagMovimento = 1; 
      fantasma[i].posicao.linha += b; // Atualiza posicoes do fantasma no mapa
      fantasma[i].posicao.coluna += a;
    }
  }

}

int identificaMovFantasma(int k, tFantasma fantasma[]) {

  if (fantasma[k].id == 'B' || fantasma[k].id == 'P') { // fantasmas que movimentam com -1
    if (fantasma[k].sentido == 1) {
      return -1;
    } else {
      return 1;
    }
  } else if (fantasma[k].id == 'C' || fantasma[k].id == 'I') { // fantasmas que movimentam com 1
    if (fantasma[k].sentido == 1) {
      return 1;
    } else {
      return -1;
    }
  }
}

void mudaSentidoFantasma(tFantasma fantasma[], int k) {

  if (fantasma[k].sentido == 1) { // O fantasma passa a se movimentar pelo sentido oposto
    fantasma[k].sentido = 2; // quando sentido e igual a 2 significa que esta movimentando pelo sentido oposto
  } else {
    fantasma[k].sentido = 1;
  }

}

void moveFantasma(tFantasma fantasma[], int k, int a, int b, int l, int c, char mapa[l][c]) {
  mapa[fantasma[k].posicao.linha + b][fantasma[k].posicao.coluna + a] = fantasma[k].id; // fantasma avanca no mapa
  mapa[fantasma[k].posicao.linha][fantasma[k].posicao.coluna] = ' ';
}

void ocupouComida(int k, tFantasma fantasma[], int l, int c, char mapa[l][c]) {

  if (fantasma[k].comida) { // em caso de ocupar comida, o fantasma avanca e coloca a comida na sua posicao atual
    mapa[fantasma[k].posicao.linha][fantasma[k].posicao.coluna] = '*';
    fantasma[k].comida--;
  }

}

tPac movimentaPacman(char mov, tPac pacman, int l, int c, char mapa[l][c]) {

  int a = 0, b = 0, flag = 1; // a manipula linha e b manipula coluna
  int coluna_pac, linha_pac; // flag para indicar se deve manipular linha e coluna

  coluna_pac = pacman.posicao.coluna;
  linha_pac = pacman.posicao.linha;

  if (mov == 'w') { // verifica movimento realizado pelo jogador
    a = -1;
  } else if (mov == 'a') {
    b = -1;
  } else if (mov == 's') {
    a = 1;
  } else if (mov == 'd') {
    b = 1;
  }

  if (mapa[linha_pac + a][coluna_pac + b] == '*') { // caso em que encontra comida
    
    pacman.pontuacao++; // aumenta pontuacao
    movePacman(pacman, a, b, l, c, mapa);
    pacman = ocupouPortal(pacman, l, c, mapa); // verifica se o pacman ocupou o portal
    pacman.flagComida = 1; // indica que o pacman ocupou uma posicao em que havia comida

  } else if (mapa[linha_pac + a][coluna_pac + b] == '#') { // colisao com a parede
    
    pacman.flagColisao = 1; // indica que o pacman colidiu com a parecde
    pacman.qtdColisoes++;
    pacman = ocupouPortal(pacman, l, c, mapa); // verifica se o pacman ocupou um portal

    if (mapa[linha_pac][coluna_pac] == '@') { // Nesse caso o pacman nao movimentou e o portal ressurge
      pacman = movPacPortal(pacman, 0, 0, l, c, mapa);
    }
    
    flag = 0; // a posicao do pacman nao e atualizada por nao ter movimentado
    
  } else if (eh_fantasma(mapa[linha_pac + a][coluna_pac + b])) { // Se caso ha um fantasma
    
    pacman.vida = 0; // pacman morre
    mapa[linha_pac][coluna_pac] = ' ';
    pacman = ocupouPortal(pacman, l, c, mapa);
    flag = 0;
    
  } else if (mapa[linha_pac + a][coluna_pac + b] == '@') { // se caso entrar em um portal
    
    pacman = movPacPortal(pacman, a, b, l, c, mapa);
    mapa[linha_pac][coluna_pac] = ' ';
    flag = 0;
    
  } else {
    movePacman(pacman, a, b, l, c, mapa); // em caso que a celula e apenas um espaco
    pacman = ocupouPortal(pacman, l, c, mapa);
  }

  if (flag) { // atualiza as posicoes do PacMan
    pacman.posicao.linha += a;
    pacman.posicao.coluna += b;
  }

  pacman.qtdMovimentos++;
  return pacman;
}

void movePacman(tPac pacman, int a, int b, int l, int c, char mapa[l][c]) {
  mapa[pacman.posicao.linha + a][pacman.posicao.coluna + b] = '>'; // movimenta o pacman pelo mapa
  mapa[pacman.posicao.linha][pacman.posicao.coluna] = ' ';
}

tPac movPacPortal(tPac pacman, int a, int b, int l, int c, char mapa[l][c]) {

  if (pacman.posicao.linha + a == pacman.tunel[0].linha && pacman.posicao.coluna + b == pacman.tunel[0].coluna) {
    mapa[pacman.tunel[1].linha][pacman.tunel[1].coluna] = '>'; // move o pacman para o proximo portal
    pacman.posicao.linha = pacman.tunel[1].linha; // atualiza a posicao do pacman com a mesma do portal
    pacman.posicao.coluna = pacman.tunel[1].coluna;
  } else {
    mapa[pacman.tunel[0].linha][pacman.tunel[0].coluna] = '>';
    pacman.posicao.linha = pacman.tunel[0].linha;
    pacman.posicao.coluna = pacman.tunel[0].coluna;
  }

  pacman.flagPortal = 1; // indica que o pacman entrou em um portal
  return pacman;
}

int todosMovimentaram(tFantasma fantasma[]) {

  int i, cont = 0;

  for (i = 0; i < 4; i++) {  // verifica se todos os fantasmas movimentaram
    if (fantasma[i].flagMovimento) {
      cont++;
    }
  }

  if (cont == 4) {
    return 1;
  } else {
    return 0;
  }

}

int verificaVidaPacman(tPac pacman, int l, int c, char mapa[l][c]) {

  if (mapa[pacman.posicao.linha][pacman.posicao.coluna] != '>') {
    return 0;
  } else {
    return 1;
  }

}

tPac ocupouPortal(tPac pacman, int l, int c, char mapa[l][c]) {

  if (pacman.flagPortal) { // a posicao anterior do pacman devolve o Portal
    mapa[pacman.posicao.linha][pacman.posicao.coluna] = '@';
    pacman.flagPortal = 0;
  }

  return pacman;
}

int eh_fantasma(char c) {

  if (c == 'B' || c == 'P' || c == 'I' || c == 'C') {
    return 1;
  } else {
    return 0;
  }
}

void arquivoJogo(FILE * file, tPac pacman, int qtd_comida, char mov, int lim_mov, int l, int c, char mapa[l][c]) {

  int i;

  fprintf(file, "Estado do jogo apos o movimento '%c':\n", mov);

  for (i = 0; i < l; i++) {
    fprintf(file, "%s\n", mapa[i]);
  }

  fprintf(file, "Pontuacao: %d\n\n", pacman.pontuacao);

  if (!pacman.vida || pacman.qtdMovimentos == lim_mov && pacman.pontuacao != qtd_comida) { // Quando morre ou atinge o limite de movimentos
    fprintf(file, "Game over!\nPontuacao final: %d\n", pacman.pontuacao);
  } else if (pacman.pontuacao == qtd_comida) { // se o pacman pegou todas as comidas
    fprintf(file, "Voce venceu!\nPontuacao final: %d\n", pacman.pontuacao);
  }

}

void preencheRanking(char movimento, tPac pacman, tRanking rank[]) {

  int i;

  for (i = 0; i < 4; i++) {

    if (movimento == rank[i].mov) {

      rank[i].qtdMov++;

      if (pacman.flagColisao && !pacman.flagPortal) { // se colidiu na parede e nao entrou no portal
        rank[i].qtdColisoes++;
      } else if (pacman.flagComida) {
        rank[i].qtd_comida++; 
      }
    }
  }
}

void arquivoResumo(FILE * file, tPac pacman, char mov) {

  if (pacman.vida) {

    if (pacman.flagColisao && !pacman.flagPortal) { // Colidiu na parede e nao entrou no portal
      fprintf(file, "Movimento %d (%c) colidiu na parede\n", pacman.qtdMovimentos, mov);
    } else if (pacman.flagComida) {
      fprintf(file, "Movimento %d (%c) pegou comida\n", pacman.qtdMovimentos, mov);
    }
  } 
  
  else {

    fprintf(file, "Movimento %d (%c) fim de jogo por encostar em um fantasma\n", pacman.qtdMovimentos, mov);

    if (pacman.flagColisao && !pacman.flagPortal) {
      fprintf(file, "Movimento %d (%c) colidiu na parede\n", pacman.qtdMovimentos, mov);
    }
  }
}

void preencheTrilha(tPac pacman, int l, int c, int trilha[l][c]) {

  if (pacman.flagPortal) { // condicao para colocar o n movimento no momento que entrar no portal

    if (pacman.posicao.linha == pacman.tunel[0].linha && pacman.posicao.coluna == pacman.tunel[0].coluna) {
      trilha[pacman.tunel[1].linha][pacman.tunel[1].coluna] = pacman.qtdMovimentos;
      trilha[pacman.posicao.linha][pacman.posicao.coluna] = pacman.qtdMovimentos;
    } else {
      trilha[pacman.tunel[0].linha][pacman.tunel[0].coluna] = pacman.qtdMovimentos;
      trilha[pacman.posicao.linha][pacman.posicao.coluna] = pacman.qtdMovimentos;
    }
  } 
  
  else {
    if (pacman.vida) {
      trilha[pacman.posicao.linha][pacman.posicao.coluna] = pacman.qtdMovimentos;
    }
  }
}

tPac ReiniciaFlagsPacman(tPac pacman) {
  pacman.flagColisao = 0;
  pacman.flagComida = 0;
  return pacman;
}

void ReiniciaFlagsFantasma(tFantasma fantasma[]) {

  int i;
  for (i = 0; i < 4; i++) {
    fantasma[i].achouPacman = 0;
    fantasma[i].flagMovimento = 0;
  }
  
}

int fimJogo(tPac pacman, int qtd_comida, int lim_mov) {

  if (!pacman.vida || pacman.pontuacao == qtd_comida) { // pacman morreu ou pegou todas as comidas
    return 1;
  } else if (pacman.qtdMovimentos == lim_mov) {
    return 1;
  } else {
    return 0;
  }

}

void arquivoRanking(char diretorio[], tRanking rank[]) {

  int i, j, flag = 0;
  tRanking aux;
  FILE * arq_ranking = fopen(diretorio, "w");

  for (i = 0; i < 4; i++) { // orderna o ranking com base nas condicoes

    for (j = i + 1; j < 4; j++) {

      if (rank[j].qtd_comida > rank[i].qtd_comida) {
        flag = 1;
      } else if (rank[j].qtd_comida == rank[i].qtd_comida) {

        if (rank[j].qtdColisoes < rank[i].qtdColisoes) {
          flag = 1;
        } else if (rank[j].qtdColisoes == rank[i].qtdColisoes) {

          if (rank[j].qtdMov > rank[i].qtdMov) {
            flag = 1;
          } else if (rank[j].qtdMov == rank[i].qtdMov) {

            if (rank[j].mov < rank[i].mov) {
              flag = 1;
            }
          }
        }
      }

      if (flag) { // se uma das condicoes e atendida faz uma ordernacao
        aux = rank[i];
        rank[i] = rank[j];
        rank[j] = aux;
      }

      flag = 0;

    }
  }

  for (i = 0; i < 4; i++) { 
    fprintf(arq_ranking, "%c,%d,%d,%d\n", rank[i].mov, rank[i].qtd_comida, rank[i].qtdColisoes, rank[i].qtdMov);
  }

  fclose(arq_ranking);
}

void arquivoEstatisticas(char diretorio[], tRanking rank[], tPac pacman) {

  int i, vet[4], qtd_semPontuar = 0;
  FILE * arqv = fopen(diretorio, "w");

  qtd_semPontuar = pacman.qtdMovimentos - pacman.pontuacao;

  for (i = 0; i < 4; i++) { // varredura para identificar os rankings corretamente 

    if (rank[i].mov == 's') {
      vet[0] = rank[i].qtdMov;
    } else if (rank[i].mov == 'w') {
      vet[1] = rank[i].qtdMov;
    } else if (rank[i].mov == 'a') {
      vet[2] = rank[i].qtdMov;
    } else if (rank[i].mov == 'd') {
      vet[3] = rank[i].qtdMov;
    }
  }

  fprintf(arqv, "Numero de movimentos: %d\n", pacman.qtdMovimentos);
  fprintf(arqv, "Numero de movimentos sem pontuar: %d\n", qtd_semPontuar);
  fprintf(arqv, "Numero de colisoes com parede: %d\n", pacman.qtdColisoes);
  fprintf(arqv, "Numero de movimentos para baixo: %d\n", vet[0]);
  fprintf(arqv, "Numero de movimentos para cima: %d\n", vet[1]);
  fprintf(arqv, "Numero de movimentos para esquerda: %d\n", vet[2]);
  fprintf(arqv, "Numero de movimentos para direita: %d\n", vet[3]);
  fclose(arqv);
}

void arquivoTrilha(char diretorio[], int l, int c, int trilha[l][c]) {

  FILE * arqv_trilha = fopen(diretorio, "w");
  int i, j;

  for (i = 0; i < l; i++) {

    for (j = 0; j < c - 1; j++) {

      if (trilha[i][j] == -1) { // lugares em que o pacman nao foi
        fprintf(arqv_trilha, "#");
      } else {
        fprintf(arqv_trilha, "%d", trilha[i][j]);
      }

      if (j != c - 2) { // Condicao para evitar de imprimir o caractere de espaco ao final da Linha
        fprintf(arqv_trilha, " ");
      }

    }

    fprintf(arqv_trilha, "\n");
  }

  fclose(arqv_trilha);
}