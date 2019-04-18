#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

void saidaArquivo(int * mochila, float * peso, float * valor, int max);
int melhorCxB(float * custo, int max, float * peso);
int preencherMochila(float * custo, float * peso, int max, int tamanhoMochila, int * itensNaMochila);
void inicializa(float * custo, float * valor, float * peso, int * itensNaMochila, int max);

void main() {
    char arqName[50];
    float * itensPeso, * itensValor, * custoBenef;
    int * itensNaMochila;
    int tamanhoMochila, qtdItens;

    printf("Digite o nome do arquivo: ");
    scanf("%s", arqName);

    FILE * fp = fopen(arqName, "r");
    int i = 0;

    if (!fp) {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    while (!feof(fp)) {
        if (i == 0)
            fscanf(fp, "%d", & tamanhoMochila);
        else if (i == 1) {
            fscanf(fp, "%d", & qtdItens);
            itensPeso = malloc(qtdItens * sizeof(float));
            itensValor = malloc(qtdItens * sizeof(float));
            custoBenef = malloc(qtdItens * sizeof(float));
            itensNaMochila = malloc(qtdItens * sizeof(int));
        } else if (i > 1) {
            fscanf(fp, "%f", & itensPeso[i - 2]);
            fscanf(fp, "%f", & itensValor[i - 2]);
        }
        i++;
    }

    fclose(fp);

    inicializa(custoBenef, itensValor, itensPeso, itensNaMochila, qtdItens);

    //fill the knapsack with the best profit value / weight
    int num = preencherMochila(custoBenef, itensPeso, qtdItens, tamanhoMochila, itensNaMochila);

    //num = number of items in the knapsack
    saidaArquivo(itensNaMochila, itensPeso, itensValor, num);
}

void saidaArquivo(int * mochila, float * peso, float * valor, int max) {
    FILE * fp;
    int pesoTotal = 0;
    int valorTotal = 0;

    for (int i = 0; i < max; i++) {
        pesoTotal += peso[mochila[i]];
        valorTotal += valor[mochila[i]];
    }

    fp = fopen("resposta.txt", "w");
    if (!fp) {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    for (int i = 0; i < max; i++) {
        fprintf(fp, "Numero do item:  %d", mochila[i] + 1);
        fprintf(fp, " Peso do item:  %.0f", peso[mochila[i]]);
        fprintf(fp, " Valor do item:  %.0f", valor[mochila[i]]);
        fputs("\n", fp);
    }
    fputs("\n", fp);
    fprintf(fp, "Somatorio dos pesos: %d\n", pesoTotal);
    fprintf(fp, "Somatorio dos valores: %d", valorTotal);

    fclose(fp);
}

int melhorCxB(float * custo, int max, float * peso) {
    float maiorCxB = 0;
    int index = 0;
    for (int i = 0; i < max; i++) {
        if (custo[i] > maiorCxB) {
            maiorCxB = custo[i];
            index = i;
        } else if (custo[i] == maiorCxB)
            if (peso[i] < peso[index])
                index = i;
    }

    custo[index] = -1; //-1 = item has already been tested

    return index;
}

int preencherMochila(float * custo, float * peso, int max, int tamanhoMochila, int * itensNaMochila) {
    int espaco = 0;
    int j = 0;

    //atual is the current index of best profit
    for (int i = 0; i < max; i++) {
        int atual = melhorCxB(custo, max, peso);
        if ((espaco + peso[atual]) <= tamanhoMochila) {
            espaco += peso[atual];
            itensNaMochila[j] = atual;
            j++;
        }
    }
    return j;
}

//init and calculates profit of each item
void inicializa(float * custo, float * valor, float * peso, int * itensNaMochila, int max) {
    for (int i = 0; i < max; i++)
        custo[i] = valor[i] / peso[i];
    for (int i = 0; i < max; i++)
        itensNaMochila[i] = -1;
}