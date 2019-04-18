/************************************************

LAED1 - Trabalho Pratico 1
Aluno: Davi Emediato e Lucca Miranda
Matricula: 20183012896 e 20183011736
Descricao do programa: problema da mochila por estrategia gulosa
Data: 12/04/2019

************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

void saidaArquivo(int * mochila, float * peso, float * valor, int max);
int melhorCxB(float * custo, int max, float * peso);
int preencherMochila(float * custo, float * peso, int max, int tamanhoMochila, int * itensNaMochila);
void inicializa(float * custo, float * valor, float * peso, int * itensNaMochila, int max);
struct timeval inicio, fim;
struct rusage usage;

void main() {
    long totalmicroseg, totalseg;
    int who = RUSAGE_SELF;
    long utotalmicroseg, utotalseg;
    long stotalmicroseg, stotalseg;
    char arqName[50];
    float * itensPeso, * itensValor, * custoBenef;
    int * itensNaMochila;
    int tamanhoMochila, qtdItens;

    printf("Digite o nome do arquivo: ");
    scanf("%s", arqName);

    gettimeofday( & inicio, NULL); //comeca a computar o tempo de execucao

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

    //ira preencher a mochila com os melhores custo beneficios
    //com o melhor aproveitamento do peso total da mochila

    int num = preencherMochila(custoBenef, itensPeso, qtdItens, tamanhoMochila, itensNaMochila);

    //num sao quantos itens estao dentro da mochila
    saidaArquivo(itensNaMochila, itensPeso, itensValor, num);

    getrusage(who, & usage);
    utotalseg = usage.ru_utime.tv_sec;
    utotalmicroseg = usage.ru_utime.tv_usec;
    stotalseg = usage.ru_stime.tv_sec;
    stotalmicroseg = usage.ru_stime.tv_usec;
    printf("\n");
    printf("***************************************************************\n");
    printf("Tempo de usuario: %ld segundos e %ld microssegundos.\n", utotalseg, utotalmicroseg);
    printf("Tempo de sistema: %ld segundos e %ld microssegundos.\n", stotalseg, stotalmicroseg);
    printf("***************************************************************\n");
    gettimeofday( & fim, NULL);
    totalseg = fim.tv_sec - inicio.tv_sec;
    totalmicroseg = fim.tv_usec - inicio.tv_usec;
    if (totalmicroseg < 0) {
        totalmicroseg += 1000000;
        totalseg -= 1;
    }
    printf("\n");
    printf("***************************************************************\n");
    printf("Tempo total: %ld segundos e %ld microssegundos.\n", totalseg, totalmicroseg);
    printf("***************************************************************\n");
    printf("\n");
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
    float maiorCxB = 0; // valor a ser comparado
    int index = 0;
    for (int i = 0; i < max; i++) {
        if (custo[i] > maiorCxB) {
            maiorCxB = custo[i];
            index = i;
        } else if (custo[i] == maiorCxB)
            if (peso[i] < peso[index])
                index = i;
    }

    custo[index] = -1; //-1 = flag que indica que item ja foi testado

    return index;
}

//preenche a mochila com os melhores custos beneficios (observando o limite de peso da mochila)
int preencherMochila(float * custo, float * peso, int max, int tamanhoMochila, int * itensNaMochila) {
    int espaco = 0;
    int j = 0;

    //atual armazena o index do melhor CxB
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

//funcao que calcula o custo beneficio de cada item e inicializa os itens
void inicializa(float * custo, float * valor, float * peso, int * itensNaMochila, int max) {
    for (int i = 0; i < max; i++)
        custo[i] = valor[i] / peso[i];
    for (int i = 0; i < max; i++)
        itensNaMochila[i] = -1;
}