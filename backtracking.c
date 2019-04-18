/************************************************

LAED1 - Trabalho Pratico 1
Alunos: Davi Emediato e Lucca Miranda
Matriculas: 20183012896 e 20183011736
Descricao do programa: problema da mochila por
tentativa e erro (bruteforce)
Data: 12/04/2019

************************************************/

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
void copia(int * orig, int * dest, int n);
void zera(int * vetor, int n);
struct timeval inicio, fim;
struct rusage usage;

void main() {
    int * itensNaMochila, qtdItens, * itensPeso, * itensValor, tamanhoMochila, pesoFinal, valorFinal;
    char arq[30];
    FILE * fp;
    int who = RUSAGE_SELF;
    double tempoTotal, uTotal, sTotal;

    printf("Digite o nome do arquivo: ");
    scanf("%s", arq);

    gettimeofday(&inicio, NULL);

    fp = fopen(arq, "r");
    if (!fp) {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    int i = 0;

    //preenche os vetores de peso e valor dos itens, bem como numero de itens e cap da mochila
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

    gettimeofday(&fim, NULL);
	getrusage(who, &usage);
    long double utotalseg = usage.ru_utime.tv_sec;
    long double utotalmicroseg = usage.ru_utime.tv_usec; 
    long double stotalseg = usage.ru_stime.tv_sec;
    long double stotalmicroseg = usage.ru_stime.tv_usec;

    long double totalseg = fim.tv_sec - inicio.tv_sec; //tempo final - inicial em segundos
	long double totalmicroseg = fim.tv_usec - inicio.tv_usec; //tempo final - inicial em microsegundos

    if (totalmicroseg < 0){
        totalmicroseg += 1000000;
        totalseg --;
    }

    tempoTotal = totalseg + (totalmicroseg/1000000);
    uTotal = utotalseg + (utotalmicroseg/1000000);
    sTotal = stotalseg + (stotalmicroseg/1000000);

    printf("\n");
    printf("Tempo total de execucao = %lf", tempoTotal);
    printf("\n");
    
    printf ("\n");
    printf ("***************************************************************\n");
    printf ("Tempo total: %Lf segundos e %Lf microssegundos.\n", totalseg, totalmicroseg);
    printf ("***************************************************************\n");
    printf ("\n");

    printf("\n");
    printf("Tempo total de CPU = %lf", uTotal);
    printf("\n");

    printf("\n");
    printf("Tempo total de chamadas de sistema = %lf", sTotal);
    printf("\n");

    printf ("\n");
    printf ("***************************************************************\n");
    printf ("Tempo de usuario: %Lf segundos e %Lf microssegundos.\n", utotalseg, utotalmicroseg);
    printf ("Tempo de sistema: %Lf segundos e %Lf microssegundos.\n", stotalseg, stotalmicroseg);
    printf ("***************************************************************\n");
    printf ("\n");
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
        if (itensNaMochila[i]) //se item atual esta na mochila, soma o peso dele no total
            sum += itensPeso[i];
    return sum;
}

int calcValor(int * itensNaMochila, int * itensValor, int qtdItens) {
    int sum = 0;
    for (int i = 0; i < qtdItens; i++)
        if (itensNaMochila[i]) //se item atual esta na mochila, soma o valor dele no total
            sum += itensValor[i];
    return sum;
}

//copia o vetor binario atual para a solucao
void copia(int * orig, int * dest, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = orig[i];
}

void mochila(int * itensNaMochila, int qtdItens, int * itensPeso, int * itensValor, int tamanhoMochila, int * pesoFinal, int * valorFinal) {
    * valorFinal = 0;
    * pesoFinal = 0;
    int * aux = malloc(qtdItens * sizeof(int));
    zera(aux, qtdItens);

    //gera todas combinacoes possiveis com 0 (nao esta na mochila) e 1 (esta na mochila) -> 2^n - 1 combinacoes
    for (int i = 0; i < pow(2, qtdItens) - 1; i++) {
        geraVetor(itensNaMochila, qtdItens); //calcula proximo vetor binario (contador)
        int currentW = calcPeso(itensNaMochila, itensPeso, qtdItens); //calcula o peso da combinacao atual
        int currentV = calcValor(itensNaMochila, itensValor, qtdItens); //calcula valor total da combinacao atual
        if (currentW <= tamanhoMochila && currentV > * valorFinal) { //se peso atual nao excede a mochila e valor eh maior que o ultimo,
            copia(itensNaMochila, aux, qtdItens); //toma ele como (possivel) solucao
            * valorFinal = currentV;
            * pesoFinal = currentW;
        }
    }

    copia(aux, itensNaMochila, qtdItens);
}