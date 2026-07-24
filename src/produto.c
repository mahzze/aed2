#include <stdlib.h>
#include <string.h>

#include "produto.h"

Produto *NewProduto(const char *nome,
                    const char *paises,
                    const char *brand){

    Produto *p = malloc(sizeof(Produto));

    strcpy(p->nome, nome);
    strcpy(p->paises, paises);
    strcpy(p->brand, brand);

    return p;
}