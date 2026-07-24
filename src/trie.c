#include <stdlib.h>

#include "trie.h"
#include "produto.h"

No *NewNode(){

    No *novo = malloc(sizeof(No));

    for(int i = 0; i < M; i++)
        novo->filho[i] = NULL;

    novo->produto = NULL;

    return novo;
}

void TrieInsert(Trie *raiz,
                const char *codigo,
                const char *nome,
                const char *paises,
                const char *brand){

    No *atual = raiz->root;

    for(int i = 0; i < 13; i++){

        int indice = codigo[i] - '0';

        if(atual->filho[indice] == NULL)
            atual->filho[indice] = NewNode();

        atual = atual->filho[indice];
    }

    atual->produto = NewProduto(nome, paises, brand);
}

Produto *TrieSearch(Trie *raiz, const char *codigo){

    No *atual = raiz->root;

    for(int i = 0; i < 13; i++){

        int indice = codigo[i] - '0';

        if(indice < 0 || indice >= M)
            return NULL;

        if(atual->filho[indice] == NULL)
            return NULL;

        atual = atual->filho[indice];
    }

    return atual->produto;
}

void TrieDelete(No *raiz){

    if(raiz == NULL)
        return;

    for(int i = 0; i < M; i++)
        TrieDelete(raiz->filho[i]);

    free(raiz->produto);
    free(raiz);
}

/*=========================================================
    REMOÇÃO
=========================================================*/

static int NoEhFolha(No *no){

    for(int i = 0; i < M; i++)
        if(no->filho[i] != NULL)
            return 0;

    return 1;
}

static int NoVazio(No *no){

    return (no->produto == NULL && NoEhFolha(no));
}

static int TrieRemoveRec(No *no,
                         const char *codigo,
                         int nivel,
                         Produto **produtoRemovido){

    if(no == NULL)
        return 0;

    if(nivel == 13){

        if(no->produto == NULL)
            return 0;

        *produtoRemovido = no->produto;
        no->produto = NULL;

        return NoEhFolha(no);
    }

    int indice = codigo[nivel] - '0';

    int apagarFilho = TrieRemoveRec(no->filho[indice],
                                    codigo,
                                    nivel + 1,
                                    produtoRemovido);

    if(apagarFilho){

        free(no->filho[indice]);
        no->filho[indice] = NULL;
    }

    return NoVazio(no);
}

Produto *TrieRemove(Trie *raiz, const char *codigo){

    if(raiz == NULL)
        return NULL;

    for(int i = 0; i < 13; i++){

        if(codigo[i] == '\0')
            return NULL;

        if(codigo[i] < '0' || codigo[i] > '9')
            return NULL;
    }

    if(codigo[13] != '\0')
        return NULL;

    Produto *produto = NULL;

    TrieRemoveRec(raiz->root,
                  codigo,
                  0,
                  &produto);

    return produto;
}


int TriePrefixos(Trie *raiz,
                 const char *prefixo,
                 int *digitos){

    No *atual = raiz->root;

    for(int i = 0; prefixo[i] != '\0'; i++){

        int indice = prefixo[i] - '0';

        if(indice < 0 || indice >= M)
            return 0;

        if(atual->filho[indice] == NULL)
            return 0;

        atual = atual->filho[indice];
    }

    int qtd = 0;

    for(int i = 0; i < M; i++){

        if(atual->filho[i] != NULL)
            digitos[qtd++] = i;
    }

    return qtd;
}


static int ContaProdutos(No *no){

    if(no == NULL)
        return 0;

    int total = (no->produto != NULL);

    for(int i = 0; i < M; i++)
        total += ContaProdutos(no->filho[i]);

    return total;
}

int TrieContaPrefixo(Trie *raiz,
                     const char *prefixo){

    No *atual = raiz->root;

    for(int i = 0; prefixo[i] != '\0'; i++){

        int indice = prefixo[i] - '0';

        if(indice < 0 || indice >= M)
            return 0;

        if(atual->filho[indice] == NULL)
            return 0;

        atual = atual->filho[indice];
    }

    return ContaProdutos(atual);
}