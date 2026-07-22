#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // Para strcasecmp
#include <time.h>    // Para medicao de tempo de execucao

#define MAX_COMPONENTES 20

// -----------------------------------------------------------------------------
// 1. ESTRUTURA DE DADOS
// -----------------------------------------------------------------------------

// Struct para representar cada componente da Torre de Fuga
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // Valor de 1 a 10
} Componente;

// Variável global para contagem de comparações (Análise de Desempenho)
long long comparacoes = 0;

// -----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------------------------------------
void cadastrarComponentes(Componente torre[], int *total);
void mostrarComponentes(const Componente torre[], int total);
void limparBuffer(void);

// Algoritmos de Ordenação
void bubbleSortNome(Componente torre[], int n);
void insertionSortTipo(Componente torre[], int n);
void selectionSortPrioridade(Componente torre[], int n);

// Medição de Desempenho
void medirTempo(void (*algoritmo)(Componente[], int), Componente torre[], int n);

// Busca Binária
int buscaBinariaPorNome(const Componente torre[], int n, const char *nomeChave);

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------------------------------------
int main() {
    Componente torre[MAX_COMPONENTES];
    int total = 0;
    int ordenadoPorNome = 0; // Flag exigida para liberar a busca binária
    int opcao;

    // Inserção inicial automatizada de itens padrão para facilitar testes rápidos
    strcpy(torre[0].nome, "Chip Central");      strcpy(torre[0].tipo, "Controle");  torre[0].prioridade = 10;
    strcpy(torre[1].nome, "Propulsor Alfa");    strcpy(torre[1].tipo, "Propulsao"); torre[1].prioridade = 8;
    strcpy(torre[2].nome, "Base de Titanio");   strcpy(torre[2].tipo, "Suporte");   torre[2].prioridade = 5;
    strcpy(torre[3].nome, "Antena de Sinal");   strcpy(torre[3].tipo, "Controle");  torre[3].prioridade = 7;
    strcpy(torre[4].nome, "Bateria de Fusao");  strcpy(torre[4].tipo, "Energia");   torre[4].prioridade = 9;
    total = 5;

    do {
        printf("\n==================================================\n");
        printf("    TORRE DE RESGATE - SAFE ZONE FINAL (ESCAPE)   \n");
        printf("==================================================\n");
        printf("Componentes prontos: %d/%d\n", total, MAX_COMPONENTES);
        printf("--------------------------------------------------\n");
        printf("1. Cadastrar novo componente\n");
        printf("2. Listar componentes atuai\n");
        printf("3. Ordenar por NOME (Bubble Sort)\n");
        printf("4. Ordenar por TIPO (Insertion Sort)\n");
        printf("5. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6. Localizar Componente-Chave (Busca Binaria)\n");
        printf("0. Ativar Torre e Escapar!\n");
        printf("--------------------------------------------------\n");
        printf("Escolha uma acao estrategica: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarComponentes(torre, &total);
                ordenadoPorNome = 0;
                break;

            case 2:
                mostrarComponentes(torre, total);
                break;

            case 3:
                printf("\n>>> Executando BUBBLE SORT (Por Nome)... <<<\n");
                medirTempo(bubbleSortNome, torre, total);
                mostrarComponentes(torre, total);
                ordenadoPorNome = 1; // Habilita busca binária
                break;

            case 4:
                printf("\n>>> Executando INSERTION SORT (Por Tipo)... <<<\n");
                medirTempo(insertionSortTipo, torre, total);
                mostrarComponentes(torre, total);
                ordenadoPorNome = 0;
                break;

            case 5:
                printf("\n>>> Executando SELECTION SORT (Por Prioridade)... <<<\n");
                medirTempo(selectionSortPrioridade, torre, total);
                mostrarComponentes(torre, total);
                ordenadoPorNome = 0;
                break;

            case 6:
                if (!ordenadoPorNome) {
                    printf("\n[BLOQUEADO] A Busca Binaria exige que a torre esteja ordenada por NOME.\n");
                    printf("Execute a opcao 3 (Bubble Sort) antes de realizar a busca!\n");
                } else {
                    char chave[30];
                    printf("\nDigite o nome do componente-chave para ativacao (ex: Chip Central): ");
                    fgets(chave, sizeof(chave), stdin);
                    chave[strcspn(chave, "\n")] = '\0';

                    comparacoes = 0;
                    int pos = buscaBinariaPorNome(torre, total, chave);

                    if (pos != -1) {
                        printf("\n==================================================\n");
                        printf(" [SISTEMA ATIVADO] Componente-chave localizado!\n");
                        printf(" -> Nome:       %s\n", torre[pos].nome);
                        printf(" -> Tipo:       %s\n", torre[pos].tipo);
                        printf(" -> Prioridade: %d\n", torre[pos].prioridade);
                        printf(" -> Slot:       [%d]\n", pos + 1);
                        printf(" -> Comparacoes na Busca Binaria: %lld\n", comparacoes);
                        printf("==================================================\n");
                    } else {
                        printf("\n[ALERTA RED] Componente \"%s\" NAO localizado na torre!\n", chave);
                        printf("-> Comparacoes na Busca Binaria: %lld\n", comparacoes);
                    }
                }
                break;

            case 0:
                printf("\n[SISTEMA] Sequencia de escape iniciada! Boa sorte no resgate!\n");
                break;

            default:
                printf("\nOpcao invalida! Escolha um comando valido.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

// -----------------------------------------------------------------------------
// FUNÇÕES AUXILIARES E DE ENTRADA
// -----------------------------------------------------------------------------

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarComponentes(Componente torre[], int *total) {
    if (*total >= MAX_COMPONENTES) {
        printf("\n[AVISO] Capacidade maxima da Torre de Resgate atingida (%d/%d).\n", *total, MAX_COMPONENTES);
        return;
    }

    Componente c;
    printf("\n--- CADASTRAR COMPONENTE DA TORRE ---\n");
    printf("Nome do componente: ");
    fgets(c.nome, sizeof(c.nome), stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("Tipo (ex: controle, suporte, propulsao): ");
    fgets(c.tipo, sizeof(c.tipo), stdin);
    c.tipo[strcspn(c.tipo, "\n")] = '\0';

    printf("Prioridade (1 a 10): ");
    while (scanf("%d", &c.prioridade) != 1 || c.prioridade < 1 || c.prioridade > 10) {
        printf("Prioridade invalida! Digite um valor entre 1 e 10: ");
        limparBuffer();
    }
    limparBuffer();

    torre[*total] = c;
    (*total)++;
    printf("[SUCESSO] Componente \"%s\" adicionado a torre!\n", c.nome);
}

void mostrarComponentes(const Componente torre[], int total) {
    printf("\n=================== MONTAGEM DA TORRE ===================\n");
    if (total == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("[%02d] Nome: %-18s | Tipo: %-12s | Prioridade: %d/10\n", 
               i + 1, torre[i].nome, torre[i].tipo, torre[i].prioridade);
    }
    printf("=========================================================\n");
}

// -----------------------------------------------------------------------------
// ALGORITMOS DE ORDENAÇÃO
// -----------------------------------------------------------------------------

// 1. Bubble Sort: Ordena por NOME (string)
void bubbleSortNome(Componente torre[], int n) {
    comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcasecmp(torre[j].nome, torre[j + 1].nome) > 0) {
                Componente temp = torre[j];
                torre[j] = torre[j + 1];
                torre[j + 1] = temp;
            }
        }
    }
}

// 2. Insertion Sort: Ordena por TIPO (string)
void insertionSortTipo(Componente torre[], int n) {
    comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente pivo = torre[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoes++;
            if (strcasecmp(torre[j].tipo, pivo.tipo) > 0) {
                torre[j + 1] = torre[j];
                j--;
            } else {
                break;
            }
        }
        torre[j + 1] = pivo;
    }
}

// 3. Selection Sort: Ordena por PRIORIDADE (int - Decrescente: 10 para 1)
void selectionSortPrioridade(Componente torre[], int n) {
    comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int indiceMaior = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (torre[j].prioridade > torre[indiceMaior].prioridade) {
                indiceMaior = j;
            }
        }
        if (indiceMaior != i) {
            Componente temp = torre[i];
            torre[i] = torre[indiceMaior];
            torre[indiceMaior] = temp;
        }
    }
}

// -----------------------------------------------------------------------------
// MEDIÇÃO DE DESEMPENHO E BUSCA BINÁRIA
// -----------------------------------------------------------------------------

// Mede o tempo exato em milissegundos e exibe as comparações
void medirTempo(void (*algoritmo)(Componente[], int), Componente torre[], int n) {
    clock_t inicio = clock();
    algoritmo(torre, n);
    clock_t fim = clock();

    double tempoMs = ((double)(fim - inicio) / CLOCKSYS_PER_SEC) * 1000.0;

    printf("-> Comparacoes realizadas: %lld\n", comparacoes);
    printf("-> Tempo de execucao:     %.4f ms\n", tempoMs);
}

// Busca Binária por NOME
int buscaBinariaPorNome(const Componente torre[], int n, const char *nomeChave) {
    int inicio = 0;
    int fim = n - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        comparacoes++;

        int res = strcasecmp(torre[meio].nome, nomeChave);

        if (res == 0) {
            return meio; // Encontrado
        }
        if (res < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; // Não encontrado
}