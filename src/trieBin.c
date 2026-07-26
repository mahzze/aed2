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
                const char *brand){

    char bits[53];
    codigoParaBits(codigo, bits);

    No *atual = raiz->root;

    for(int i = 0; i < 52; i++){

        int indice = bits[i] - '0';

        if(atual->filho[indice] == NULL)
            atual->filho[indice] = NewNode();

        atual = atual->filho[indice];
    }

    atual->produto = NewProduto(nome, paises, brand);
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

void TrieDeleteBinRec(No *raiz){

    if(raiz == NULL)
        return;

    for(int i = 0; i < M; i++)
        TrieDeleteBinRec(raiz->filho[i]);

    free(raiz->produto);
    free(raiz);
}

void TrieDeleteBin(Trie *raiz){

    if(raiz == NULL)
        return;

    TrieDeleteBinRec(raiz->root);

    free(raiz);
}

static int NoEhFolhaBin(No *no){

    for(int i = 0; i < M; i++)
        if(no->filho[i] != NULL)
            return 0;

    return 1;
}

static int NoVazioBin(No *no){

    return (no->produto == NULL && NoEhFolhaBin(no));
}

static int TrieRemoveRecBin(No *no,
                            const char *bits,
                            int nivel,
                            Produto **produtoRemovido){

    if(no == NULL)
        return 0;

    if(nivel == 52){

        if(no->produto == NULL)
            return 0;

        *produtoRemovido = no->produto;
        no->produto = NULL;

        return NoEhFolhaBin(no);
    }

    int indice = bits[nivel] - '0';

    int apagarFilho = TrieRemoveRecBin(no->filho[indice],
                                       bits,
                                       nivel + 1,
                                       produtoRemovido);

    if(apagarFilho){

        free(no->filho[indice]);
        no->filho[indice] = NULL;
    }

    return NoVazioBin(no);
}

Produto *TrieRemoveBin(Trie *raiz, const char *codigo){

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

    char bits[53];
    codigoParaBits(codigo, bits);

    Produto *produto = NULL;

    TrieRemoveRecBin(raiz->root,
                     bits,
                     0,
                     &produto);

    return produto;
}

static void prefixoParaBits(const char *prefixo, char *bits){

    int k = 0;

    for(int i = 0; prefixo[i] != '\0'; i++){

        int d = prefixo[i] - '0';

        for(int b = 3; b >= 0; b--)
            bits[k++] = ((d >> b) & 1) + '0';
    }

    bits[k] = '\0';
}

int TriePrefixosBin(Trie *raiz,
                    const char *prefixo,
                    int *digitos){

    char bits[53];
    prefixoParaBits(prefixo, bits);

    No *atual = raiz->root;

    for(int i = 0; bits[i] != '\0'; i++){

        int indice = bits[i] - '0';

        if(atual->filho[indice] == NULL)
            return 0;

        atual = atual->filho[indice];
    }

    int qtd = 0;

    for(int d = 0; d < 10; d++){

        No *aux = atual;
        int existe = 1;

        for(int b = 3; b >= 0; b--){

            int bit = (d >> b) & 1;

            if(aux->filho[bit] == NULL){
                existe = 0;
                break;
            }

            aux = aux->filho[bit];
        }

        if(existe)
            digitos[qtd++] = d;
    }

    return qtd;
}


static int ContaProdutosBin(No *no){

    if(no == NULL)
        return 0;

    int total = (no->produto != NULL);

    for(int i = 0; i < M; i++)
        total += ContaProdutosBin(no->filho[i]);

    return total;
}

int TrieContaPrefixoBin(Trie *raiz,
                        const char *prefixo){

    char bits[53];
    prefixoParaBits(prefixo, bits);

    No *atual = raiz->root;

    for(int i = 0; bits[i] != '\0'; i++){

        int indice = bits[i] - '0';

        if(atual->filho[indice] == NULL)
            return 0;

        atual = atual->filho[indice];
    }

    return ContaProdutosBin(atual);
}

static int ContaNosBin(No *no){

    if(no == NULL)
        return 0;

    int total = 1;

    for(int i = 0; i < M; i++)
        total += ContaNosBin(no->filho[i]);

    return total;
}

size_t TrieMemoriaBin(Trie *raiz, int *quantidadeNos){

    if(raiz == NULL){

        if(quantidadeNos != NULL)
            *quantidadeNos = 0;

        return 0;
    }

    int nos = ContaNosBin(raiz->root);

    if(quantidadeNos != NULL)
        *quantidadeNos = nos;

    return (size_t)nos * sizeof(No);
}