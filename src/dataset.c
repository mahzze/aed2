#include <stdio.h>

#include "dataset.h"
#include "trie.h"
#include <stdlib.h>

void ReadCampo(char **p, char *campo){

    int i = 0;
    int aspas = 0;

    if(**p == '"'){
        aspas = 1;
        (*p)++;
    }

    while(**p){

        if(aspas){

            if(**p == '"' &&
              ((*p)[1] == ',' || (*p)[1] == '\n' || (*p)[1] == '\0')){

                (*p)++;
                break;
            }

        }else{

            if(**p == ',' || **p == '\n' || **p == '\0')
                break;
        }

        campo[i++] = **p;
        (*p)++;
    }

    campo[i] = '\0';

    if(**p == ',')
        (*p)++;
}

Trie *TrieCreate(FILE *fp){

    Trie *newTrie = malloc(sizeof(Trie));
    newTrie->root = NewNode();

    char linha[MAX_LINHA];

    while(fgets(linha, sizeof(linha), fp)){

        char codigo[14];
        char nome[MAX_TEXT];
        char paises[MAX_TEXT];
        char labels[MAX_TEXT];

        char *p = linha;

        ReadCampo(&p, codigo);
        ReadCampo(&p, nome);
        ReadCampo(&p, paises);
        ReadCampo(&p, labels);

        TrieInsert(newTrie, codigo, nome, paises, labels);
    }

    return newTrie;
}