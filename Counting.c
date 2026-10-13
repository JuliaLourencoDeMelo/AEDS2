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

// Recebe a string bruta do CSV, no formato certo e devolve uma struct Data
Data parseData(char *s)
{
    Data d;

    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);

    return d;
}

// recebe um espaço de memória (buffer) e escreve o resultado
void formatData(Data d, char *buffer)
{
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

// recebe uma linha do CSV e devolve um ponteiro para um Veiculo já montado
Veiculo *parseVeiculo(char *s)
{
    // malloc reserva memória no heap para um Veiculo
    Veiculo *v = (Veiculo *)malloc(sizeof(Veiculo));

    // strtok quebra a string colocando '\0' onde encontra o separador
    // então trabalhamos numa cópia para não estragar a linha original
    char copia[1000];

    strcpy(copia, s);

    char *campo;

    // usamos strtok_r para guardar separadamente a posição da leitura
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

// counting sort

void countingSort(Veiculo *veiculos, int n)
{
    int contagem[17] = {0};

    Veiculo *ordenado = (Veiculo *)malloc(n * sizeof(Veiculo));

    // Conta os cilindros
    for (int i = 0; i < n; i++)
    {
        contagem[veiculos[i].cilindros]++;
    }

    // Soma acumulada
    for (int i = 1; i < 17; i++)
    {
        contagem[i] = contagem[i] + contagem[i - 1];
    }

    // Coloca na posição correta
    for (int i = n - 1; i >= 0; i--)
    {
        int cilindros = veiculos[i].cilindros;

        ordenado[contagem[cilindros] - 1] = veiculos[i];

        contagem[cilindros]--;
    }

    // Copia de volta
    for (int i = 0; i < n; i++)
    {
        veiculos[i] = ordenado[i];
    }

    free(ordenado);
}

int main()
{
    int n;

    // Lê todos os veículos do arquivo CSV
    Veiculo *veiculos = lerCsv("/tmp/veiculos.csv", &n);

    // Vetor que vai guardar SOMENTE os veículos
    // informados na entrada
    Veiculo selecionados[MAX_VEICULOS];

    int quantidade = 0;
    int id;

    // Lê os IDs até encontrar -1
    while (scanf("%d", &id) == 1)
    {
        if (id == -1)
        {
            break;
        }

        // Procura o veículo pelo ID
        for (int i = 0; i < n; i++)
        {
            if (veiculos[i].id == id)
            {
                // Coloca o veículo encontrado no vetor
                // que será ordenado
                selecionados[quantidade] = veiculos[i];

                quantidade++;

                break;
            }
        }
    }

    countingSort(selecionados, quantidade);

    // Mostra os veículos ordenados
    for (int i = 0; i < quantidade; i++)
    {
        char buffer[500];

        formatVeiculo(selecionados[i], buffer);

        printf("%s\n", buffer);
    }

    // Libera a memória
    free(veiculos);
    return 0;
}