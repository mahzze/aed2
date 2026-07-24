#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "DataBase.hpp"


///  CABEÇALHO  .................................

// Modelo genérico para uma Trie
//  = árvore digital com referências ao tipo D (ex.: Product), com aridade M (ex.: 2, para binária)
//   obs.1: assume-se que elementos do tipo D possuem chaves adequadas (campo público 'key', do tipo string)
//   obs.2: assume-se ainda que os alfabetos seguem sempre o formato {'0', '1', '2, ..., 'M-1'}
template <typename D, size_t M>
class Trie
{
		// Estrutura de um nó dessa Trie
		struct TrieNode
		{
				friend class Trie;

			private:

				TrieNode * children[M] {};  // array de ponteiros para os TrieNode's filhos, todos inicializados como 'nullptr'
				D * data {};  // ponteiro para um dado do tipo D, inicializado como 'nullptr'
							  //  assim, se 'data' != 'nullptr', este é um nó terminal

				// Construtor e destrutor (inicialização e gerenciamento de recursos para os TrieNode's)
				TrieNode() = default;
				~TrieNode()
				{
					for (auto child : this->children) { delete child; }
				}

				// Declaração dos métodos de TrieNode
				bool isLeaf() const;
		};

	private:

		TrieNode * root = new TrieNode();  // nó raiz, inicializado em alocação dinâmica

	public:

		// Construtor e destrutor (inicialização e gerenciamento de recursos para a Trie)
		Trie() = default;
		Trie(DataBase<D> & dataBase)  // construir a partir de uma DataBase, sem realizar conversão de chaves
		{
			this->constructFromDataBase(dataBase);
		}
		Trie(DataBase<D> & dataBase, std::string (*conversion)(const std::string &))  // construir a partir de uma DataBase
																						//  aplicando 'conversion' para conversão das chaves
		{
			this->constructFromDataBase(dataBase, conversion);
		}
		~Trie()
		{
			delete this->root;
		}

		// Declaração dos métodos de construção de Tries a partir de bases de dados
		void constructFromDataBase(DataBase<D> & dataBase);
		void constructFromDataBase(DataBase<D> & dataBase, std::string (*conversion)(const std::string &));  // recebe um ponteiro para função de conversão

		// Declaração dos métodos específicos de Trie
		bool isEmpty() const;
		bool insert(D * data, const std::string & key = "");  // retorna 'true' se inserido com sucesso; 'false' caso já esteja na árvore (ou erro)
		D * search(const std::string & key) const;  // retorna 'nullptr' caso não exista na árvore (ou erro)
		std::vector<D *> searchPrefix(const std::string & prefix) const;  // retorna um std::vector vazio caso não exista na árvore (ou erro)
		bool remove(const std::string & key);  // retorna 'true' se removido com sucesso; 'false' caso não exista na árvore (ou erro)
		bool clear();  // retorna 'true' se limpa com sucesso; 'false' caso não há nada a remover (ou erro)
};


///  IMPLEMENTAÇÃO  .............................

// Definição dos métodos de TrieNode
template <typename D, size_t M>
bool Trie<D, M>::TrieNode::isLeaf() const
{
	for (auto child : children)
		if (child)
		{
			return false;
		}

	return true;
}

// Definição dos métodos de construção de Tries a partir de bases de dados
template <typename D, size_t M>
void Trie<D, M>::constructFromDataBase(DataBase<D> & dataBase)
{
	this->clear();

	for (auto & data : dataBase)
	{
		this->insert(&data);
	}
}
template <typename D, size_t M>
void Trie<D, M>::constructFromDataBase(DataBase<D> & dataBase, std::string (*conversion)(const std::string &))
{
	this->clear();

	for (auto & data : dataBase)
	{
		this->insert(&data, conversion(data.key));
	}
}

