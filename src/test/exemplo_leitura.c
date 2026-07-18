#include <stdio.h>
#include <string.h>

#define TAM_CODIGO 13

int main() {
    FILE *f = fopen("../../data/ean13_dataset.txt", "r");
    if (!f) {
        perror("erro ao abrir arquivo");
        return 1;
    }

    char code[TAM_CODIGO + 1]; // 13 dígitos + '\0'
    int count = 0;

    while (fscanf(f, "%13s", code) == 1) {
        // inserir 'code' na árvore digital (binária e m-ária)
        // ex.: arvore_binaria_inserir(raiz_bin, code);
        //      arvore_m4_inserir(raiz_m4, code);
        count++;
    }

    printf("Total de códigos lidos: %d\n", count);

    fclose(f);
    return 0;
}