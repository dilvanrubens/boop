#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int gatinhos;
    int gatos;
} Jogador;

// Estrutura para representar o conteúdo de uma casa
typedef struct {
    char peca;      // ' ' (vazio), 'x'/'o' (gatinho), 'X'/'O' (gato)
    int jogador;    // 1 ou 2, 0 se vazio
} Casa;

char** criarTabuleiro(int linhas, int colunas) {
    char** tabuleiro = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
    return tabuleiro;
}

void exibeTabuleiro(char** tabuleiro, int linhas, int colunas, int pecasJogador1, int pecasJogador2) {
    system("cls || clear");

    for (int i = 0; i < linhas; i++) {
        printf("+");
        for (int j = 0; j < colunas; j++) {
            printf("---+");
        }
        printf("\n");

        for (int j = 0; j < colunas; j++) {
            printf("| %c ", tabuleiro[i][j]);
        }
        printf("| %d\n", i + 1);
    }
    printf("+");
    for (int j = 0; j < colunas; j++) {
        printf("---+");
    }
    printf("\n");

    printf("  ");
    for (int j = 0; j < colunas; j++) {
        printf(" %c  ", 'a' + j);
    }
    printf("\n");
}

int verificarLinhaDe3(char** tabuleiro, int linhas, int colunas, char peca) {
    // Verificar horizontal
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i][j+1] == peca && tabuleiro[i][j+2] == peca) {
                return 1;
            }
        }
    }

    // Verificar vertical
    for (int j = 0; j < colunas; j++) {
        for (int i = 0; i < linhas - 2; i++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i+1][j] == peca && tabuleiro[i+2][j] == peca) {
                return 1;
            }
        }
    }

    // Verificar diagonal principal
    for (int i = 0; i < linhas - 2; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i+1][j+1] == peca && tabuleiro[i+2][j+2] == peca) {
                return 1;
            }
        }
    }

    // Verificar diagonal secundária
    for (int i = 2; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i-1][j+1] == peca && tabuleiro[i-2][j+2] == peca) {
                return 1;
            }
        }
    }

    return 0;
}

void removerGatinhos(char** tabuleiro, int linhas, int colunas, char peca) {
    // Horizontal
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca &&
                tabuleiro[i][j+1] == peca &&
                tabuleiro[i][j+2] == peca) {
                tabuleiro[i][j] = ' ';
                tabuleiro[i][j+1] = ' ';
                tabuleiro[i][j+2] = ' ';
                return;
            }
        }
    }

    // Vertical
    for (int i = 0; i < linhas - 2; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == peca &&
                tabuleiro[i+1][j] == peca &&
                tabuleiro[i+2][j] == peca) {
                tabuleiro[i][j] = ' ';
                tabuleiro[i+1][j] = ' ';
                tabuleiro[i+2][j] = ' ';
                return;
            }
        }
    }

    // Diagonal principal
    for (int i = 0; i < linhas - 2; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca &&
                tabuleiro[i+1][j+1] == peca &&
                tabuleiro[i+2][j+2] == peca) {
                tabuleiro[i][j] = ' ';
                tabuleiro[i+1][j+1] = ' ';
                tabuleiro[i+2][j+2] = ' ';
                return;
            }
        }
    }

    // Diagonal secundária
    for (int i = 2; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca &&
                tabuleiro[i-1][j+1] == peca &&
                tabuleiro[i-2][j+2] == peca) {
                tabuleiro[i][j] = ' ';
                tabuleiro[i-1][j+1] = ' ';
                tabuleiro[i-2][j+2] = ' ';
                return;
            }
        }
    }
}

