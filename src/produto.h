#ifndef PRODUTO_H
#define PRODUTO_H

#define MAX_TEXT 200

typedef struct Produto{
    char nome[MAX_TEXT];
    char paises[MAX_TEXT];
    char labels[MAX_TEXT];
} Produto;

Produto *NewProduto(const char *nome,
                    const char *paises,
                    const char *labels);

#endif