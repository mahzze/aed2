#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Uso: %s arquivo.txt\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if(fp == NULL){
        perror("Erro ao abrir arquivo");
        return 1;
    }

    Trie *root = TrieCreateBin(fp);

    fclose(fp);

    char codigo[14];

    while(1){

        printf("\nDigite um codigo de 13 digitos (0 para sair): ");
        scanf("%13s", codigo);

        if(strcmp(codigo, "0") == 0)
            break;

        Produto *p = TrieSearchBin(root, codigo);

        if(p == NULL){

            printf("\nProduto nao encontrado.\n");

        }else{

            printf("\nProduto encontrado\n");
            printf("Nome    : %s\n", p->nome);
            printf("Paises  : %s\n", p->paises);
            printf("Labels  : %s\n", p->labels);

        }
    }

    TrieDeleteBin(root->root);
    free(root);


    /*
    FILE *fp = fopen(argv[1], "r");

    if(fp == NULL){
        perror("Erro ao abrir arquivo");
        return 1;
    }

    Trie *root = TrieCreate(fp);

    fclose(fp);

    char codigo[14];

    while(1){

        printf("\nDigite um codigo de 13 digitos (0 para sair): ");
        scanf("%13s", codigo);

        if(strcmp(codigo, "0") == 0)
            break;

        Produto *p = TrieSearchBin(root, codigo);

        if(p == NULL){

            printf("\nProduto nao encontrado.\n");

        }else{

            printf("\nProduto encontrado\n");
            printf("Nome    : %s\n", p->nome);
            printf("Paises  : %s\n", p->paises);
            printf("Labels  : %s\n", p->labels);

        }
    }

    TrieDeleteBin(root->root);
    free(root);
    */

    return 0;
}