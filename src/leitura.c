#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "./trie.h"

#define STRINGIFY(X) #X
#define TO_STRING(X) STRINGIFY(X)

#define CODE_SIZE 13
#define BINARY_CODE_SIZE (4 * CODE_SIZE)


bool createBinaryCode(const char * code, char * binaryCode);
void testBinaryTrie(Trie * binTrie);


int main(void)
{
	FILE * fin = fopen("../../../data/ean13_dataset.txt", "r");
	if (!fin)
	{
		perror("ERROR: could not open the file");
		return 1;
	}

	Trie * binTrie = trie_create();

	char code[CODE_SIZE + 1];
	char binaryCode[BINARY_CODE_SIZE + 1];

    int countAll = 0;
    int countInserted = 0;
	while (fscanf(fin, "%" TO_STRING(CODE_SIZE) "s", code) == 1)
	{
		if (createBinaryCode(code, binaryCode))
		{
			trie_insert(binTrie, binaryCode);
            countInserted++;
		}
        countAll++;
	}
    printf("Total codes read: %d\n", countAll);
    printf("Total codes successfully inserted: %d\n", countInserted);

	fclose(fin);

    testBinaryTrie(binTrie);
	trie_destroy(binTrie);

	return 0;
}


bool createBinaryCode(const char * code, char * binaryCode)
{
	while (code[0])
	{
		int digit = code[0] - '0';
		if (digit < 0 || digit > 9)
        {
            binaryCode[0] = '\0';
            return false;
        }

		for (int bit = 3; bit >= 0; bit--)
		{
			binaryCode[0] = (digit & (1 << bit)) ? '1' : '0';
			binaryCode++;
		}

		code++;
	}

	binaryCode[0] = '\0';
	return true;
}
void testBinaryTrie(Trie * binTrie)
{
    char buffer[BINARY_CODE_SIZE + 1];

    printf("\n========== TESTES NA TRIE BINÁRIA ==========\n");


    printf("\n===== BUSCAS POR CÓDIGOS EXISTENTES =====\n");

    createBinaryCode("6111035000430", buffer);
    printf("6111035000430 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("5449000147417", buffer);
    printf("5449000147417 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("7622210449283", buffer);
    printf("7622210449283 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("8052575090230", buffer);
    printf("8052575090230 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("7300400481588", buffer);
    printf("7300400481588 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");


    printf("\n===== BUSCAS POR CÓDIGOS INEXISTENTES =====\n");

    createBinaryCode("6111035000431", buffer);
    printf("6111035000431 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("5449000147418", buffer);
    printf("5449000147418 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("1111111111111", buffer);
    printf("1111111111111 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");

    createBinaryCode("9999999999999", buffer);
    printf("9999999999999 : %s\n", trie_search(binTrie, buffer) ? "true" : "false");


    printf("\n===== BUSCAS POR PREFIXOS EXISTENTES =====\n");

    createBinaryCode("6111", buffer);
    printf("\"6111\" : %s\n", trie_searchPrefix(binTrie, buffer) ? "true" : "false");

    createBinaryCode("5449", buffer);
    printf("\"5449\" : %s\n", trie_searchPrefix(binTrie, buffer) ? "true" : "false");

    createBinaryCode("7622", buffer);
    printf("\"7622\" : %s\n", trie_searchPrefix(binTrie, buffer) ? "true" : "false");

    createBinaryCode("7300", buffer);
    printf("\"7300\" : %s\n", trie_searchPrefix(binTrie, buffer) ? "true" : "false");


    printf("\n===== BUSCAS POR PREFIXOS INEXISTENTES =====\n");

    createBinaryCode("9999", buffer);
    printf("\"9999\" : %s\n", trie_searchPrefix(binTrie, buffer) ? "true" : "false");

    createBinaryCode("0000", buffer);
    printf("\"0000\" : %s\n", trie_searchPrefix(binTrie, buffer) ? "true" : "false");


    printf("\n===== TESTES DE ENTRADAS =====\n");

    printf("\"6111035000430\"  : %s\n", createBinaryCode("6111035000430", buffer) ? "valid" : "invalid");
    printf("\"5449000147418\"  : %s\n", createBinaryCode("5449000147418", buffer) ? "valid" : "invalid");
    
    printf("\"ABC\"            : %s\n", createBinaryCode("ABC", buffer) ? "valid" : "invalid");
    printf("\"61110A5000430\"  : %s\n", createBinaryCode("61110A5000430", buffer) ? "valid" : "invalid");
    printf("\"61-1035000430\"  : %s\n", createBinaryCode("61-1035000430", buffer) ? "valid" : "invalid");
}
