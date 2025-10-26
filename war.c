#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10
#define NUM_MISSOES 2

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Funções de setup e gerenciamento
Territorio* alocarMapa(int tamanho);
void inicializarTerritorios(Territorio *mapa, int tamanho);
void liberarMemoria(Territorio *mapa);

// Funções de interface com usuário
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa, int tamanho);
void exibirMissao(int missaoID);

// Funções de lógica principal
void faseDeAtaque(Territorio *mapa, int tamanho);
void simularAtaque(Territorio *origem, Territorio *destino);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int tamanho, const char *corJogador, int missaoID);

// Função utilitária
void limparBufferEntrada();

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand((unsigned int)time(NULL));

    Territorio *mapa = alocarMapa(NUM_TERRITORIOS);
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, NUM_TERRITORIOS);
    char corJogador[MAX_COR];
    printf("Escolha a cor do seu exército: ");
    scanf("%s", corJogador);
    limparBufferEntrada();

    int missaoID = sortearMissao();

    int opcao;
    do {
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(missaoID);
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2:
                if (verificarVitoria(mapa, NUM_TERRITORIOS, corJogador, missaoID)) {
                    printf("Parabens! Missao cumprida!\n");
                    opcao = 0; // encerra o jogo
                } else {
                    printf("Missao ainda nao cumprida.\n");
                }
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        printf("\n");
    } while(opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---

// Aloca memória para os territórios
Territorio* alocarMapa(int tamanho) {
    return (Territorio*)calloc(tamanho, sizeof(Territorio));
}

// Inicializa os territórios com dados padrões
void inicializarTerritorios(Territorio *mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("Cadastro do Territorio %d\n", i+1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exército: ");
        scanf("%s", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
        printf("\n");
    }
}

// Libera memória alocada
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

// Menu principal
void exibirMenuPrincipal() {
    printf("=== MENU PRINCIPAL ===\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
}

// Exibe o mapa atual
void exibirMapa(const Territorio *mapa, int tamanho) {
    printf("\n=== MAPA ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}

// Exibe a missão do jogador
void exibirMissao(int missaoID) {
    printf("=== MISSÃO ===\n");
    if (missaoID == 1) {
        printf("Destruir o exército de uma cor específica.\n");
    } else if (missaoID == 2) {
        printf("Conquistar pelo menos 3 territórios.\n");
    }
    printf("\n");
}

// Simula fase de ataque
void faseDeAtaque(Territorio *mapa, int tamanho) {
    int origem, destino;
    printf("Escolha o territorio de origem (1-%d): ", tamanho);
    scanf("%d", &origem);
    printf("Escolha o territorio de destino (1-%d): ", tamanho);
    scanf("%d", &destino);
    limparBufferEntrada();

    if (origem < 1 || origem > tamanho || destino < 1 || destino > tamanho || origem == destino) {
        printf("Territorios invalidos!\n");
        return;
    }

    simularAtaque(&mapa[origem-1], &mapa[destino-1]);
}

// Simula ataque entre dois territórios
void simularAtaque(Territorio *origem, Territorio *destino) {
    if (origem->tropas < 2) {
        printf("Nao ha tropas suficientes para atacar.\n");
        return;
    }

    int ataque = rand() % origem->tropas + 1;
    int defesa = rand() % destino->tropas + 1;

    printf("Ataque: %d | Defesa: %d\n", ataque, defesa);

    if (ataque > defesa) {
        printf("Territorio conquistado!\n");
        strcpy(destino->cor, origem->cor);
        destino->tropas = ataque - defesa;
        origem->tropas -= ataque - defesa;
    } else {
        printf("Ataque falhou!\n");
        origem->tropas -= ataque;
    }
}

// Sorteia uma missão aleatória
int sortearMissao() {
    return rand() % NUM_MISSOES + 1;
}

// Verifica se a missão foi cumprida
int verificarVitoria(const Territorio *mapa, int tamanho, const char *corJogador, int missaoID) {
    if (missaoID == 1) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0) return 0;
        }
        return 1;
    } else if (missaoID == 2) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) count++;
        }
        return count >= 3;
    }
    return 0;
}

// Limpa buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
