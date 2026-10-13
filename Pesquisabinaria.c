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

Veiculo *parseVeiculo(char *s)
{
    // malloc reserva memória no heap para um Veiculo
    Veiculo *v = (Veiculo *)malloc(sizeof(Veiculo));

    // trabalhamos numa cópia para não estragar a linha original.
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
    // strtok_r resolve isso guardando o progresso da leitura numa variável própria
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
            // parseVeiculo devolve um ponteiro; copia o conteúdo
            // dele para dentro do array e depois libera o ponteiro
            // temporário, já que o valor já foi copiado
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

// ordenação por Seleção (Selection Sort)

void ordenacaoSelecao(Veiculo *veiculos, int n)
{
    for (int i = 0; i < n - 1; i++)
    {

        int indiceMenor = i;

        for (int j = i + 1; j < n; j++)
        {

            if (strcasecmp(veiculos[j].modelo, veiculos[indiceMenor].modelo) < 0)
            {
                indiceMenor = j;
            }
        }

        if (indiceMenor != i)
        {
            Veiculo temp = veiculos[i];
            veiculos[i] = veiculos[indiceMenor];
            veiculos[indiceMenor] = temp;
        }
    }
}

// Pesquisa binaria

int pesquisaBinaria(Veiculo *veiculos, int n, char *modelo)
{
    // esquerda começa na primeira posição do vetor
    int esquerda = 0;

    // direita começa na última posição do vetor
    int direita = n - 1;

    while (esquerda <= direita)
    {
        int meio = (esquerda + direita) / 2;

        int comparacao = strcasecmp(modelo, veiculos[meio].modelo);
        if (comparacao == 0)
        {
            return 1; // encontrou
        }

        // se o modelo procurado vem depois do modelo do meio, procura na metade da direita
        if (comparacao > 0)
        {
            esquerda = meio + 1;
        }
        else
        {
            direita = meio - 1;
        }
    }

    return 0;
}

int main()
{
    int n; // quantidade de veículos carregados

    Veiculo *veiculos = lerCsv("/tmp/veiculos.csv", &n);

    // array separado só com os veículos que forem encontrados
    // a partir dos ids digitados na entrada
    Veiculo *selecionados =
        (Veiculo *)malloc(MAX_VEICULOS * sizeof(Veiculo));

    int totalSelecionados = 0;

    int id;

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
            selecionados[totalSelecionados] = *encontrado;
            totalSelecionados++;
        }
    }

    ordenacaoSelecao(selecionados, totalSelecionados);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char modelo[MAX_STR];

    while (fgets(modelo, sizeof(modelo), stdin) != NULL)
    {
        modelo[strcmp(modelo, "\n")] = '\0';

        if (strcmp(modelo, "FIM") == 0)
        {
            break;
        }

        // Faz a pesquisa binária pelo modelo
        int encontrado =
            pesquisaBinaria(
                selecionados,
                totalSelecionados,
                modelo);

        if (encontrado == 1)
        {
            printf("SIM\n");
        }
        else
        {
            printf("NAO\n");
        }
    }

    // libera a memória alocada
    free(veiculos);
    free(selecionados);

    return 0;
}