// Função boop modificada para considerar as regras específicas de Gatos
int boop(char** tabuleiro, int linhas, int colunas, int linha, int coluna, char peca, Jogador* jogador1, Jogador* jogador2) {
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},     // Ortogonais
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}    // Diagonais
    };

    int boopRealizado = 0;

    for (int i = 0; i < 8; i++) {
        int newRow = linha + directions[i][0];
        int newCol = coluna + directions[i][1];

        if (newRow >= 0 && newRow < linhas && newCol >= 0 && newCol < colunas) {
            char pecaAdj = tabuleiro[newRow][newCol];

            // Ignorar espaços vazios
            if (pecaAdj == ' ') continue;

            // Gatinho não pode empurrar Gato
            if ((peca == 'x' || peca == 'o') && (pecaAdj == 'X' || pecaAdj == 'O')) continue;

            int pushRow = newRow + directions[i][0];
            int pushCol = newCol + directions[i][1];

            // Se empurrar para fora do tabuleiro
            if (pushRow < 0 || pushRow >= linhas || pushCol < 0 || pushCol >= colunas) {
                // Devolver peça para o jogador correto
                if (pecaAdj == 'x' || pecaAdj == 'X') {
                    if (pecaAdj == 'x') jogador1->gatinhos++;
                    else jogador1->gatos++;
                } else {
                    if (pecaAdj == 'o') jogador2->gatinhos++;
                    else jogador2->gatos++;
                }
                tabuleiro[newRow][newCol] = ' ';
                boopRealizado = 1;
            }
            // Se empurrar para espaço vazio
            else if (tabuleiro[pushRow][pushCol] == ' ') {
                tabuleiro[pushRow][pushCol] = tabuleiro[newRow][newCol];
                tabuleiro[newRow][newCol] = ' ';
                boopRealizado = 1;
            }
        }
    }
    return boopRealizado;
}

// Função auxiliar para verificar e processar linha (mover para fora da função graduarLinhaDe3)
int verificarLinha(char p1, char p2, char p3, char pecaGatinho, char pecaGato) {
    int gatinhosCount = 0;
    int gatosCount = 0;

    if (p1 == pecaGatinho) gatinhosCount++;
    else if (p1 == pecaGato) gatosCount++;

    if (p2 == pecaGatinho) gatinhosCount++;
    else if (p2 == pecaGato) gatosCount++;

    if (p3 == pecaGatinho) gatinhosCount++;
    else if (p3 == pecaGato) gatosCount++;

    return (gatinhosCount == 2 && gatosCount == 1) ||
           (gatinhosCount == 1 && gatosCount == 2);
}

// Função para verificar e graduar linha de 3 (incluindo combinações de Gatos e Gatinhos)
int graduarLinhaDe3(char** tabuleiro, int linhas, int colunas, char pecaGatinho, char pecaGato, Jogador* jogador) {
    int graduou = 0;

    // Verificar horizontal
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            char p1 = tabuleiro[i][j];
            char p2 = tabuleiro[i][j+1];
            char p3 = tabuleiro[i][j+2];

            if ((p1 == pecaGatinho || p1 == pecaGato) &&
                (p2 == pecaGatinho || p2 == pecaGato) &&
                (p3 == pecaGatinho || p3 == pecaGato)) {

                if (verificarLinha(p1, p2, p3, pecaGatinho, pecaGato)) {
                    jogador->gatos += 3; // 2 novos + 1 devolvido

                    tabuleiro[i][j] = ' ';
                    tabuleiro[i][j+1] = ' ';
                    tabuleiro[i][j+2] = ' ';

                    return 1;
                }
            }
        }
    }

    // Verificar vertical
    for (int j = 0; j < colunas; j++) {
        for (int i = 0; i < linhas - 2; i++) {
            char p1 = tabuleiro[i][j];
            char p2 = tabuleiro[i+1][j];
            char p3 = tabuleiro[i+2][j];

            if ((p1 == pecaGatinho || p1 == pecaGato) &&
                (p2 == pecaGatinho || p2 == pecaGato) &&
                (p3 == pecaGatinho || p3 == pecaGato)) {

                if (verificarLinha(p1, p2, p3, pecaGatinho, pecaGato)) {
                    jogador->gatos += 3; // 2 novos + 1 devolvido

                    tabuleiro[i][j] = ' ';
                    tabuleiro[i+1][j] = ' ';
                    tabuleiro[i+2][j] = ' ';

                    return 1;
                }
            }
        }
    }

    // Diagonal principal
    for (int i = 0; i < linhas - 2; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            char p1 = tabuleiro[i][j];
            char p2 = tabuleiro[i+1][j+1];
            char p3 = tabuleiro[i+2][j+2];

            if ((p1 == pecaGatinho || p1 == pecaGato) &&
                (p2 == pecaGatinho || p2 == pecaGato) &&
                (p3 == pecaGatinho || p3 == pecaGato)) {

                if (verificarLinha(p1, p2, p3, pecaGatinho, pecaGato)) {
                    jogador->gatos += 3; // 2 novos + 1 devolvido

                    tabuleiro[i][j] = ' ';
                    tabuleiro[i+1][j+1] = ' ';
                    tabuleiro[i+2][j+2] = ' ';

                    return 1;
                }
            }
        }
    }

    // Diagonal secundária
    for (int i = 2; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            char p1 = tabuleiro[i][j];
            char p2 = tabuleiro[i-1][j+1];
            char p3 = tabuleiro[i-2][j+2];

            if ((p1 == pecaGatinho || p1 == pecaGato) &&
                (p2 == pecaGatinho || p2 == pecaGato) &&
                (p3 == pecaGatinho || p3 == pecaGato)) {

                if (verificarLinha(p1, p2, p3, pecaGatinho, pecaGato)) {
                    jogador->gatos += 3; // 2 novos + 1 devolvido

                    tabuleiro[i][j] = ' ';
                    tabuleiro[i-1][j+1] = ' ';
                    tabuleiro[i-2][j+2] = ' ';

                    return 1;
                }
            }
        }
    }

    return graduou;
}

