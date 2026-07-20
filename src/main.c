#include "trie.h"
#include "trie.c"
#include "leitura.c"

// leitura dos dados e construcao da trie

// implementação de uma API de buscas na trie, recebendo inputs, e mostrando
// o output e o tempo necessario para encontrar cada registro. Talvez valha
// a pena colocar a API em arquivo proprio e depois integrar tudo, mas parece
// simples o bastante para deixar em um arquivo único.
// ideia das operaçoes: 
// 0 - busca de um código de barras especifico.
// 1 - filtragem por empresa 
// 2 - sair
int run = 1;
int operacao;

while (run) {
    scanf("%d", &operacao);

    switch (operação) {
        case 0:

            break;
        default:
            run = 0;
    }
}
