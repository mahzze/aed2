#include <stdlib.h>
#include <string.h>

#include "produto.h"

Produto *NewProduto(const char *nome,
                    const char *paises,
                    const char *labels){

    Produto *p = malloc(sizeof(Produto));

    strcpy(p->nome, nome);
    strcpy(p->paises, paises);
    strcpy(p->labels, labels);

    return p;
}