#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>

void saidaArquivo(int * itensNaMochila, int * itensPeso, int * itensValor, int qtdItens, int pesoFinal, int valorFinal);
void geraVetor(int * vetor, int n);
void mochila(int * itensNaMochila, int qtdItens, int * itensPeso, int * itensValor, int tamanhoMochila, int * pesoFinal, int * valorFinal);
int calcPeso(int * itensNaMochila, int * itensPeso, int qtdItens);
int calcValor(int * itensNaMochila, int * itensValor, int qtdItens);
void copy(int * orig, int * dest, int n);
void zera(int * vetor, int n);

void main() {
    int * itensNaMochila, qtdItens, * itensPeso, * itensValor, tamanhoMochila, pesoFinal, valorFinal;
    char arq[30];
    FILE * fp;

    printf("Digite o nome do arquivo: ");
    scanf("%s", arq);

    fp = fopen(arq, "r");
    if (!fp) {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    int i = 0;

    //fill the arrays with weight and value of the items, as well as the knapsack size
    while (!feof(fp)) {
        if (i == 0)
            fscanf(fp, "%d", & tamanhoMochila);
        else if (i == 1) {
            fscanf(fp, "%d", & qtdItens);
            itensPeso = malloc(qtdItens * sizeof(int));
            itensValor = malloc(qtdItens * sizeof(int));
            itensNaMochila = malloc(qtdItens * sizeof(int));
        } else if (i > 1) {
            fscanf(fp, "%d", & itensPeso[i - 2]);
            fscanf(fp, "%d", & itensValor[i - 2]);
        }
        i++;
    }
    fclose(fp);
    zera(itensNaMochila, qtdItens);
    mochila(itensNaMochila, qtdItens, itensPeso, itensValor, tamanhoMochila, & pesoFinal, & valorFinal);
    saidaArquivo(itensNaMochila, itensPeso, itensValor, qtdItens, pesoFinal, valorFinal);
}

void saidaArquivo(int * itensNaMochila, int * itensPeso, int * itensValor, int qtdItens, int pesoFinal, int valorFinal) {
    FILE * fp;

    fp = fopen("resposta.txt", "w");
    if (!fp) {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    for (int i = 0; i < qtdItens; i++)
        if (itensNaMochila[i] == 1) {
            fprintf(fp, "Numero do item:  %d", i + 1);
            fprintf(fp, " Peso do item:  %d", itensPeso[i]);
            fprintf(fp, " Valor do item:  %d", itensValor[i]);
            fputs("\n", fp);
        }

    fputs("\n", fp);
    fprintf(fp, "Somatorio dos pesos: %d\n", pesoFinal);
    fprintf(fp, "Somatorio dos valores: %d", valorFinal);

    fclose(fp);
}

void zera(int * vetor, int n) {
    for (int i = 0; i < n; i++)
        vetor[i] = 0;
}

void geraVetor(int * vetor, int n) {
    for (int i = 0; i < n; i++) {
        if (vetor[i] == 0) {
            vetor[i] = 1;
            break;
        } else
            vetor[i] = 0;
    }
}

int calcPeso(int * itensNaMochila, int * itensPeso, int qtdItens) {
    int sum = 0;
    for (int i = 0; i < qtdItens; i++)
        if (itensNaMochila[i]) //if the current item is in the knapsack, sum its weight
            sum += itensPeso[i];
    return sum;
}

int calcValor(int * itensNaMochila, int * itensValor, int qtdItens) {
    int sum = 0;
    for (int i = 0; i < qtdItens; i++)
        if (itensNaMochila[i]) //if the current item is in the knapsack, sum its value
            sum += itensValor[i];
    return sum;
}

//copy binary array
void copy(int * orig, int * dest, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = orig[i];
}

void mochila(int * itensNaMochila, int qtdItens, int * itensPeso, int * itensValor, int tamanhoMochila, int * pesoFinal, int * valorFinal) {
    * valorFinal = 0;
    * pesoFinal = 0;
    int * aux = malloc(qtdItens * sizeof(int));
    zera(aux, qtdItens);

    //generates all the possible combinations with 0 (not in) and 1 (in the knapsack) -> 2^n - 1 combinations
    for (int i = 0; i < pow(2, qtdItens) - 1; i++) {
        geraVetor(itensNaMochila, qtdItens); //calculates the next binary array (counter)
        int currentW = calcPeso(itensNaMochila, itensPeso, qtdItens); //current weight of this comb
        int currentV = calcValor(itensNaMochila, itensValor, qtdItens); //current value of this comb
        if (currentW <= tamanhoMochila && currentV > * valorFinal) { //if current weight does not exceed knapsack size
            copy(itensNaMochila, aux, qtdItens); //and value is better than last one, take it as a possible solution
            * valorFinal = currentV;
            * pesoFinal = currentW;
        }
    }
    copy(aux, itensNaMochila, qtdItens);
}