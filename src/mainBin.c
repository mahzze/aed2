#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "trieBin.h"
#include "datasetBin.h"

#define MAX_PRODUTOS 10000
#define REP 100
#define PREFIXO 6

char codigos[MAX_PRODUTOS][14];
char nomes[MAX_PRODUTOS][200];
char paises[MAX_PRODUTOS][200];
char brands[MAX_PRODUTOS][200];

int nCodigos = 0;

void LerCodigos(const char *arquivo){

    FILE *fp = fopen(arquivo, "r");

    if(fp == NULL){
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char linha[4096];

    fgets(linha, sizeof(linha), fp);

    while(fgets(linha, sizeof(linha), fp)){

        sscanf(linha,
               "%13[^,],%199[^,],%199[^,],%199[^\n]",
               codigos[nCodigos],
               nomes[nCodigos],
               paises[nCodigos],
               brands[nCodigos]);

        nCodigos++;
    }

    fclose(fp);
}

double TempoMs(clock_t ini, clock_t fim){

    return 1000.0 * (fim - ini) / CLOCKS_PER_SEC;
}

double Media(double v[], int n){

    double soma = 0.0;

    for(int i = 0; i < n; i++)
        soma += v[i];

    return soma / n;
}

int main(int argc, char *argv[]){

    if(argc != 2){

        printf("Uso: %s dataset.csv\n", argv[0]);
        return 1;
    }

    LerCodigos(argv[1]);

    FILE *fp = fopen(argv[1], "r");

    if(fp == NULL){

        perror("Erro");
        return 1;
    }

    clock_t ini, fim;

    printf("=====================================\n");
    printf("          BENCHMARK TRIE\n");
    printf("=====================================\n\n");

    printf("Produtos: %d\n", nCodigos);
    printf("Repeticoes: %d\n\n", REP);

    /*=========================
      CRIAÇÃO
    =========================*/

double tempos[REP];

for(int r = 0; r < REP; r++){

    fp = fopen(argv[1], "r");

    if(fp == NULL){
        perror("Erro");
        return 1;
    }

    ini = clock();

    Trie *rootTeste = TrieCreateBin(fp);

    fim = clock();

    tempos[r] = TempoMs(ini, fim);

    fclose(fp);
    TrieDeleteBin(rootTeste);
}

double mediaCriacao = Media(tempos, REP);

/* Cria a trie definitiva para os demais testes */

fp = fopen(argv[1], "r");

if(fp == NULL){
    perror("Erro");
    return 1;
}

Trie *root = TrieCreateBin(fp);

fclose(fp);

int nos;
size_t memoria = TrieMemoriaBin(root, &nos);

printf("\n===== MEMORIA =====\n");
printf("Nos               : %d\n", nos);
printf("Memoria da trie   : %.2f KB\n", memoria / 1024.0);

printf("Criacao              : %10.3lf ms (%.3lf us/prod)\n",
       mediaCriacao,
       mediaCriacao * 1000.0 / nCodigos);




for(int r = 0; r < REP; r++){

    Trie *rootInsert = malloc(sizeof(Trie));
    rootInsert->root = NewNode();

    ini = clock();

    for(int i = 0; i < nCodigos; i++){

        TrieInsertBin(rootInsert,
                      codigos[i],
                      nomes[i],
                      paises[i],
                      brands[i]);
    }

    fim = clock();

    tempos[r] = TempoMs(ini, fim);

    TrieDeleteBin(rootInsert);
}

double mediaInsercao = Media(tempos, REP);

printf("Insercao             : %10.3lf ms (%.3lf us/op)\n",
       mediaInsercao,
       mediaInsercao * 1000.0 / nCodigos);



    /*=========================
      BUSCA EXISTENTE
    =========================*/

for(int r = 0; r < REP; r++){

    ini = clock();

    for(int i = 0; i < nCodigos; i++)
        TrieSearchBin(root, codigos[i]);

    fim = clock();

    tempos[r] = TempoMs(ini, fim);
}

double mediaBusca = Media(tempos, REP);

printf("Busca existente      : %10.3lf ms (%.3lf us/op)\n",
       mediaBusca,
       mediaBusca * 1000.0 / nCodigos);

    /*=========================
      BUSCA INEXISTENTE
    =========================*/

for(int r = 0; r < REP; r++){

    ini = clock();

    for(int i = 0; i < nCodigos; i++){

        char codigo[14];

        strcpy(codigo, codigos[i]);

        codigo[11] = (codigo[11]=='9') ? '8' : '9';
        codigo[12] = (codigo[12]=='9') ? '8' : '9';

        TrieSearchBin(root, codigo);
    }

    fim = clock();

    tempos[r] = TempoMs(ini, fim);
}

double mediaBuscaInv = Media(tempos, REP);

printf("Busca inexistente    : %10.3lf ms (%.3lf us/op)\n",
       mediaBuscaInv,
       mediaBuscaInv * 1000.0 / nCodigos);

    /*=========================
      PREFIXOS
    =========================*/

int digitos[10];

for(int r = 0; r < REP; r++){

    ini = clock();

    for(int i = 0; i < nCodigos; i++){

        char prefixo[PREFIXO+1];

        strncpy(prefixo, codigos[i], PREFIXO);
        prefixo[PREFIXO] = '\0';

        TriePrefixosBin(root, prefixo, digitos);
    }

    fim = clock();

    tempos[r] = TempoMs(ini, fim);
}

double mediaPrefixo = Media(tempos, REP);

printf("Prefixos             : %10.3lf ms (%.3lf us/op)\n",
       mediaPrefixo,
       mediaPrefixo * 1000.0 / nCodigos);

    /*=========================
      CONTA PREFIXO
    =========================*/
for(int r = 0; r < REP; r++){

    ini = clock();

    for(int i = 0; i < nCodigos; i++){

        char prefixo[PREFIXO+1];

        strncpy(prefixo, codigos[i], PREFIXO);
        prefixo[PREFIXO] = '\0';

        TrieContaPrefixoBin(root, prefixo);
    }

    fim = clock();

    tempos[r] = TempoMs(ini, fim);
}

double mediaConta = Media(tempos, REP);

printf("Conta prefixo        : %10.3lf ms (%.3lf us/op)\n",
       mediaConta,
       mediaConta * 1000.0 / nCodigos);

    /*=========================
      REMOÇÃO
    =========================*/

for(int r = 0; r < REP; r++){

    rewind(fp);

    Trie *rootRem = TrieCreateBin(fp);

    ini = clock();

    for(int i = 0; i < nCodigos; i++){

        Produto *p = TrieRemoveBin(rootRem, codigos[i]);

        if(p != NULL)
            free(p);
    }

    fim = clock();

    tempos[r] = TempoMs(ini, fim);

    TrieDeleteBin(rootRem);
}

double mediaRemocao = Media(tempos, REP);

printf("Remocao              : %10.3lf ms (%.3lf us/op)\n",
       mediaRemocao,
       mediaRemocao * 1000.0 / nCodigos);

    /*=========================
      DESTRUIÇÃO
    =========================*/

    ini = clock();

    TrieDeleteBin(root);

    fim = clock();

    printf("Destruicao           : %10.3lf ms\n",
           TempoMs(ini,fim));

    return 0;
}