#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include "trie.h"
//#include "dataset.h"
#include "trieBin.h"
#include "datasetBin.h"

// leitura dos dados e construcao da trie

// implementação de uma API de buscas na trie, recebendo inputs, e mostrando
// o output e o tempo necessario para encontrar cada registro. Talvez valha
// a pena colocar a API em arquivo proprio e depois integrar tudo, mas parece
// simples o bastante para deixar em um arquivo único.
// ideia das operaçoes: 
// 0 - busca de um código de barras especifico.
// 1 - filtragem por empresa 
// 2 - sair


#define MAX_PRODUTOS 1000000
#define REP 1
#define PREFIXO 5

char codigos[MAX_PRODUTOS][14];
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

        sscanf(linha, "%13[^,]", codigos[nCodigos]);
        nCodigos++;
    }

    fclose(fp);
}

double TempoMs(clock_t ini, clock_t fim){

    return 1000.0 * (fim - ini) / CLOCKS_PER_SEC;
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

    ini = clock();

    Trie *root = TrieCreateBin(fp);

    fim = clock();

    int nos;
    size_t memoria = TrieMemoriaBin(root, &nos);

    printf("\n===== MEMORIA =====\n");
    printf("Nos               : %d\n", nos);
    printf("Memoria da trie   : %.2f KB\n", memoria / 1024.0);

    fclose(fp);

    printf("Criacao              : %10.3lf ms\n",
           TempoMs(ini,fim));

    /*=========================
      BUSCA EXISTENTE
    =========================*/

    ini = clock();

    for(int r=0;r<REP;r++)
        for(int i=0;i<nCodigos;i++)
            TrieSearchBin(root,codigos[i]);

    fim = clock();

    printf("Busca existente      : %10.3lf ms (%.3lf us/op)\n",
           TempoMs(ini,fim),
           TempoMs(ini,fim)*1000.0/(REP*nCodigos));

    /*=========================
      BUSCA INEXISTENTE
    =========================*/

    ini = clock();

    for(int r=0;r<REP;r++){

        for(int i=0;i<nCodigos;i++){

            char codigo[14];

            strcpy(codigo,codigos[i]);

            codigo[12] = (codigo[12]=='9') ? '8' : '9';

            TrieSearchBin(root,codigo);
        }
    }

    fim = clock();

    printf("Busca inexistente    : %10.3lf ms (%.3lf us/op)\n",
           TempoMs(ini,fim),
           TempoMs(ini,fim)*1000.0/(REP*nCodigos));

    /*=========================
      PREFIXOS
    =========================*/

    int digitos[10];

    ini = clock();

    for(int r=0;r<REP;r++){

        for(int i=0;i<nCodigos;i++){

            char prefixo[PREFIXO+1];

            strncpy(prefixo,codigos[i],PREFIXO);
            prefixo[PREFIXO]='\0';

            TriePrefixosBin(root,prefixo,digitos);
        }
    }

    fim = clock();

    printf("Prefixos             : %10.3lf ms (%.3lf us/op)\n",
           TempoMs(ini,fim),
           TempoMs(ini,fim)*1000.0/(REP*nCodigos));

    /*=========================
      CONTA PREFIXO
    =========================*/

    ini = clock();

    for(int r=0;r<REP;r++){

        for(int i=0;i<nCodigos;i++){

            char prefixo[PREFIXO+1];

            strncpy(prefixo,codigos[i],PREFIXO);
            prefixo[PREFIXO]='\0';

            TrieContaPrefixoBin(root,prefixo);
        }
    }

    fim = clock();

    printf("Conta prefixo        : %10.3lf ms (%.3lf us/op)\n",
           TempoMs(ini,fim),
           TempoMs(ini,fim)*1000.0/(REP*nCodigos));

    /*=========================
      REMOÇÃO
    =========================*/

    ini = clock();

    for(int i=0;i<nCodigos;i++){

        Produto *p = TrieRemoveBin(root,codigos[i]);

        if(p != NULL)
            free(p);
    }

    fim = clock();

    printf("Remocao              : %10.3lf ms (%.3lf us/op)\n",
           TempoMs(ini,fim),
           TempoMs(ini,fim)*1000.0/nCodigos);

    /*=========================
      DESTRUIÇÃO
    =========================*/

    ini = clock();

    TrieDeleteBin(root->root);
    free(root);

    fim = clock();

    printf("Destruicao           : %10.3lf ms\n",
           TempoMs(ini,fim));

    return 0;
}