typedef struct {
    int linhas;
    int colunas;
    char** tabuleiro;
    Jogador jog1;
    Jogador jog2;
    int jogada;
} EstadoJogo;

void salvarJogo(EstadoJogo* estado) {
    FILE* arquivo = fopen("jogo_salvo.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de save!\n");
        return;
    }

    fwrite(&estado->linhas, sizeof(int), 1, arquivo);
    fwrite(&estado->colunas, sizeof(int), 1, arquivo);

    for (int i = 0; i < estado->linhas; i++) {
        fwrite(estado->tabuleiro[i], sizeof(char), estado->colunas, arquivo);
    }

    fwrite(&estado->jog1, sizeof(Jogador), 1, arquivo);
    fwrite(&estado->jog2, sizeof(Jogador), 1, arquivo);
    fwrite(&estado->jogada, sizeof(int), 1, arquivo);

    fclose(arquivo);
    printf("\nJogo salvo com sucesso!\n");
}

int carregarJogo(EstadoJogo* estado) {
    FILE* arquivo = fopen("jogo_salvo.bin", "rb");
    if (arquivo == NULL) {
        printf("Nenhum jogo salvo encontrado!\n");
        return 0;
    }

    fread(&estado->linhas, sizeof(int), 1, arquivo);
    fread(&estado->colunas, sizeof(int), 1, arquivo);

    estado->tabuleiro = criarTabuleiro(estado->linhas, estado->colunas);

    for (int i = 0; i < estado->linhas; i++) {
        fread(estado->tabuleiro[i], sizeof(char), estado->colunas, arquivo);
    }

    fread(&estado->jog1, sizeof(Jogador), 1, arquivo);
    fread(&estado->jog2, sizeof(Jogador), 1, arquivo);
    fread(&estado->jogada, sizeof(int), 1, arquivo);

    fclose(arquivo);
    return 1;
}