// Definição dos métodos específicos de Trie
template <typename D, size_t M>
bool Trie<D, M>::isEmpty() const
{
	return (not root->data) and root->isLeaf();
}
template <typename D, size_t M>
bool Trie<D, M>::insert(D * data, const std::string & key)
{
	if (not data) { return false; }
	
	const std::string & insertKey = (key.empty()) ? data->key : key;  // se a chave passada for vazia (caso padrão), considera-se automaticamente a 'key' de 'data'

	// Encontrar/construir o nó final para o dado
	TrieNode * curr = this->root;
	for (char digit : insertKey)  // <!> aqui, assume-se que 'data' possui o atributo 'key' do tipo std::string
	{
		size_t index = digit - '0';  // <!> aqui, assume-se que o alfabeto é da forma {'0', '1', '2, ..., 'M-1'}
		if (index >= M) { return false; }  // dígito inválido

		if (not curr->children[index])  // não tem o filho certo (ele, então, é criado)
		{
			curr->children[index] = new TrieNode();
		}

		curr = curr->children[index];  // avança para o próximo nó
	}

	// ... 'curr' é o nó terminal correspondente à chave ...

	if (curr->data)  // o dado já existe na árvore
	{
		return false;
	}

	curr->data = data;
	return true;
}
template <typename D, size_t M>
D * Trie<D, M>::search(const std::string & key) const
{
	// Encontrar o nó final da chave
	TrieNode * curr = this->root;
	for (char digit : key)
	{
		size_t index = digit - '0';  // <!> aqui, assume-se que o alfabeto é da forma {'0', '1', '2, ..., 'M-1'}
		if (index >= M) { return nullptr; }  // dígito inválido

		if (not curr->children[index]) { return nullptr; }  // não tem o filho certo (busca interrompida)

		curr = curr->children[index];  // avança para o próximo nó
	}

	// ... 'curr' é o nó terminal correspondente à chave ...

	return curr->data;
}
template <typename D, size_t M>
std::vector<D *> Trie<D, M>::searchPrefix(const std::string & prefix) const
{
	// Encontrar o nó final do prefixo
	TrieNode * curr = this->root;
	for (char digit : prefix)
	{
		size_t index = digit - '0';  // <!> aqui, assume-se que o alfabeto é da forma {'0', '1', '2, ..., 'M-1'}
		if (index >= M) { return {}; }  // dígito inválido

		if (not curr->children[index]) { return {}; }  // não tem o filho certo (busca interrompida)

		curr = curr->children[index];  // avança para o próximo nó
	}

	// ... 'curr' é o nó terminal correspondente ao prefixo ...

	std::vector<D *> result;

	// Lambda recursiva para coletar todos os dados válidos a partir de 'curr' e inseri-los em 'result' (usando DFS)
	auto collectData = [&result] (this auto self, TrieNode * node)
		{
			if (not node) { return; }

			if (node->data)
			{
				result.push_back(node->data);
			}

			for (auto child : node->children)
			{
				self(child);
			}
		};

	collectData(curr);

	return result;
}
template <typename D, size_t M>
bool Trie<D, M>::remove(const std::string & key)
{
	bool removed = false;

	TrieNode * root = this->root;
	// Lambda recursiva para remover o nó correspondente a 'key' e todos aqueles que não farão mais sentido na árvore
	auto remove_rec = [&key, &removed, root] (this auto self, TrieNode *& node, size_t depth)
		{
			// "Descer", procurando anular o dado do nó terminal correto
			if (not node) { return; }  // não encontrou o filho adequado (nada a ser removido)
			if (depth < key.size())  // a leitura da chave não acabou
			{
				size_t index = key[depth] - '0';  // <!> aqui, assume-se que o alfabeto é da forma {'0', '1', '2, ..., 'M-1'}
				if (index >= M) { return; }  // dígito inválido

				self(node->children[index], depth + 1);  // "avança": chama a recursão para o próximo filho

				if (not removed) { return; }  // nada foi feito mesmo após ler a chave toda (nada a ser removido)
			}
			else  // a leitura da chave terminou (não há mais o que avançar)
			{
				// ... 'node' é o nó terminal referente a 'key' ...

				if (not node->data) { return; }  // não há dado a anular (nada a ser removido)

				node->data = nullptr;
				removed = true;
			}

			// "Subir", liberando todos os nós inúteis
			if ((node != root) and (not node->data) and (node->isLeaf()))
			{
				delete node;
				node = nullptr;
			}
		};

	remove_rec(this->root, 0);

	return removed;
}
template <typename D, size_t M>
bool Trie<D, M>::clear()
{
	if (this->isEmpty()) { return false; }

	delete this->root;
	this->root = new TrieNode();

	return true;
}
