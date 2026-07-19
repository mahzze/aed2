#include <iostream>
#include <fstream>
#include <string>
#include "../exemplos_implementacao/exemplo_implementacao_cpp/Trie.h"


std::string binaryCode(const std::string & code);
void testBinaryTrie(const Trie & binTrie);


int main()
{
    std::cout << std::boolalpha;

	std::ifstream fin("../../../data/ean13_dataset.txt");
	if (!fin)
	{
		std::cerr << "ERROR: could not open the file\n";
		return 1;
	}

	Trie binTrie;

	std::string code;

	size_t countAll = 0;
	size_t countInserted = 0;
	while (fin >> code)
	{
        std::string binCode = binaryCode(code);
		if (!binCode.empty())
		{
			binTrie.insert(binCode);
			countInserted++;
		}
		countAll++;
	}
	std::cout << "Total codes read: " << countAll << "\n";
	std::cout << "Total codes successfully inserted: " << countInserted << "\n";

	testBinaryTrie(binTrie);

	return 0;
}


std::string binaryCode(const std::string & code)
{
	std::string result {};
    result.reserve(4 * code.size());

	for (char c : code)
	{
		int digit = c - '0';
		if (digit < 0 or digit > 9)
		{
            return {};
		}

		for (int bit = 3; bit >= 0; bit--)
		{
			result += (digit & (1 << bit)) ? '1' : '0';
		}
	}

	return result;
}
void testBinaryTrie(const Trie & binTrie)
{
    std::cout << "\n========== TESTES NA TRIE BINÁRIA ==========\n";


    std::cout << "\n===== BUSCAS POR CÓDIGOS EXISTENTES =====\n";

    std::cout << "6111035000430 : "
        << binTrie.search(binaryCode("6111035000430")) << '\n';

    std::cout << "5449000147417 : "
        << binTrie.search(binaryCode("5449000147417")) << '\n';

    std::cout << "7622210449283 : "
        << binTrie.search(binaryCode("7622210449283")) << '\n';

    std::cout << "8052575090230 : "
        << binTrie.search(binaryCode("8052575090230")) << '\n';

    std::cout << "7300400481588 : "
        << binTrie.search(binaryCode("7300400481588")) << '\n';


    std::cout << "\n===== BUSCAS POR CÓDIGOS INEXISTENTES =====\n";

    std::cout << "6111035000431 : "
        << binTrie.search(binaryCode("6111035000431")) << '\n';

    std::cout << "5449000147418 : "
        << binTrie.search(binaryCode("5449000147418")) << '\n';

    std::cout << "1111111111111 : "
        << binTrie.search(binaryCode("1111111111111")) << '\n';

    std::cout << "9999999999999 : "
        << binTrie.search(binaryCode("9999999999999")) << '\n';


    std::cout << "\n===== BUSCAS POR PREFIXOS EXISTENTES =====\n";

    std::cout << "\"6111\" : "
        << binTrie.searchPrefix(binaryCode("6111")) << '\n';

    std::cout << "\"5449\" : "
        << binTrie.searchPrefix(binaryCode("5449")) << '\n';

    std::cout << "\"7622\" : "
        << binTrie.searchPrefix(binaryCode("7622")) << '\n';

    std::cout << "\"7300\" : "
        << binTrie.searchPrefix(binaryCode("7300")) << '\n';


    std::cout << "\n===== BUSCAS POR PREFIXOS INEXISTENTES =====\n";

    std::cout << "\"9999\" : "
        << binTrie.searchPrefix(binaryCode("9999")) << '\n';

    std::cout << "\"0000\" : "
        << binTrie.searchPrefix(binaryCode("0000")) << '\n';


    std::cout << "\n===== TESTES DE ENTRADAS =====\n";

    std::cout << "\"6111035000430\"  : "
        << ((!binaryCode("6111035000430").empty()) ? "valid" : "invalid") << '\n';

    std::cout << "\"5449000147418\"  : "
        << ((!binaryCode("5449000147418").empty()) ? "valid" : "invalid") << '\n';

    std::cout << "\"ABC\"            : "
        << ((!binaryCode("ABC").empty()) ? "valid" : "invalid") << '\n';

    std::cout << "\"61110A5000430\"  : "
        << ((!binaryCode("61110A5000430").empty()) ? "valid" : "invalid") << '\n';

    std::cout << "\"61-1035000430\"  : "
        << ((!binaryCode("61-1035000430").empty()) ? "valid" : "invalid") << '\n';
}
