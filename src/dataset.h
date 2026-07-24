#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include "trie.h"

#define MAX_LINHA 400

Trie *TrieCreate(FILE *fp);

#endif