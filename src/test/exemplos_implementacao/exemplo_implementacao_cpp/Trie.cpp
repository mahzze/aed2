#include "Trie.h"


void Trie::insert(const std::string & key)
{
	TrieNode * curr = this->root;
	for (char digit : key)
	{
		size_t index = digit - '0';
		if (index < 0 or index >= ALPHABET_SIZE) { return; }

		if (curr->children[index] == nullptr)
		{
			curr->children[index] = new TrieNode();
		}

		curr = curr->children[index];
	}

	curr->isEnd = true;
}
bool Trie::search(const std::string & key) const
{
	TrieNode * curr = this->root;
	for (char digit : key)
	{
		size_t index = digit - '0';
		if (index < 0 or index >= ALPHABET_SIZE) { return false; }

		if (curr->children[index] == nullptr)
		{
			return false;
		}

		curr = curr->children[index];
	}

	return curr->isEnd;
}
bool Trie::searchPrefix(const std::string & prefix) const
{
	TrieNode * curr = this->root;
	for (char digit : prefix)
	{
		size_t index = digit - '0';
		if (index < 0 or index >= ALPHABET_SIZE) { return false; }

		if (curr->children[index] == nullptr)
		{
			return false;
		}

		curr = curr->children[index];
	}

	return true;
}
