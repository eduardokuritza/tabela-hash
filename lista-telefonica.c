#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO_TABELA 101

// Estrutura do contato
typedef struct Contato
{
    char nome[50];
    char telefone[20];
    struct Contato *proximo;
} Contato;

// Tabela hash
Contato *tabelaHash[TAMANHO_TABELA];

// Função hash simples baseada na soma dos caracteres do nome
unsigned int funcaoHash(char *nome)
{
    unsigned int soma = 0;
    while (*nome)
    {
        soma += *nome;
        nome++;
    }
    return soma % TAMANHO_TABELA;
}

// Inicializa a tabela hash
void inicializarTabela()
{
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        tabelaHash[i] = NULL;
    }
}

// Adiciona um novo contato à tabela hash
void adicionarContato()
{
    char nome[50];
    char telefone[20];

    printf("Nome: ");
    scanf(" %[^\n]", nome);
    printf("Telefone: ");
    scanf(" %[^\n]", telefone);

    unsigned int indice = funcaoHash(nome);

    // Verifica se o contato já existe
    Contato *atual = tabelaHash[indice];
    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            printf("Contato ja existe. Deseja atualizar o numero? (s/n): ");
            char opcao;
            scanf(" %c", &opcao);
            if (opcao == 's' || opcao == 'S')
            {
                strcpy(atual->telefone, telefone);
                printf("Contato atualizado com sucesso.\n");
                return;
            }
            else
            {
                printf("Operacao cancelada.\n");
                return;
            }
        }
        atual = atual->proximo;
    }

    // Cria um novo contato
    Contato *novoContato = (Contato *)malloc(sizeof(Contato));
    strcpy(novoContato->nome, nome);
    strcpy(novoContato->telefone, telefone);
    novoContato->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoContato;

    printf("Contato adicionado com sucesso.\n");
}

// Busca um contato pelo nome
void buscarContato()
{
    char nome[50];
    printf("Nome: ");
    scanf(" %[^\n]", nome);

    clock_t inicio, fim;
    double tempoGasto;

    inicio = clock();

    unsigned int indice = funcaoHash(nome);
    Contato *atual = tabelaHash[indice];

    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            fim = clock();
            tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
            printf("Telefone de %s: %s (tempo de busca: %.2f ms)\n", atual->nome, atual->telefone, tempoGasto);
            return;
        }
        atual = atual->proximo;
    }

    fim = clock();
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
    printf("Contato nao encontrado. (tempo de busca: %.2f ms)\n", tempoGasto);
}

// Remove um contato da tabela hash
void removerContato()
{
    char nome[50];
    printf("Nome: ");
    scanf(" %[^\n]", nome);

    unsigned int indice = funcaoHash(nome);
    Contato *atual = tabelaHash[indice];
    Contato *anterior = NULL;

    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            if (anterior == NULL)
            {
                tabelaHash[indice] = atual->proximo;
            }
            else
            {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Contato removido com sucesso.\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Contato nao encontrado.\n");
}

// Exibe todos os contatos da tabela hash
void exibirContatos()
{
    printf("\n--- Lista de Contatos ---\n");
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        Contato *atual = tabelaHash[i];
        while (atual != NULL)
        {
            printf("Nome: %s | Telefone: %s\n", atual->nome, atual->telefone);
            atual = atual->proximo;
        }
    }
}

// Calcula o tempo médio de inserção de N contatos
void medirTempoInsercao(int n)
{
    clock_t inicio, fim;
    double tempoTotal = 0;
    char nome[50];
    char telefone[20];

    for (int i = 0; i < n; i++)
    {
        sprintf(nome, "Contato%d", i);
        sprintf(telefone, "Telefone%d", i);

        inicio = clock();

        unsigned int indice = funcaoHash(nome);
        Contato *novoContato = (Contato *)malloc(sizeof(Contato));
        strcpy(novoContato->nome, nome);
        strcpy(novoContato->telefone, telefone);
        novoContato->proximo = tabelaHash[indice];
        tabelaHash[indice] = novoContato;

        fim = clock();
        tempoTotal += ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000; // em milissegundos
    }

    printf("Tempo medio de insercao: %.2f ms\n", tempoTotal / n);
}

// Limpa a memória alocada
void limparTabela()
{
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        Contato *atual = tabelaHash[i];
        while (atual != NULL)
        {
            Contato *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

int main()
{
    int opcao;
    inicializarTabela();

    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("5 - Medir tempo medio de insercao (teste com N contatos)\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");

        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            adicionarContato();
            break;
        case 2:
            buscarContato();
            break;
        case 3:
            removerContato();
            break;
        case 4:
            exibirContatos();
            break;
        case 5:
        {
            int n;
            printf("Digite o numero de contatos para o teste: ");
            scanf("%d", &n);
            medirTempoInsercao(n);
            break;
        }
        case 0:
            printf("Saindo...\n");
            limparTabela();
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
