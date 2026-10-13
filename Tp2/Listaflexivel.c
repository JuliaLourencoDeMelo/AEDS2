#include <stdio.h>
#include <stdlib.h>

// nó da fila
typedef struct No
{
    int valor;
    struct No *prox;
} No;

// nó fantasma, sem valor útil
typedef struct
{
    No *cabeca;
    No *fim;
} Fila;

// cria a fila já com a célula cabeça alocada

Fila *criarFila()
{
    Fila *f = (Fila *)malloc(sizeof(Fila));

    f->cabeca = (No *)malloc(sizeof(No));
    f->cabeca->prox = NULL;

    f->fim = f->cabeca;

    return f;
}

void enfileirar(Fila *f, int x)
{
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = x;
    novo->prox = NULL;

    // encaixa o novo nó depois do que era o fim, e atualiza o fim
    f->fim->prox = novo;
    f->fim = novo;
}

// desenfileira e devolve o número removido
int desenfileirar(Fila *f)
{
    // fila vazia = célula cabeça não aponta pra nenhum nó real
    if (f->cabeca->prox == NULL)
    {
        return -1;
    }

    No *removido = f->cabeca->prox;
    int valor = removido->valor;

    // a cabeça passa a apontar pro segundo nó que vira o novo primeiro
    f->cabeca->prox = removido->prox;

    // o fim precisa voltar a apontar pra célula cabeça
    if (f->fim == removido)
    {
        f->fim = f->cabeca;
    }

    free(removido);
    return valor;
}

void imprimir(Fila *f)
{
    if (f->cabeca->prox == NULL)
    {
        printf("V\n");
        return;
    }

    No *atual = f->cabeca->prox;
    int primeiro = 1;

    while (atual != NULL)
    {
        if (!primeiro)
        {
            printf(" ");
        }
        printf("%d", atual->valor);
        primeiro = 0;
        atual = atual->prox;
    }

    printf("\n");
}

int pesquisar(Fila *f, int y)
{
    No *atual = f->cabeca->prox;

    while (atual != NULL)
    {
        if (atual->valor == y)
        {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

int main()
{
    Fila *fila = criarFila();

    char comando;

    // ler o próximo caractere que não seja espaço em branco
    while (scanf(" %c", &comando) == 1)
    {
        if (comando == 'E')
        {
            int x;
            scanf("%d", &x);
            enfileirar(fila, x);
        }
        else if (comando == 'D')
        {
            int valor = desenfileirar(fila);
            printf("%d\n", valor);
        }
        else if (comando == 'M')
        {
            imprimir(fila);
        }
        else if (comando == 'P')
        {
            int y;
            scanf("%d", &y);
            printf(pesquisar(fila, y) ? "S\n" : "N\n");
        }
    }

    // libera os nós que sobraram na fila, um por um
    No *atual = fila->cabeca;
    while (atual != NULL)
    {
        No *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(fila);

    return 0;
}