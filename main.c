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
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i][j+1] == peca && tabuleiro[i][j+2] == peca) {
                return 1;
            }
        }
    }

    for (int i = 0; i < linhas - 2; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i+1][j] == peca && tabuleiro[i+2][j] == peca) {
                return 1;
            }
        }
    }

    for (int i = 0; i < linhas - 2; i++) {
        for (int j = 0; j < colunas - 2; j++) {
            if (tabuleiro[i][j] == peca && tabuleiro[i+1][j+1] == peca && tabuleiro[i+2][j+2] == peca) {
                return 1;
            }
        }
    }

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

// Função modificada para jogar
void jogar() {
    int linhas = 6, colunas = 6;
    char** tabuleiro = NULL;
    Jogador jog1 = {8, 0};
    Jogador jog2 = {8, 0};
    int jogada = 1;

    // Declarar as variáveis que estavam faltando
    char peca;
    char colunaLetra;
    int linhaEscolhida;

    // Declarar os protótipos das funções no início do arquivo
    void salvarJogo(char** tabuleiro, int linhas, int colunas, Jogador jog1, Jogador jog2, int jogada);
    int carregarJogo(char*** tabuleiro, int* linhas, int* colunas, Jogador* jog1, Jogador* jog2, int* jogada);

    // Perguntar se deseja carregar jogo salvo
    printf("Deseja carregar um jogo salvo? (S/N): ");
    char resposta;
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        if (!carregarJogo(&tabuleiro, &linhas, &colunas, &jog1, &jog2, &jogada)) {
            tabuleiro = criarTabuleiro(linhas, colunas);
        }
    } else {
        tabuleiro = criarTabuleiro(linhas, colunas);
    }

    while (1) {
        exibeTabuleiro(tabuleiro, linhas, colunas, jog1.gatinhos, jog2.gatinhos);

        // Adicionar opção de salvar
        printf("\nDigite 'S' para salvar o jogo ou pressione ENTER para continuar: ");
        char opcao;
        while ((opcao = getchar()) != '\n') {
            if (opcao == 'S' || opcao == 's') {
                salvarJogo(tabuleiro, linhas, colunas, jog1, jog2, jogada);
                printf("Deseja sair do jogo? (S/N): ");
                scanf(" %c", &opcao);
                if (opcao == 'S' || opcao == 's') {
                    // Liberar memória e sair
                    for (int i = 0; i < linhas; i++) {
                        free(tabuleiro[i]);
                    }
                    free(tabuleiro);
                    return;
                }
                break;
            }
        }

        printf("\n=== Placar de Gatos ===\n");
        printf("Jogador (X): %d Gatos\n", jog1.gatos);
        printf("Jogador (O): %d Gatos\n", jog2.gatos);

        Jogador* jogAtual = (jogada % 2 == 1) ? &jog1 : &jog2;
        char pecaGatinho = (jogada % 2 == 1) ? 'x' : 'o';
        char pecaGato = (jogada % 2 == 1) ? 'X' : 'O';

        printf("\n========= TURNO DO JOGADOR %c =========\n",
               jogada % 2 == 1 ? 'X' : 'O');

        printf("\nPecas disponiveis:\n");
        printf("- Gatinhos: %d\n", jogAtual->gatinhos);
        if (jogAtual->gatos > 0) {
            printf("- Gatos: %d\n", jogAtual->gatos);
        }

        int escolha = 1; // Começa assumindo gatinho
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

        printf("Escolha a coluna (a-%c) e linha (1-%d): ", 'a' + colunas - 1, linhas);
        scanf(" %c %d", &colunaLetra, &linhaEscolhida);
        int coluna = colunaLetra - 'a';
        linhaEscolhida--;

        if (linhaEscolhida >= 0 && linhaEscolhida < linhas &&
            coluna >= 0 && coluna < colunas &&
            tabuleiro[linhaEscolhida][coluna] == ' ') {

            tabuleiro[linhaEscolhida][coluna] = peca;

            // Atualizar contadores
            if (escolha == 1) {
                jogAtual->gatinhos--;
            } else {
                jogAtual->gatos--;
            }

            // Realizar boop PRIMEIRO
            if (boop(tabuleiro, linhas, colunas, linhaEscolhida, coluna, peca, &jog1, &jog2)) {
                printf("\n     >> Boop! Peca adversaria empurrada!\n");
            }

            // Verificar linha de 3 Gatinhos APÓS o boop
            if (escolha == 1 && verificarLinhaDe3(tabuleiro, linhas, colunas, peca)) {
                exibeTabuleiro(tabuleiro, linhas, colunas, jog1.gatinhos, jog2.gatinhos);
                printf("Linha de 3 Gatinhos formada! Ganhando 3 Gatos!\n");
                removerGatinhos(tabuleiro, linhas, colunas, peca);
                jogAtual->gatos += 3;
            }

            // Verificar graduação após verificar 3 gatinhos
            if (graduarLinhaDe3(tabuleiro, linhas, colunas, pecaGatinho, pecaGato, jogAtual)) {
                printf("\n- Linha formada!\n");
                printf("- Os gatinhos foram promovidos a gatos!\n");
                printf("- O gato da linha voltou para sua reserva!\n");
            }

            // Verificar vitória por linha de 3 Gatos
            if (verificarLinhaDe3(tabuleiro, linhas, colunas, pecaGato)) {
                exibeTabuleiro(tabuleiro, linhas, colunas, jog1.gatinhos, jog2.gatinhos);
                printf("\n==========================================\n");
                printf("||            VITORIA!!!                ||\n");
                printf("||      Jogador %c venceu o jogo!       ||\n",
                       jogada % 2 == 1 ? 'X' : 'O');
                printf("==========================================\n\n");
                break;
            }

            jogada++;
        } else {
            printf("Jogada invalida! Tente novamente.\n");
        }

        // Verificar empate (todas as peças usadas e ninguém ganhou)
        if (jog1.gatinhos == 0 && jog2.gatinhos == 0 &&
            jog1.gatos == 0 && jog2.gatos == 0) {
            exibeTabuleiro(tabuleiro, linhas, colunas, jog1.gatinhos, jog2.gatinhos);
            printf("\n==========================================\n");
            printf("||              EMPATE!!!               ||\n");
            printf("||      Todas as pecas foram usadas     ||\n");
            printf("==========================================\n\n");
            break;
        }
    }

    // Liberar memória
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
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
    do {
        telaInicial();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                jogar();
                break;
            case 2:
                jogar(); // Vai perguntar se quer carregar jogo
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

// Função para salvar o estado do jogo
void salvarJogo(char** tabuleiro, int linhas, int colunas, Jogador jog1, Jogador jog2, int jogada) {
    FILE* arquivo = fopen("jogo_salvo.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de save!\n");
        return;
    }

    // Salvar dimensões do tabuleiro
    fwrite(&linhas, sizeof(int), 1, arquivo);
    fwrite(&colunas, sizeof(int), 1, arquivo);

    // Salvar estado do tabuleiro
    for (int i = 0; i < linhas; i++) {
        fwrite(tabuleiro[i], sizeof(char), colunas, arquivo);
    }

    // Salvar estado dos jogadores
    fwrite(&jog1, sizeof(Jogador), 1, arquivo);
    fwrite(&jog2, sizeof(Jogador), 1, arquivo);

    // Salvar número da jogada atual
    fwrite(&jogada, sizeof(int), 1, arquivo);

    fclose(arquivo);
    printf("\nJogo salvo com sucesso!\n");
}

// Função para carregar jogo salvo
int carregarJogo(char*** tabuleiro, int* linhas, int* colunas, Jogador* jog1, Jogador* jog2, int* jogada) {
    FILE* arquivo = fopen("jogo_salvo.bin", "rb");
    if (arquivo == NULL) {
        printf("Nenhum jogo salvo encontrado!\n");
        return 0;
    }

    // Ler dimensões do tabuleiro
    fread(linhas, sizeof(int), 1, arquivo);
    fread(colunas, sizeof(int), 1, arquivo);

    // Criar novo tabuleiro
    *tabuleiro = criarTabuleiro(*linhas, *colunas);

    // Carregar estado do tabuleiro
    for (int i = 0; i < *linhas; i++) {
        fread((*tabuleiro)[i], sizeof(char), *colunas, arquivo);
    }

    // Carregar estado dos jogadores
    fread(jog1, sizeof(Jogador), 1, arquivo);
    fread(jog2, sizeof(Jogador), 1, arquivo);

    // Carregar número da jogada atual
    fread(jogada, sizeof(int), 1, arquivo);

    fclose(arquivo);
    return 1;
}

