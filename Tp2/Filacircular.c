#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

// tamanho máximo de veículos que conseguimos guardar de uma vez
#define MAX_VEICULOS 1000

// tamanho máximo para os textos
#define MAX_STR 100

// tipos de combustível um único veículo pode ter
#define MAX_COMBUSTIVEIS 5

typedef struct
{
    int ano;
    int mes;
    int dia;
} Data;

typedef struct
{
    int id;
    char marca[MAX_STR];
    char modelo[MAX_STR];
    int ano;
    char categoria[MAX_STR];

    // matriz de char; cada linha desse array guarda um combustível diferente (IA)
    char combustivel[MAX_COMBUSTIVEIS][MAX_STR];
    int numCombustiveis;

    int cilindros;
    double cilindrada;
    char transmissao[MAX_STR];
    char tracao[MAX_STR];
    double consumoCidade;
    double consumoEstrada;
    double co2;
    bool turbo;

    Data dataRegistro;
} Veiculo;

// Recebe a string bruta do CSV, no formato certo e devolve uma struct Data
Data parseData(char *s)
{
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

// recebe um espaço de memória e escreve o resultado, usando sprintf
void formatData(Data d, char *buffer)
{
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

// recebe uma linha do CSV e devolve um ponteiro para um Veiculo já montado
// ( uso um ponteiro porque veiculo é uma struct evitando copiar tudo

Veiculo *parseVeiculo(char *s)
{
    // malloc reserva memória no heap para um Veiculo
    Veiculo *v = (Veiculo *)malloc(sizeof(Veiculo));

    char copia[1000];
    strcpy(copia, s);

    char *campo;

    // uso strtok_r em vez de strtok.
    // O strtok "normal" guarda o progresso da leitura numa única
    // variável global — então, se chamarmos strtok de novo (com outro
    // separador) ANTES de terminar de usar o primeiro, ele perde o lugar
    // onde estava. Isso vai acontecer aqui, porque precisamos de um
    // strtok para separar por "," (campos) e de outro para separar
    // por ";" (combustíveis), ao mesmo tempo.
    // strtok_r resolve isso guardando o progresso numa variável própria
    // (o "saveptr"), que passamos explicitamente em cada chamada ( CHAT GPT )
    char *saveptrCampos;

    campo = strtok_r(copia, ",", &saveptrCampos);
    v->id = atoi(campo); // atoi = converte string para int

    campo = strtok_r(NULL, ",", &saveptrCampos);
    strcpy(v->marca, campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    strcpy(v->modelo, campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->ano = atoi(campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    strcpy(v->categoria, campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->numCombustiveis = 0;

    char combustivelCopia[MAX_STR];
    strcpy(combustivelCopia, campo);

    char *saveptrCombustivel;
    char *comb = strtok_r(combustivelCopia, ";", &saveptrCombustivel);
    while (comb != NULL)
    {
        strcpy(v->combustivel[v->numCombustiveis], comb);
        v->numCombustiveis++;
        comb = strtok_r(NULL, ";", &saveptrCombustivel);
    }

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->cilindros = atoi(campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->cilindrada = atof(campo); // atof = converte string para double

    campo = strtok_r(NULL, ",", &saveptrCampos);
    strcpy(v->transmissao, campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    strcpy(v->tracao, campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->consumoCidade = atof(campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->consumoEstrada = atof(campo);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->co2 = atof(campo);

    // compara a string com strcmp (retorna 0 quando são iguais).
    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->turbo = (strcmp(campo, "true") == 0);

    campo = strtok_r(NULL, ",", &saveptrCampos);
    v->dataRegistro = parseData(campo);

    return v;
}

void formatVeiculo(Veiculo v, char *buffer)
{

    char combustivelStr[MAX_STR] = "[";
    for (int i = 0; i < v.numCombustiveis; i++)
    {
        strcat(combustivelStr, v.combustivel[i]);
        if (i < v.numCombustiveis - 1)
        {
            strcat(combustivelStr, ",");
        }
    }
    strcat(combustivelStr, "]");

    char dataStr[20];
    formatData(v.dataRegistro, dataStr);

    sprintf(buffer,
            "[%d ## %s ## %s ## %d ## %s ## %s ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %s ## %s]",
            v.id, v.marca, v.modelo, v.ano, v.categoria, combustivelStr,
            v.cilindros, v.cilindrada, v.transmissao, v.tracao,
            v.consumoCidade, v.consumoEstrada, v.co2,
            v.turbo ? "true" : "false", dataStr);
}

// lê o arquivo inteiro e devolve um array de Veiculo (por ponteiro).

Veiculo *lerCsv(char *caminhoArquivo, int *n)
{
    FILE *arquivo = fopen(caminhoArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo: %s\n", caminhoArquivo);
        *n = 0;
        return NULL;
    }

    // aloca de uma vez o espaço máximo
    Veiculo *veiculos = (Veiculo *)malloc(MAX_VEICULOS * sizeof(Veiculo));
    int count = 0;

    char linha[1000];

    // a primeira linha do CSV é o cabeçalho — descarta
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) > 0)
        {
            // parseVeiculo devolve um ponteiro; copia o conteúdo
            // dele para dentro do array e depois libera o ponteiro
            // temporário, já que o valor já foi copiado.
            Veiculo *v = parseVeiculo(linha);
            veiculos[count] = *v;
            free(v);
            count++;
        }
    }

    fclose(arquivo);
    *n = count;
    return veiculos;
}

// busca sequencial: percorre o array do início ao fim, comparando o id de cada veículo

Veiculo *buscaSequencial(Veiculo *veiculos, int n, int id)
{
    for (int i = 0; i < n; i++)
    {
        if (veiculos[i].id == id)
        {
            return &veiculos[i];
        }
    }
    return NULL;
}

// fila circular com array de tamanho 5

#define CAPACIDADE 5

typedef struct
{
    Veiculo dados[CAPACIDADE];
    int inicio; // posição do primeiro elemento
    int total;  // quantos elementos tem na fila agora
} FilaCircular;

void inicializarFila(FilaCircular *fila)
{
    fila->inicio = 0;
    fila->total = 0;
}

int filaCheia(FilaCircular *fila)
{
    return fila->total == CAPACIDADE;
}

int filaVazia(FilaCircular *fila)
{
    return fila->total == 0;
}

int desenfileirar(FilaCircular *fila, Veiculo *removido)
{
    if (filaVazia(fila))
    {
        return 0;
    }

    *removido = fila->dados[fila->inicio];

    // anda o inicio, dando a volta no array com %
    fila->inicio = (fila->inicio + 1) % CAPACIDADE;
    fila->total--;

    return 1;
}

void enfileirar(FilaCircular *fila, Veiculo v)
{
    if (filaCheia(fila))
    {
        Veiculo removido;
        desenfileirar(fila, &removido);
        printf("(R)%s %s\n", removido.marca, removido.modelo);
    }

    // calcula onde entra o novo elemento (inicio + quantos já tem),
    // com % pra dar a volta no array quando passar do tamanho dele
    int posicaoInsercao = (fila->inicio + fila->total) % CAPACIDADE;
    fila->dados[posicaoInsercao] = v;
    fila->total++;
}

int main()
{
    int n; // quantidade de veículos carregados

    Veiculo *veiculos = lerCsv("/tmp/veiculos.csv", &n);

    FilaCircular fila;
    inicializarFila(&fila);

    int id;
    while (scanf("%d", &id) == 1)
    {
        if (id == -1)
        {
            break;
        }

        Veiculo *encontrado = buscaSequencial(veiculos, n, id);
        if (encontrado != NULL)
        {
            enfileirar(&fila, *encontrado);
        }
    }

    int m;
    scanf("%d", &m);

    char comando[10];
    for (int i = 0; i < m; i++)
    {
        scanf("%s", comando);

        if (strcmp(comando, "I") == 0)
        {
            int idComando;
            scanf("%d", &idComando);

            Veiculo *encontrado = buscaSequencial(veiculos, n, idComando);
            if (encontrado != NULL)
            {
                enfileirar(&fila, *encontrado);
            }
        }
        else if (strcmp(comando, "R") == 0)
        {
            Veiculo removido;
            if (desenfileirar(&fila, &removido))
            {
                printf("(R)%s %s\n", removido.marca, removido.modelo);
            }
        }
    }

    // no final, mostra os registros que sobraram na fila, do primeiro ao último
    for (int i = 0; i < fila.total; i++)
    {

        int posicao = (fila.inicio + i) % CAPACIDADE;

        char buffer[500];
        formatVeiculo(fila.dados[posicao], buffer);
        printf("%s\n", buffer);
    }

    free(veiculos);

    return 0;
}