void jogar(EstadoJogo* estado) {
    char peca;
    char colunaLetra;
    int linhaEscolhida;

    while (1) {
        exibeTabuleiro(estado->tabuleiro, estado->linhas, estado->colunas, estado->jog1.gatinhos, estado->jog2.gatinhos);

        printf("\n=== Placar de Gatos ===\n");
        printf("Jogador (X): %d Gatos\n", estado->jog1.gatos);
        printf("Jogador (O): %d Gatos\n", estado->jog2.gatos);

        Jogador* jogAtual = (estado->jogada % 2 == 1) ? &estado->jog1 : &estado->jog2;
        char pecaGatinho = (estado->jogada % 2 == 1) ? 'x' : 'o';
        char pecaGato = (estado->jogada % 2 == 1) ? 'X' : 'O';

        printf("\n========= TURNO DO JOGADOR %c =========\n",
               estado->jogada % 2 == 1 ? 'X' : 'O');

        printf("\nPecas disponiveis:\n");
        printf("- Gatinhos: %d\n", jogAtual->gatinhos);
        if (jogAtual->gatos > 0) {
            printf("- Gatos: %d\n", jogAtual->gatos);
        }

        int escolha = 1;
        if (jogAtual->gatos > 0) {
            printf("\nEscolha sua peca:\n");
            printf("1 - Gatinho\n");
            printf("2 - Gato\n");
            printf("Opcao: ");
            scanf("%d", &escolha);

            while (escolha != 1 && escolha != 2) {
                printf("Opcao invalida!\n");
                printf("1 - Gatinho\n");
                printf("2 - Gato\n");
                printf("Escolha novamente: ");
                scanf("%d", &escolha);
            }
        }

        peca = (escolha == 1) ? pecaGatinho : pecaGato;

        if ((escolha == 1 && jogAtual->gatinhos <= 0) ||
            (escolha == 2 && jogAtual->gatos <= 0)) {
            printf("Voce nao tem mais pecas desse tipo!\n");
            continue;
        }

        printf("Escolha a coluna (a-%c) e linha (1-%d) ou digite 's' para salvar: ", 'a' + estado->colunas - 1, estado->linhas);
        char input[10];
        scanf("%s", input);

        if (input[0] == 's' || input[0] == 'S') {
            salvarJogo(estado);
            printf("Jogo salvo!\n");
            continue;
        }

        colunaLetra = input[0];
        linhaEscolhida = atoi(&input[1]) - 1;
        int coluna = colunaLetra - 'a';

        if (linhaEscolhida >= 0 && linhaEscolhida < estado->linhas &&
            coluna >= 0 && coluna < estado->colunas &&
            estado->tabuleiro[linhaEscolhida][coluna] == ' ') {

            estado->tabuleiro[linhaEscolhida][coluna] = peca;

            if (escolha == 1) {
                jogAtual->gatinhos--;
            } else {
                jogAtual->gatos--;
            }

            if (boop(estado->tabuleiro, estado->linhas, estado->colunas, linhaEscolhida, coluna, peca, &estado->jog1, &estado->jog2)) {
                printf("\n     >> Boop! Peca adversaria empurrada!\n");
            }

            if (escolha == 1 && verificarLinhaDe3(estado->tabuleiro, estado->linhas, estado->colunas, peca)) {
                exibeTabuleiro(estado->tabuleiro, estado->linhas, estado->colunas, estado->jog1.gatinhos, estado->jog2.gatinhos);
                printf("Linha de 3 Gatinhos formada! Ganhando 3 Gatos!\n");
                removerGatinhos(estado->tabuleiro, estado->linhas, estado->colunas, peca);
                jogAtual->gatos += 3;
            }

            if (graduarLinhaDe3(estado->tabuleiro, estado->linhas, estado->colunas, pecaGatinho, pecaGato, jogAtual)) {
                printf("\n- Linha formada!\n");
                printf("- Os gatinhos foram promovidos a gatos!\n");
                printf("- O gato da linha voltou para sua reserva!\n");
            }

            if (verificarLinhaDe3(estado->tabuleiro, estado->linhas, estado->colunas, pecaGato)) {
                exibeTabuleiro(estado->tabuleiro, estado->linhas, estado->colunas, estado->jog1.gatinhos, estado->jog2.gatinhos);
                printf("\n==========================================\n");
                printf("||            VITORIA!!!                ||\n");
                printf("||      Jogador %c venceu o jogo!       ||\n",
                       estado->jogada % 2 == 1 ? 'X' : 'O');
                printf("==========================================\n\n");
                printf("Pressione qualquer tecla para continuar...\n");
                getchar(); // Espera o usuário pressionar uma tecla
                getchar(); // Captura o '\n' deixado pelo scanf anterior
                break; // Sai do loop para retornar ao menu inicial
            }

            estado->jogada++;
        } else {
            printf("Jogada invalida! Tente novamente.\n");
        }

        if (estado->jog1.gatinhos == 0 && estado->jog2.gatinhos == 0 &&
            estado->jog1.gatos == 0 && estado->jog2.gatos == 0) {
            exibeTabuleiro(estado->tabuleiro, estado->linhas, estado->colunas, estado->jog1.gatinhos, estado->jog2.gatinhos);
            printf("\n==========================================\n");
            printf("||              EMPATE!!!               ||\n");
            printf("||      Todas as pecas foram usadas     ||\n");
            printf("==========================================\n\n");
            break;
        }
    }

    // Libera a memória do tabuleiro após o término do jogo
    for (int i = 0; i < estado->linhas; i++) {
        free(estado->tabuleiro[i]);
    }
    free(estado->tabuleiro);
}

