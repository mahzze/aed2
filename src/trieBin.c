#include <stdlib.h>

#include "trie.h"
#include "produto.h"

void codigoParaBits(const char *codigo, char bits[53]){

    int k = 0;

    for(int i = 0; i < 13; i++){

        int d = codigo[i] - '0';

        for(int b = 3; b >= 0; b--)
            bits[k++] = ((d >> b) & 1) + '0';
    }

    bits[k] = '\0';
}

No *NewNode(){

    No *novo = malloc(sizeof(No));

    for(int i = 0; i < M; i++)
        novo->filho[i] = NULL;

    novo->produto = NULL;

    return novo;
}

void TrieInsertBin(Trie *raiz,
                const char *codigo,
                const char *nome,
                const char *paises,
                const char *labels){

    char bits[53];
    codigoParaBits(codigo, bits);

    No *atual = raiz->root;

    for(int i = 0; i < 52; i++){

        int indice = bits[i] - '0';

        if(atual->filho[indice] == NULL)
            atual->filho[indice] = NewNode();

        atual = atual->filho[indice];
    }

    atual->produto = NewProduto(nome, paises, labels);
}

Produto *TrieSearchBin(Trie *raiz, const char *codigo){

    char bits[53];
    codigoParaBits(codigo, bits);

    No *atual = raiz->root;

    for(int i = 0; i < 52; i++){

        int indice = bits[i] - '0';

        if(atual->filho[indice] == NULL)
            return NULL;

        atual = atual->filho[indice];
    }

    return atual->produto;
}

void TrieDeleteBin(No *raiz){

    if(raiz == NULL)
        return;

    for(int i = 0; i < M; i++)
        TrieDeleteBin(raiz->filho[i]);

    free(raiz->produto);
    free(raiz);
}