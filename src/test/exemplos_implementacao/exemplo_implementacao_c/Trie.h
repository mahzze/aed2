#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>


typedef struct Trie Trie;
typedef const char * string;


Trie * trie_create();
void trie_destroy(Trie * trie);

void trie_insert(Trie * trie, string key);
bool trie_search(const Trie * trie, string key);
bool trie_searchPrefix(const Trie * trie, string prefix);



#endif
