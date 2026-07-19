#pragma once

#include <cstddef>
#include <string>

#define ALPHABET_SIZE 2  // {'0', '1'}


struct TrieNode
{
		friend class Trie;

	private:

		TrieNode * children[ALPHABET_SIZE] {};
		bool isEnd = false;

		TrieNode() = default;
		~TrieNode()
		{
			for (auto child : this->children)
			{
				delete child;
			}
		}
};

class Trie
{
	private:

		TrieNode * root = new TrieNode();

	public:

		Trie() = default;
		~Trie()
		{
			delete this->root;
		}

		Trie(const Trie & trie) = delete;
		Trie & operator = (const Trie & trie) = delete;

		void insert(const std::string & key);
		bool search(const std::string & key) const;
		bool searchPrefix(const std::string & prefix) const;
};
