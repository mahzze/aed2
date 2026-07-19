#include <iostream>
#include "Trie.h"


int main()
{
    std::cout << std::boolalpha;

    Trie trie;

    trie.insert("0");
    trie.insert("1");
    trie.insert("00");
    trie.insert("01");
    trie.insert("10");
    trie.insert("11");
    trie.insert("010");
    trie.insert("0101");
    trie.insert("0110");
    trie.insert("1010");
    trie.insert("111111");

    std::cout << "===== BUSCAS POR CHAVES EXISTENTES =====\n";

    std::cout << "0       : " << trie.search("0") << '\n';
    std::cout << "1       : " << trie.search("1") << '\n';
    std::cout << "00      : " << trie.search("00") << '\n';
    std::cout << "01      : " << trie.search("01") << '\n';
    std::cout << "010     : " << trie.search("010") << '\n';
    std::cout << "0101    : " << trie.search("0101") << '\n';
    std::cout << "0110    : " << trie.search("0110") << '\n';
    std::cout << "1010    : " << trie.search("1010") << '\n';
    std::cout << "111111  : " << trie.search("111111") << '\n';

    std::cout << "\n===== BUSCAS POR CHAVES INEXISTENTES =====\n";

    std::cout << "001     : " << trie.search("001") << '\n';
    std::cout << "10101   : " << trie.search("10101") << '\n';
    std::cout << "11111   : " << trie.search("11111") << '\n';
    std::cout << "1001    : " << trie.search("1001") << '\n';
    std::cout << "0111    : " << trie.search("0111") << '\n';

    std::cout << "\n===== BUSCAS POR PREFIXOS EXISTENTES =====\n";

    std::cout << "\"0\"      : " << trie.searchPrefix("0") << '\n';
    std::cout << "\"01\"     : " << trie.searchPrefix("01") << '\n';
    std::cout << "\"010\"    : " << trie.searchPrefix("010") << '\n';
    std::cout << "\"011\"    : " << trie.searchPrefix("011") << '\n';
    std::cout << "\"1\"      : " << trie.searchPrefix("1") << '\n';
    std::cout << "\"10\"     : " << trie.searchPrefix("10") << '\n';
    std::cout << "\"11\"     : " << trie.searchPrefix("11") << '\n';

    std::cout << "\n===== BUSCAS POR PREFIXOS INEXISTENTES =====\n";

    std::cout << "\"0011\"   : " << trie.searchPrefix("0011") << '\n';
    std::cout << "\"1100\"   : " << trie.searchPrefix("1100") << '\n';
    std::cout << "\"1011\"   : " << trie.searchPrefix("1011") << '\n';

    std::cout << "\n===== BUSCAS POR CHAVE/PREFIXO VAZIA/O =====\n";

    std::cout << "search(\"\")       : " << trie.search("") << '\n';
    std::cout << "searchPrefix(\"\") : " << trie.searchPrefix("") << '\n';

    std::cout << "\n===== INSERÇÃO DUPLICADA, SEGUIDA DE BUSCA =====\n";

    trie.insert("0101");
    trie.insert("0101");
    trie.insert("0101");
    std::cout << "0101 : " << trie.search("0101") << '\n';

    std::cout << "\n===== BUSCAS COM ENTRADAS INVÁLIDAS =====\n";

    std::cout << "\"2\"      : " << trie.search("2") << '\n';
    std::cout << "\"012\"    : " << trie.search("012") << '\n';
    std::cout << "\"abc\"    : " << trie.search("abc") << '\n';
    std::cout << "\"10a01\"  : " << trie.search("10a01") << '\n';

    return 0;
}
