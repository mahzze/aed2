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

    int opcao;

    while(1){

        printf("\n");
        printf("1 - Buscar produto\n");
        printf("2 - Remover produto\n");
        printf("3 - Proximos digitos validos\n");
        printf("4 - Contar produtos com prefixo\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        scanf("%d", &opcao);

        if(opcao == 0)
            break;

        if(opcao == 1){

            char codigo[14];

            printf("Codigo: ");
            scanf("%13s", codigo);

            Produto *p = TrieSearchBin(root, codigo);

            if(p == NULL){

                printf("\nProduto nao encontrado.\n");

            }else{

                printf("\nProduto encontrado\n");
                printf("Nome   : %s\n", p->nome);
                printf("Paises : %s\n", p->paises);
                printf("brand : %s\n", p->brand);

            }

        }else if(opcao == 2){

            char codigo[14];

            printf("Codigo a remover: ");
            scanf("%13s", codigo);

            Produto *p = TrieRemoveBin(root, codigo);

            if(p == NULL){

                printf("Produto nao encontrado.\n");

            }else{

                printf("Produto removido\n");
                printf("Nome   : %s\n", p->nome);
                printf("Paises : %s\n", p->paises);
                printf("brand : %s\n", p->brand);

                free(p);
            }

        }else if(opcao == 3){

            char prefixo[14];
            int n;
            int digitos[10];

            printf("Prefixo: ");
            scanf("%13s", prefixo);

            int qtd = TriePrefixosBin(root,
                                      prefixo,
                                      digitos);

            if(qtd == 0){

                printf("Nenhuma continuacao encontrada.\n");

            }else{

                printf("Proximos digitos possiveis: ");

                for(int i = 0; i < qtd; i++)
                    printf("%d ", digitos[i]);

                printf("\n");
            }

        }else if(opcao == 4){

            char prefixo[14];
            int n;

            printf("Prefixo: ");
            scanf("%13s", prefixo);

            int qtd = TrieContaPrefixoBin(root,
                                          prefixo);

            printf("Quantidade de produtos com esse prefixo: %d\n", qtd);

        }else{

            printf("Opcao invalida.\n");

        }
    }

    TrieDeleteBin(root->root);
    free(root);

    return 0;
}