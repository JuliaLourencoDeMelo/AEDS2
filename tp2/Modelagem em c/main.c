/*
 * Exe 2 - MODELAGEM EM C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// tamanhos fixos 

#define TAM_MARCA        50
#define TAM_MODELO       100
#define TAM_CATEGORIA    50
#define TAM_TEXTO        30    
#define MAX_COMBUSTIVEIS 5     
#define TAM_LINHA        512   
#define TAM_BUFFER       1024  


typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

typedef struct {
    int id;
    char marca[TAM_MARCA];
    char modelo[TAM_MODELO];
    int ano;
    char categoria[TAM_CATEGORIA];
    char combustivel[MAX_COMBUSTIVEIS][TAM_TEXTO];
    int cilindros;
    double cilindrada;
    char transmissao[TAM_TEXTO];
    char tracao[TAM_TEXTO];
    double consumoCidade;
    double consumoEstrada;
    double co2;
    bool turbo;
    Data dataRegistro;   
} Veiculo;


/* Converte "0000-00-00" (ano-mês-dia, como no CSV) em uma Data.
 * sscanf lê valores de dentro de um texto, seguindo um formato:
 */

Data parseData(char *s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

// Escreve a data no buffer no formato DD/MM/YYYY.

void formatData(Data d, char *buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}



/* Converte uma linha do CSV em um Veiculo (alocado com calloc).
 * O chamador é quem deve dar free() no ponteiro devolvido.
 */

Veiculo *parseVeiculo(char *s) {

    /* calloc aloca a memória ja zerada. Assim os combustíveis não usados
     * ficam como "" (texto vazio), que é o marcador de fim usado no formatVeiculo. */

    Veiculo *v = (Veiculo *) calloc(1, sizeof(Veiculo));
    if (v == NULL) {
        return NULL;
    }

    /* PASSO 1: cortar a linha nas vírgulas e guardar cada pedaço em campos[].
     * strtok corta o texto no delimitador; na 1ª chamada recebe o texto,
     * nas seguintes recebe NULL para continuar de onde parou.
     * "\r\n" também é delimitador: assim o final da linha não entra no último campo. */
    char *campos[15];
    char *pedaco = strtok(s, ",\r\n");
    int n = 0;
    while (pedaco != NULL && n < 15) {
        campos[n] = pedaco;
        n++;
        pedaco = strtok(NULL, ",\r\n");
    }

    // Se a linha não tem os 15 campos, ela é inválida. 
    if (n < 15) {
        free(v);
        return NULL;
    }

    //converter cada campo para o tipo certo.
   

    v->id = atoi(campos[0]);
    sprintf(v->marca, "%s", campos[1]);
    sprintf(v->modelo, "%s", campos[2]);
    v->ano = atoi(campos[3]);
    sprintf(v->categoria, "%s", campos[4]);

    int k = 0;
    char *comb = strtok(campos[5], ";");
    while (comb != NULL && k < MAX_COMBUSTIVEIS) {
        sprintf(v->combustivel[k], "%s", comb);
        k++;
        comb = strtok(NULL, ";");
    }

    v->cilindros = atoi(campos[6]);
    v->cilindrada = atof(campos[7]);
    sprintf(v->transmissao, "%s", campos[8]);
    sprintf(v->tracao, "%s", campos[9]);
    v->consumoCidade = atof(campos[10]);
    v->consumoEstrada = atof(campos[11]);
    v->co2 = atof(campos[12]);

    /* turbo: o texto "true" vira true (strcmp devolve 0 quando os textos são iguais) */
    v->turbo = (strcmp(campos[13], "true") == 0);

    v->dataRegistro = parseData(campos[14]);

    return v;
}


void formatVeiculo(Veiculo v, char *buffer) {

    /* Monta o texto dos combustíveis separados por vírgula, sem espaço.
     * sprintf devolve quantos caracteres escreveu; somamos em "pos"
     * para o próximo pedaço ser escrito logo depois do anterior.
     * O for para quando encontra um combustível vazio ("") ou no máximo. */
    char combustiveis[TAM_BUFFER];
    combustiveis[0] = '\0';
    int pos = 0;
    for (int i = 0; i < MAX_COMBUSTIVEIS && v.combustivel[i][0] != '\0'; i++) {
        pos += sprintf(combustiveis + pos, "%s%s", (i > 0 ? "," : ""), v.combustivel[i]);
    }

    /* Data já formatada como DD/MM/YYYY */
    char data[20];
    formatData(v.dataRegistro, data);

    /* %d = inteiro | %s = texto | %.1f = decimal com 1 casa | %.2f = 2 casas.
     * turbo (bool) vira o texto "true" ou "false" pelo operador ternário. */
    sprintf(buffer,
            "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %s ## %s]",
            v.id, v.marca, v.modelo, v.ano, v.categoria, combustiveis,
            v.cilindros, v.cilindrada, v.transmissao, v.tracao,
            v.consumoCidade, v.consumoEstrada, v.co2,
            (v.turbo ? "true" : "false"), data);
}

/* Lê o CSV e devolve um vetor de Veiculo (alocado com malloc).
 * A quantidade de veículos lidos é devolvida pelo parâmetro n (ponteiro):
 * como a função só pode dar um return, "*n = ..." é como ela entrega um segundo resultado.
 * O chamador deve dar free() no vetor devolvido. */
Veiculo *lerCsv(char *caminhoArquivo, int *n) {
    *n = 0;

    FILE *arq = fopen(caminhoArquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s\n", caminhoArquivo);
        return NULL;
    }

    /* Não sabemos quantas linhas o arquivo tem: começamos com espaço para 100
     * veículos e dobramos com realloc sempre que encher. */
    int capacidade = 100;
    Veiculo *vetor = (Veiculo *) malloc(capacidade * sizeof(Veiculo));
    if (vetor == NULL) {
        fclose(arq);
        return NULL;
    }

    char linha[TAM_LINHA];

    /* A primeira linha só tem os nomes das colunas: lemos e descartamos. */
    fgets(linha, TAM_LINHA, arq);

    /* fgets lê uma linha por vez; devolve NULL quando o arquivo acaba. */
    while (fgets(linha, TAM_LINHA, arq) != NULL) {

        /* Ignora linhas vazias. */
        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == '\0') {
            continue;
        }

        /* Se o vetor encheu, dobra o tamanho. */
        if (*n == capacidade) {
            capacidade = capacidade * 2;
            vetor = (Veiculo *) realloc(vetor, capacidade * sizeof(Veiculo));
            if (vetor == NULL) {
                fclose(arq);
                return NULL;
            }
        }

        /* Converte a linha em Veiculo, copia para o vetor e libera o temporário. */
        Veiculo *v = parseVeiculo(linha);
        if (v != NULL) {
            vetor[*n] = *v;   /* copia o struct inteiro */
            free(v);
            (*n)++;
        }
    }

    fclose(arq);
    return vetor;
}


int main(void) {
    int n = 0;

    /* Lê o CSV uma única vez. O arquivo precisa estar na pasta onde o programa roda. */
    Veiculo *veiculos = lerCsv("veiculos.csv", &n);
    if (veiculos == NULL) {
        return 1;
    }

    char buffer[TAM_BUFFER];
    int id;

    while (scanf("%d", &id) == 1 && id != -1) {

        /* percorre o vetor do início ao fim
         * comparando o id de cada veículo com o id procurado. */
        for (int i = 0; i < n; i++) {
            if (veiculos[i].id == id) {
                formatVeiculo(veiculos[i], buffer);
                printf("%s\n", buffer);
                break;   /* achou: não precisa continuar procurando */
            }
        }
    }

    /* Libera a memória alocada por lerCsv. */
    free(veiculos);
    return 0;
}