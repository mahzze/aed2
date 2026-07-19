#include <stdio.h>
#include "Trie.h"

int main(void)
{
    Trie * trie = trie_create();

    trie_insert(trie, "0");
    trie_insert(trie, "1");
    trie_insert(trie, "00");
    trie_insert(trie, "01");
    trie_insert(trie, "10");
    trie_insert(trie, "11");
    trie_insert(trie, "010");
    trie_insert(trie, "0101");
    trie_insert(trie, "0110");
    trie_insert(trie, "1010");
    trie_insert(trie, "111111");

    printf("===== BUSCAS POR CHAVES EXISTENTES =====\n");

    printf("0       : %s\n", trie_search(trie, "0") ? "true" : "false");
    printf("1       : %s\n", trie_search(trie, "1") ? "true" : "false");
    printf("00      : %s\n", trie_search(trie, "00") ? "true" : "false");
    printf("01      : %s\n", trie_search(trie, "01") ? "true" : "false");
    printf("010     : %s\n", trie_search(trie, "010") ? "true" : "false");
    printf("0101    : %s\n", trie_search(trie, "0101") ? "true" : "false");
    printf("0110    : %s\n", trie_search(trie, "0110") ? "true" : "false");
    printf("1010    : %s\n", trie_search(trie, "1010") ? "true" : "false");
    printf("111111  : %s\n", trie_search(trie, "111111") ? "true" : "false");

    printf("\n===== BUSCAS POR CHAVES INEXISTENTES =====\n");

    printf("001     : %s\n", trie_search(trie, "001") ? "true" : "false");
    printf("10101   : %s\n", trie_search(trie, "10101") ? "true" : "false");
    printf("11111   : %s\n", trie_search(trie, "11111") ? "true" : "false");
    printf("1001    : %s\n", trie_search(trie, "1001") ? "true" : "false");
    printf("0111    : %s\n", trie_search(trie, "0111") ? "true" : "false");

    printf("\n===== BUSCAS POR PREFIXOS EXISTENTES =====\n");

    printf("\"0\"      : %s\n", trie_searchPrefix(trie, "0") ? "true" : "false");
    printf("\"01\"     : %s\n", trie_searchPrefix(trie, "01") ? "true" : "false");
    printf("\"010\"    : %s\n", trie_searchPrefix(trie, "010") ? "true" : "false");
    printf("\"011\"    : %s\n", trie_searchPrefix(trie, "011") ? "true" : "false");
    printf("\"1\"      : %s\n", trie_searchPrefix(trie, "1") ? "true" : "false");
    printf("\"10\"     : %s\n", trie_searchPrefix(trie, "10") ? "true" : "false");
    printf("\"11\"     : %s\n", trie_searchPrefix(trie, "11") ? "true" : "false");

    printf("\n===== BUSCAS POR PREFIXOS INEXISTENTES =====\n");

    printf("\"0011\"   : %s\n", trie_searchPrefix(trie, "0011") ? "true" : "false");
    printf("\"1100\"   : %s\n", trie_searchPrefix(trie, "1100") ? "true" : "false");
    printf("\"1011\"   : %s\n", trie_searchPrefix(trie, "1011") ? "true" : "false");

    printf("\n===== BUSCAS POR CHAVE/PREFIXO VAZIA/O =====\n");

    printf("search(\"\")       : %s\n", trie_search(trie, "") ? "true" : "false");

    printf("searchPrefix(\"\") : %s\n", trie_searchPrefix(trie, "") ? "true" : "false");

    printf("\n===== INSERÇÃO DUPLICADA, SEGUIDA DE BUSCA =====\n");

    trie_insert(trie, "0101");
    trie_insert(trie, "0101");
    trie_insert(trie, "0101");

    printf("0101 : %s\n", trie_search(trie, "0101") ? "true" : "false");

    printf("\n===== BUSCAS COM ENTRADAS INVÁLIDAS =====\n");

    printf("\"2\"      : %s\n", trie_search(trie, "2") ? "true" : "false");
    printf("\"012\"    : %s\n", trie_search(trie, "012") ? "true" : "false");
    printf("\"abc\"    : %s\n", trie_search(trie, "abc") ? "true" : "false");
    printf("\"10a01\"  : %s\n", trie_search(trie, "10a01") ? "true" : "false");

    trie_destroy(trie);

    return 0;
}
