#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    // matriz de char; cada linha desse array guarda um combustível diferente
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

// Recebe a string bruta do CSV e devolve uma struct Data
Data parseData(char *s)
{
    Data d;

    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);

    return d;
}

// recebe um espaço de memória e escreve o resultado
void formatData(Data d, char *buffer)
{
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

// recebe uma linha do CSV e devolve um ponteiro para um Veiculo já montado
Veiculo *parseVeiculo(char *s)
{
    // malloc reserva memória no heap para um Veiculo
    Veiculo *v = (Veiculo *)malloc(sizeof(Veiculo));

    // trabalhamos em uma cópia para não estragar a linha original
    char copia[1000];

    strcpy(copia, s);

    char *campo;

    // guarda a posição atual da leitura dos campos
    char *saveptrCampos;

    campo = strtok_r(copia, ",", &saveptrCampos);
    v->id = atoi(campo);

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

    // Fazemos uma cópia desse campo porque vamos usar outro strtok_r
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
    v->cilindrada = atof(campo);

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
            v.id,
            v.marca,
            v.modelo,
            v.ano,
            v.categoria,
            combustivelStr,
            v.cilindros,
            v.cilindrada,
            v.transmissao,
            v.tracao,
            v.consumoCidade,
            v.consumoEstrada,
            v.co2,
            v.turbo ? "true" : "false",
            dataStr);
}

// lê o arquivo inteiro e devolve um array de Veiculo
Veiculo *lerCsv(char *caminhoArquivo, int *n)
{
    FILE *arquivo = fopen(caminhoArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo: %s\n", caminhoArquivo);

        *n = 0;

        return NULL;
    }

    // aloca de uma vez o espaço máximo que vamos precisar
    Veiculo *veiculos =
        (Veiculo *)malloc(MAX_VEICULOS * sizeof(Veiculo));

    int count = 0;

    char linha[1000];

    // a primeira linha do CSV é o cabeçalho — descarta
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) > 0)
        {
            // parseVeiculo devolve um ponteiro
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

// busca sequencial
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

void countingPorDigito(Veiculo *veiculos, int n, int posicao)
{
    int contagem[10] = {0};

    // vetor auxiliar
    Veiculo *ordenado =
        (Veiculo *)malloc(n * sizeof(Veiculo));

    for (int i = 0; i < n; i++)
    {
        // conta os algarismos
        int digito = (veiculos[i].ano / posicao) % 10;

        contagem[digito]++;
    }

    for (int i = 1; i < 10; i++)
    {
        contagem[i] =
            contagem[i] + contagem[i - 1];
    }

    // coloca no vetor ordenado

    for (int i = n - 1; i >= 0; i--)
    {
        int digito =
            (veiculos[i].ano / posicao) % 10;

        ordenado[contagem[digito] - 1] =
            veiculos[i];

        contagem[digito]--;
    }

    for (int i = 0; i < n; i++)
    {
        veiculos[i] = ordenado[i];
    }

    free(ordenado);
}

// Radixsort

void radixSort(Veiculo *veiculos, int n)
{
    if (n <= 0)
    {
        return;
    }

    // descobre o maior ano
    int maior = veiculos[0].ano;

    for (int i = 1; i < n; i++)
    {
        if (veiculos[i].ano > maior)
        {
            maior = veiculos[i].ano;
        }
    }

    for (int posicao = 1;
         maior / posicao > 0;
         posicao *= 10)
    {
        countingPorDigito(
            veiculos,
            n,
            posicao);
    }
}

int main()
{
    int n;

    // quantidade de veículos carregados
    Veiculo *veiculos =
        lerCsv("/tmp/veiculos.csv", &n);

    // Array separado somente com os veículos
    // encontrados a partir dos IDs digitados.
    Veiculo *selecionados =
        (Veiculo *)malloc(MAX_VEICULOS * sizeof(Veiculo));

    int totalSelecionados = 0;

    int id;

    // Lê os IDs da entrada
    while (scanf("%d", &id) == 1)
    {
        if (id == -1)
        {
            break;
        }

        Veiculo *encontrado =
            buscaSequencial(veiculos, n, id);

        if (encontrado != NULL)
        {
            selecionados[totalSelecionados] =
                *encontrado;

            totalSelecionados++;
        }
    }

    // Ordena os veículos selecionados
    // pelo atributo ANO usando Radixsort.
    radixSort(
        selecionados,
        totalSelecionados);

    // Imprime os veículos ordenados
    for (int i = 0; i < totalSelecionados; i++)
    {
        char buffer[500];

        formatVeiculo(
            selecionados[i],
            buffer);

        printf("%s\n", buffer);
    }

    // Libera a memória
    free(veiculos);
    free(selecionados);

    return 0;
}