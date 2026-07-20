#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

// Definicao da struct Item para agregar os dados de cada objeto
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Protótipos das funções
void inserirItem(Item mochila[], int *total);
void removerItem(Item mochila[], int *total);
void listarItens(const Item mochila[], int total);
void buscarItem(const Item mochila[], int total);
void limparBuffer(void);

int main() {
    Item mochila[MAX_ITENS];
    int totalItens = 0;
    int opcao;

    do {
        printf("\n========================================\n");
        printf("         MOCHILA DE LOOT INICIAL        \n");
        printf("========================================\n");
        printf("Capacidade atual: %d/%d itens\n", totalItens, MAX_ITENS);
        printf("----------------------------------------\n");
        printf("1. Cadastrar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens da mochila\n");
        printf("4. Buscar item por nome\n");
        printf("0. Sair do jogo\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;
            case 2:
                removerItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                buscarItem(mochila, totalItens);
                break;
            case 0:
                printf("\nSaindo da mochila... Prepare-se para a partida!\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

// Auxiliar para limpar o buffer do teclado ao usar scanf/fgets
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para cadastrar até 10 itens na mochila
void inserirItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\n[AVISO] Mochila cheia! Voce nao pode carregar mais itens (%d/%d).\n", *total, MAX_ITENS);
        return;
    }

    Item novo;

    printf("\n--- CADASTRAR NOVO ITEM ---\n");
    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove o \n final

    printf("Tipo do item (ex: arma, municao, cura): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    while (scanf("%d", &novo.quantidade) != 1 || novo.quantidade <= 0) {
        printf("Quantidade invalida! Digite um numero positivo: ");
        limparBuffer();
    }
    limparBuffer();

    mochila[*total] = novo;
    (*total)++;

    printf("\n[SUCESSO] Item \"%s\" adicionado com sucesso!\n", novo.nome);
}

// Função para remover um item informando seu nome
void removerItem(Item mochila[], int *total) {
    if (*total == 0) {
        printf("\n[AVISO] A mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int indiceEncontrado = -1;

    // Busca sequencial para encontrar a posição do item
    for (int i = 0; i < *total; i++) {
        if (strcasecmp(mochila[i].nome, nomeBusca) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("\n[ERRO] Item \"%s\" nao foi encontrado na mochila.\n", nomeBusca);
        return;
    }

    // Reorganiza o vetor deslocando os elementos para preencher a lacuna
    for (int i = indiceEncontrado; i < *total - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*total)--;
    printf("\n[SUCESSO] Item \"%s\" removido da mochila!\n", nomeBusca);
}

// Função para listar todos os itens atuais e seus dados
void listarItens(const Item mochila[], int total) {
    printf("\n=== ITENS NA MOCHILA (%d/%d) ===\n", total, MAX_ITENS);

    if (total == 0) {
        printf("A mochila esta totalmente vazia.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("[%d] Nome: %-18s | Tipo: %-10s | Qtd: %d\n", 
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função de busca sequencial por nome
void buscarItem(const Item mochila[], int total) {
    if (total == 0) {
        printf("\n[AVISO] Mochila vazia. Nenhum item cadastrado.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- BUSCA SEQUENCIAL DE ITEM ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Busca sequencial item por item
    for (int i = 0; i < total; i++) {
        if (strcasecmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[ENCONTRADO] Dados do item:\n");
            printf(" -> Posição: Slot %d\n", i + 1);
            printf(" -> Nome:    %s\n", mochila[i].nome);
            printf(" -> Tipo:    %s\n", mochila[i].tipo);
            printf(" -> Qtd:     %d\n", mochila[i].quantidade);
            return;
        }
    }

    printf("\n[NAO ENCONTRADO] O item \"%s\" nao esta na sua mochila.\n", nomeBusca);
}