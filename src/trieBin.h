#ifndef TRIE_H
#define TRIE_H

#include "produto.h"

#define M 2

typedef struct No{
    struct No *filho[M];
    Produto *produto;
} No;

typedef struct Trie{
    No *root;
} Trie;

void codigoParaBits(const char *codigo, char bits[53]);

No *NewNode(void);

void TrieInsertBin(Trie *raiz,
                const char *codigo,
                const char *nome,
                const char *paises,
                const char *brand);

Produto *TrieSearchBin(Trie *raiz,
                       const char *codigo);

void TrieDeleteBin(Trie *raiz);

Produto *TrieRemoveBin(Trie *raiz, const char *codigo);

int TriePrefixosBin(Trie *raiz,
                    const char *prefixo,
                    int *digitos);

int TrieContaPrefixoBin(Trie *raiz,
                        const char *prefixo);

size_t TrieMemoriaBin(Trie *raiz, int *quantidadeNos);

#endif