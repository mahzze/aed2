#include "Trie.h"
#include <stdlib.h>

#define ALPHABET_SIZE 2  // {'0', '1'}


typedef struct TrieNode
{
	struct TrieNode * children[ALPHABET_SIZE];
	bool isEnd;
} TrieNode;

struct Trie
{
	TrieNode * root;
};


static TrieNode * node_create(void)
{
	TrieNode * newNode = malloc(sizeof(TrieNode));
	if (!newNode) { return NULL; }

	newNode->isEnd = false;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		newNode->children[i] = NULL;
	}

	return newNode;
}
static void node_destroy(TrieNode * node)
{
	if (!node) { return; }

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		node_destroy(node->children[i]);
	}

	free(node);
}


Trie * trie_create(void)
{
	Trie * newTrie = malloc(sizeof(Trie));
	if (!newTrie) { return NULL; }

	newTrie->root = node_create();
	if (!newTrie->root) { free(newTrie); return NULL; }

	return newTrie;
}
void trie_destroy(Trie * trie)
{
	if (!trie) { return; }

	node_destroy(trie->root);
	free(trie);
}

void trie_insert(Trie * trie, string key)
{
	TrieNode * curr = trie->root;
	while (key[0])
	{
		int index = key[0] - '0';
		if (index < 0 || index >= ALPHABET_SIZE) { return; }

		if (curr->children[index] == NULL)
		{
			curr->children[index] = node_create();
		}

		curr = curr->children[index];
		key++;
	}

	curr->isEnd = true;
}
bool trie_search(const Trie * trie, string key)
{
	TrieNode * curr = trie->root;
	while (key[0])
	{
		int index = key[0] - '0';
		if (index < 0 || index >= ALPHABET_SIZE) { return false; }

		if (curr->children[index] == NULL)
		{
			return false;
		}

		curr = curr->children[index];
		key++;
	}

	return curr->isEnd;
}
bool trie_searchPrefix(const Trie * trie, string prefix)
{
	TrieNode * curr = trie->root;
	while (prefix[0])
	{
		int index = prefix[0] - '0';
		if (index < 0 || index >= ALPHABET_SIZE) { return false; }

		if (curr->children[index] == NULL)
		{
			return false;
		}

		curr = curr->children[index];
		prefix++;
	}

	return true;
}
