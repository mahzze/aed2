#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 10
#define MAX_LINHA 400
#define MAX_TEXT 200

typedef struct Produto{
    char nome[MAX_TEXT];
    char paises[MAX_TEXT];
    char labels[MAX_TEXT];
} Produto;

typedef struct No{
    struct No *filho[M];
    Produto *produto;
} No;


No *NewNode(){

    No *novo = malloc(sizeof(No));

    for(int i = 0; i < M; i++)
        novo->filho[i] = NULL;

    novo->produto = NULL;

    return novo;
}



Produto *NewProduto(const char *nome, const char *paises, const char *labels){

    Produto *p = malloc(sizeof(Produto));

    strcpy(p->nome, nome);
    strcpy(p->paises, paises);
    strcpy(p->labels, labels);

    return p;
}


/*le os dados do dataset passado*/
void ReadCampo(char **p, char *campo){

    int i = 0;
    int aspas = 0;

    if(**p == '"'){
        aspas = 1;
        (*p)++;
    }

    while(**p){

        if(aspas){
            if(**p == '"' && ((*p)[1] == ',' || (*p)[1] == '\n' || (*p)[1] == '\0')){
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

/*insere nós na arvore*/
void TrieInsert(No *raiz, const char *codigo, const char *nome, const char *paises, const char *labels){

    No *atual = raiz;

    for(int i = 0; i < 13; i++){

        int indice = codigo[i] - '0';

        if(atual->filho[indice] == NULL)
            atual->filho[indice] = NewNode();

        atual = atual->filho[indice];
    }

    atual->produto = NewProduto(nome, paises, labels);
}


No *TrieCreate(FILE *fp){

    No *raiz = NewNode();

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

        TrieInsert(raiz, codigo, nome, paises, labels);
    }

    return raiz;
}


Produto *TrieSearch(No *raiz, const char *codigo){

    No *atual = raiz;

    for(int i = 0; i < 13; i++){

        int indice = codigo[i] - '0';

        if(indice < 0 || indice > 9) return NULL;

        if(atual->filho[indice] == NULL) return NULL;

        atual = atual->filho[indice];
    }

    return atual->produto;
}


void TrieDelete(No *raiz){

    if(raiz == NULL) return;

    for(int i = 0; i < M; i++) TrieDelete(raiz->filho[i]);

    if(raiz->produto != NULL) free(raiz->produto);

    free(raiz);
}



int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Uso: %s arquivo.txt\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if(fp == NULL){
        perror("Erro ao abrir arquivo");
        return 1;
    }

    No *raiz = TrieCreate(fp);

    fclose(fp);

    char codigo[14];

    while(1){

        printf("\nDigite um codigo de 13 digitos (0 para sair): ");
        scanf("%13s", codigo);

        if(strcmp(codigo, "0") == 0)
            break;

        Produto *p = TrieSearch(raiz, codigo);

        if(p == NULL){

            printf("\nProduto nao encontrado.\n");

        }else{

            printf("\nProduto encontrado\n");
            printf("Nome    : %s\n", p->nome);
            printf("Paises  : %s\n", p->paises);
            printf("Labels  : %s\n", p->labels);

        }
    }

    TrieDelete(raiz);

    return 0;
}