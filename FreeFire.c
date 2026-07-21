#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // Para strcasecmp

#define MAX_ITENS 10

// -----------------------------------------------------------------------------
// 1. DEFINIÇÃO DAS ESTRUTURAS DE DADOS
// -----------------------------------------------------------------------------

// Struct base do Item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Struct Nó para a Lista Encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// Variável global para contar comparações nas buscas (Análise Empírica)
int comparacoesBusca = 0;

// -----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------------------------------------
void limparBuffer(void);

// Operações no Vetor
void inserirItemVetor(Item vetor[], int *total);
void removerItemVetor(Item vetor[], int *total);
void listarItensVetor(const Item vetor[], int total);
int buscarSequencialVetor(const Item vetor[], int total, const char *nome);
void ordenarVetor(Item vetor[], int total);
int buscarBinariaVetor(const Item vetor[], int total, const char *nome);

// Operações na Lista Encadeada
void inserirItemLista(No **cabeca);
void removerItemLista(No **cabeca);
void listarItensLista(No *cabeca);
No* buscarSequencialLista(No *cabeca, const char *nome);
void liberarLista(No **cabeca);

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL (MENU INTERATIVO)
// -----------------------------------------------------------------------------
int main() {
    Item vetorMochila[MAX_ITENS];
    int totalVetor = 0;
    int vetorOrdenado = 0; // Flag para controlar se o vetor está ordenado

    No *listaMochila = NULL; // Ponteiro para a cabeça da lista encadeada

    int opcaoEstrutura, opcaoOperacao;

    do {
        printf("\n==================================================\n");
        printf("       SISTEMA DE MOCHILA - SELECAO DE MODO       \n");
        printf("==================================================\n");
        printf("1. Operar Mochila com VETOR (Lista Sequencial)\n");
        printf("2. Operar Mochila com LISTA ENCADEADA (Dinamica)\n");
        printf("3. Comparar Desempenho de Busca (Vetor vs Lista)\n");
        printf("0. Sair do Jogo\n");
        printf("--------------------------------------------------\n");
        printf("Escolha a estrutura: ");
        
        if (scanf("%d", &opcaoEstrutura) != 1) {
            opcaoEstrutura = -1;
        }
        limparBuffer();

        switch (opcaoEstrutura) {
            case 1: // MODO VETOR
                do {
                    printf("\n--- MOCHILA COM VETOR (%d/%d) ---\n", totalVetor, MAX_ITENS);
                    printf("1. Inserir Item\n");
                    printf("2. Remover Item\n");
                    printf("3. Listar Itens\n");
                    printf("4. Buscar Item (Sequencial)\n");
                    printf("5. Ordenar Vetor (Bubble Sort)\n");
                    printf("6. Buscar Item (Binaria - Requer Ordenacao)\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("Opcao: ");
                    scanf("%d", &opcaoOperacao);
                    limparBuffer();

                    if (opcaoOperacao == 1) {
                        inserirItemVetor(vetorMochila, &totalVetor);
                        vetorOrdenado = 0; // Inserção desordena o vetor
                        listarItensVetor(vetorMochila, totalVetor);
                    } else if (opcaoOperacao == 2) {
                        removerItemVetor(vetorMochila, &totalVetor);
                        listarItensVetor(vetorMochila, totalVetor);
                    } else if (opcaoOperacao == 3) {
                        listarItensVetor(vetorMochila, totalVetor);
                    } else if (opcaoOperacao == 4) {
                        char nome[30];
                        printf("Nome do item para Busca Sequencial: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';
                        
                        comparacoesBusca = 0;
                        int idx = buscarSequencialVetor(vetorMochila, totalVetor, nome);
                        if (idx != -1) {
                            printf("[ENCONTRADO] Item '%s' na posicao %d do Vetor.\n", vetorMochila[idx].nome, idx + 1);
                        } else {
                            printf("[NAO ENCONTRADO] Item nao esta no Vetor.\n");
                        }
                        printf("-> Comparacoes realizadas: %d\n", comparacoesBusca);
                    } else if (opcaoOperacao == 5) {
                        ordenarVetor(vetorMochila, totalVetor);
                        vetorOrdenado = 1;
                        listarItensVetor(vetorMochila, totalVetor);
                    } else if (opcaoOperacao == 6) {
                        if (!vetorOrdenado) {
                            printf("\n[AVISO] Ordene o vetor (Opcao 5) antes de executar a Busca Binaria!\n");
                        } else {
                            char nome[30];
                            printf("Nome do item para Busca Binaria: ");
                            fgets(nome, sizeof(nome), stdin);
                            nome[strcspn(nome, "\n")] = '\0';

                            comparacoesBusca = 0;
                            int idx = buscarBinariaVetor(vetorMochila, totalVetor, nome);
                            if (idx != -1) {
                                printf("[ENCONTRADO] Item '%s' no slot %d.\n", vetorMochila[idx].nome, idx + 1);
                            } else {
                                printf("[NAO ENCONTRADO] Item nao localizado via Busca Binaria.\n");
                            }
                            printf("-> Comparacoes realizadas: %d\n", comparacoesBusca);
                        }
                    }
                } while (opcaoOperacao != 0);
                break;

            case 2: // MODO LISTA ENCADEADA
                do {
                    printf("\n--- MOCHILA COM LISTA ENCADEADA ---\n");
                    printf("1. Inserir Item\n");
                    printf("2. Remover Item\n");
                    printf("3. Listar Itens\n");
                    printf("4. Buscar Item (Sequencial)\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("Opcao: ");
                    scanf("%d", &opcaoOperacao);
                    limparBuffer();

                    if (opcaoOperacao == 1) {
                        inserirItemLista(&listaMochila);
                        listarItensLista(listaMochila);
                    } else if (opcaoOperacao == 2) {
                        removerItemLista(&listaMochila);
                        listarItensLista(listaMochila);
                    } else if (opcaoOperacao == 3) {
                        listarItensLista(listaMochila);
                    } else if (opcaoOperacao == 4) {
                        char nome[30];
                        printf("Nome do item para Busca na Lista: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';

                        comparacoesBusca = 0;
                        No *encontrado = buscarSequencialLista(listaMochila, nome);
                        if (encontrado != NULL) {
                            printf("[ENCONTRADO] Item: %s | Tipo: %s | Qtd: %d\n", 
                                   encontrado->dados.nome, encontrado->dados.tipo, encontrado->dados.quantidade);
                        } else {
                            printf("[NAO ENCONTRADO] Item nao esta na Lista.\n");
                        }
                        printf("-> Comparacoes realizadas: %d\n", comparacoesBusca);
                    }
                } while (opcaoOperacao != 0);
                break;

            case 3: // COMPARATIVO DE DESEMPENHO
                printf("\n=== TESTE COMPARATIVO DE DESEMPENHO ===\n");
                if (totalVetor == 0) {
                    printf("Cadastre alguns itens no Vetor para testar a comparacao!\n");
                } else {
                    char nome[30];
                    printf("Digite o nome do item para testar em ambas as estruturas: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    // Teste 1: Sequencial Vetor
                    comparacoesBusca = 0;
                    buscarSequencialVetor(vetorMochila, totalVetor, nome);
                    int compSeqVetor = comparacoesBusca;

                    // Teste 2: Sequencial Lista
                    comparacoesBusca = 0;
                    buscarSequencialLista(listaMochila, nome);
                    int compSeqLista = comparacoesBusca;

                    printf("\n--- RESULTADO DAS COMPARATIVEIS ---\n");
                    printf("1. Busca Sequencial no Vetor : %d comparacoes\n", compSeqVetor);
                    printf("2. Busca Sequencial na Lista : %d comparacoes\n", compSeqLista);

                    if (vetorOrdenado) {
                        comparacoesBusca = 0;
                        buscarBinariaVetor(vetorMochila, totalVetor, nome);
                        printf("3. Busca Binaria no Vetor     : %d comparacoes (O(log n))\n", comparacoesBusca);
                    } else {
                        printf("3. Busca Binaria no Vetor     : [Nao executada - Ordene o vetor primeiro]\n");
                    }
                }
                break;

            case 0:
                liberarLista(&listaMochila);
                printf("\nSaindo e liberando memoria dinamica. Boa partida!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (opcaoEstrutura != 0);

    return 0;
}

// -----------------------------------------------------------------------------
// FUNÇÕES AUXILIARES
// -----------------------------------------------------------------------------
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// -----------------------------------------------------------------------------
// OPERAÇÕES DO VETOR (LISTA SEQUENCIAL)
// -----------------------------------------------------------------------------

void inserirItemVetor(Item vetor[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\n[ERRO] Vetor cheio! Capacidade maxima de %d atingida.\n", MAX_ITENS);
        return;
    }
    Item novo;
    printf("Nome: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    limparBuffer();

    vetor[*total] = novo;
    (*total)++;
    printf("[SUCESSO] Item adicionado ao Vetor!\n");
}

void removerItemVetor(Item vetor[], int *total) {
    if (*total == 0) {
        printf("\n[AVISO] Vetor vazio.\n");
        return;
    }
    char nome[30];
    printf("Nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int idx = -1;
    for (int i = 0; i < *total; i++) {
        if (strcasecmp(vetor[i].nome, nome) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("[ERRO] Item nao encontrado.\n");
        return;
    }

    // Desloca elementos para cobrir a lacuna (Reorganização contígua)
    for (int i = idx; i < *total - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    (*total)--;
    printf("[SUCESSO] Item removido do Vetor.\n");
}

void listarItensVetor(const Item vetor[], int total) {
    printf("\n--- ITENS NO VETOR (%d/%d) ---\n", total, MAX_ITENS);
    if (total == 0) {
        printf("Vetor vazio.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("[%d] %-15s | Tipo: %-10s | Qtd: %d\n", i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

int buscarSequencialVetor(const Item vetor[], int total, const char *nome) {
    for (int i = 0; i < total; i++) {
        comparacoesBusca++; // Incrementa contador de operações
        if (strcasecmp(vetor[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Algoritmo de Ordenação: Bubble Sort
void ordenarVetor(Item vetor[], int total) {
    if (total < 2) {
        printf("\n[AVISO] Vetor precisa ter pelo menos 2 itens para ordenar.\n");
        return;
    }

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (strcasecmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                Item temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
    printf("\n[SUCESSO] Vetor ordenado por NOME com sucesso!\n");
}

// Busca Binária no Vetor (Divisão e Conquista)
int buscarBinariaVetor(const Item vetor[], int total, const char *nome) {
    int inicio = 0;
    int fim = total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        comparacoesBusca++;

        int res = strcasecmp(vetor[meio].nome, nome);

        if (res == 0) {
            return meio; // Encontrado
        }
        if (res < 0) {
            inicio = meio + 1; // Busca na metade direita
        } else {
            fim = meio - 1;    // Busca na metade esquerda
        }
    }
    return -1; // Não encontrado
}

// -----------------------------------------------------------------------------
// OPERAÇÕES DA LISTA ENCADEADA (ALOCAÇÃO DINÂMICA)
// -----------------------------------------------------------------------------

void inserirItemLista(No **cabeca) {
    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("\n[ERRO] Falha ao alocar memoria na RAM!\n");
        return;
    }

    printf("Nome: ");
    fgets(novoNo->dados.nome, sizeof(novoNo->dados.nome), stdin);
    novoNo->dados.nome[strcspn(novoNo->dados.nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(novoNo->dados.tipo, sizeof(novoNo->dados.tipo), stdin);
    novoNo->dados.tipo[strcspn(novoNo->dados.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoNo->dados.quantidade);
    limparBuffer();

    // Inserção no início da lista encadeada (Operação O(1))
    novoNo->proximo = *cabeca;
    *cabeca = novoNo;

    printf("[SUCESSO] Item alocado dinamicamente na Lista Encadeada!\n");
}

void removerItemLista(No **cabeca) {
    if (*cabeca == NULL) {
        printf("\n[AVISO] Lista encadeada vazia.\n");
        return;
    }

    char nome[30];
    printf("Nome do item a remover da lista: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = *cabeca;
    No *anterior = NULL;

    while (atual != NULL && strcasecmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("[ERRO] Item nao encontrado na lista.\n");
        return;
    }

    // Remoção do primeiro nó
    if (anterior == NULL) {
        *cabeca = atual->proximo;
    } else {
        // Ignora o nó atual religando os ponteiros
        anterior->proximo = atual->proximo;
    }

    free(atual); // Libera a memória alocada dinamicamente
    printf("[SUCESSO] No removido e memoria liberada (free)!\n");
}

void listarItensLista(No *cabeca) {
    printf("\n--- ITENS NA LISTA ENCADEADA ---\n");
    if (cabeca == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    No *atual = cabeca;
    int slot = 1;
    while (atual != NULL) {
        printf("[%d] %-15s | Tipo: %-10s | Qtd: %d\n", 
               slot++, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

No* buscarSequencialLista(No *cabeca, const char *nome) {
    No *atual = cabeca;
    while (atual != NULL) {
        comparacoesBusca++; // Incrementa contador de comparações
        if (strcasecmp(atual->dados.nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Desaloca toda a memória da lista encadeada para evitar vazamentos (Memory Leaks)
void liberarLista(No **cabeca) {
    No *atual = *cabeca;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *cabeca = NULL;
}