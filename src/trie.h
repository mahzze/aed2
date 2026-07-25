#ifndef TRIE_H
#define TRIE_H

#include "produto.h"

#define M 10

typedef struct No{
    struct No *filho[M];
    Produto *produto;
} No;

typedef struct Trie{
    No *root;
} Trie;

No *NewNode(void);

void TrieInsert(Trie *raiz,
                const char *codigo,
                const char *nome,
                const char *paises,
                const char *brand);

Produto *TrieSearch(Trie *raiz,
                    const char *codigo);

void TrieDelete(No *raiz);

Produto *TrieRemove(Trie *raiz, const char *codigo);

int TriePrefixos(Trie *raiz,
                 const char *prefixo,
                 int *digitos);

int TrieContaPrefixo(Trie *raiz,
                     const char *prefixo);

size_t TrieMemoria(Trie *raiz, int *quantidadeNos);

#endif