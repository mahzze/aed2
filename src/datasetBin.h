#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>

#include "trieBin.h"

#define MAX_LINHA 400

void ReadCampo(char **p, char *campo);
Trie *TrieCreateBin(FILE *fp);

#endif