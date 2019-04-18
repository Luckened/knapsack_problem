/************************************************
LAED1 - Trabalho Pratico 1
Aluno: Davi Emediato e Lucca Miranda
Matricula: xxxxx e 20183011736
Descricao do programa: gera arquivos com itens de valor e peso randomicos
Data: 12/04/2019
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void main()
{
    srand(time(NULL));
    int cap, num;
    FILE *fp = fopen("itens.txt", "w");
    if (!fp)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(0);
    }

    printf("Quantos itens?\n");
    scanf("%d", &num);
    double temp = 0;
    int tempW[num], tempV[num];
    for (int i = 0; i < num; i++)
    {
        tempW[i] = rand() % 9 + 1;
        tempV[i] = rand() % 9 + 1;
        temp += tempW[i];
    }
    int W = floor(temp / 2);
    W = rand()%W + 1;
    fprintf(fp, "%d\n", W);
    fprintf(fp, "%d\n", num);
    for (int i = 0; i < num; i++)
    {
        fprintf(fp, "%d ", tempW[i]);
        fprintf(fp, "%d", tempV[i]);
        if (i < num - 1)
            fputs("\n", fp);
    }
    fclose(fp);
}