// Função que retorna o conteúdo de uma casa
Casa naCasa(char** tabuleiro, int linha, int coluna, int linhas, int colunas) {
    Casa casa = {' ', 0};

    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        casa.peca = tabuleiro[linha][coluna];
        if (casa.peca != ' ') {
            casa.jogador = (casa.peca == 'x' || casa.peca == 'X') ? 1 : 2;
        }
    }

    return casa;
}

// Função que valida uma jogada
int validaJogada(char** tabuleiro, int linha, int coluna, int linhas, int colunas) {
    // Verifica se está dentro dos limites do tabuleiro
    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas) {
        return 0;
    }

    // Verifica se a casa está vazia
    if (tabuleiro[linha][coluna] != ' ') {
        return 0;
    }

    return 1;
}

// Função que realiza o boop (renomeada de acordo com o requisito)
int fazBoop(char** tabuleiro, int linha, int coluna, int linhas, int colunas) {
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},     // Ortogonais
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}    // Diagonais
    };

    Casa casa = naCasa(tabuleiro, linha, coluna, linhas, colunas);
    int boopRealizado = 0;

    for (int i = 0; i < 8; i++) {
        int newRow = linha + directions[i][0];
        int newCol = coluna + directions[i][1];

        if (newRow >= 0 && newRow < linhas && newCol >= 0 && newCol < colunas) {
            Casa casaAdj = naCasa(tabuleiro, newRow, newCol, linhas, colunas);

            // Verifica regras de boop...
            if (casaAdj.peca != ' ' && casaAdj.jogador != casa.jogador) {
                int pushRow = newRow + directions[i][0];
                int pushCol = newCol + directions[i][1];

                if (pushRow < 0 || pushRow >= linhas ||
                    pushCol < 0 || pushCol >= colunas ||
                    tabuleiro[pushRow][pushCol] == ' ') {

                    // Realiza o boop
                    if (pushRow >= 0 && pushRow < linhas &&
                        pushCol >= 0 && pushCol < colunas) {
                        tabuleiro[pushRow][pushCol] = tabuleiro[newRow][newCol];
                    }
                    tabuleiro[newRow][newCol] = ' ';
                    boopRealizado = 1;
                }
            }
        }
    }
    return boopRealizado;
}

// Função que verifica e realiza graduações
int graduar(char** tabuleiro, int linhas, int colunas) {
    // Reutiliza a lógica da função graduarLinhaDe3, mas simplificada
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            Casa c1 = naCasa(tabuleiro, i, j, linhas, colunas);
            Casa c2 = naCasa(tabuleiro, i, j+1, linhas, colunas);
            Casa c3 = naCasa(tabuleiro, i, j+2, linhas, colunas);

            if (c1.jogador == c2.jogador && c2.jogador == c3.jogador && c1.jogador != 0) {
                // Verifica condições de graduação...
                return 1;
            }
        }
    }
    // Verificar vertical e diagonais similarmente...
    return 0;
}

// Função que verifica vitória
int verificaVitoria(char** tabuleiro, Casa casa, int linhas, int colunas) {
    // Verifica se há linha de 3 gatos do mesmo jogador
    char pecaGato = (casa.jogador == 1) ? 'X' : 'O';
    return verificarLinhaDe3(tabuleiro, linhas, colunas, pecaGato);
}

void telaInicial() {
    system("cls || clear");
    printf("\n\n");
    printf("==========================================\n");
    printf("||                                      ||\n");
    printf("||                 BOOP!                ||\n");
    printf("||                                      ||\n");
    printf("||        1 - Novo Jogo                 ||\n");
    printf("||        2 - Carregar Jogo             ||\n");
    printf("||        3 - Sair                      ||\n");
    printf("||                                      ||\n");
    printf("==========================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    EstadoJogo estado;

    do {
        telaInicial();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                estado.linhas = 6;
                estado.colunas = 6;
                estado.tabuleiro = criarTabuleiro(estado.linhas, estado.colunas);
                estado.jog1 = (Jogador){8, 0};
                estado.jog2 = (Jogador){8, 0};
                estado.jogada = 1;
                jogar(&estado);
                break;
            case 2:
                if (carregarJogo(&estado)) {
                    jogar(&estado);
                }
                break;
            case 3:
                printf("Obrigado por jogar!\n");
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);

    return 0;
}

