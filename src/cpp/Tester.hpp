#pragma once

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include <vector>
#include "DataBase.hpp"
#include "KeyConverter.hpp"
#include "Product.hpp"
#include "Trie.hpp"


///  CABEÇALHO  .................................

// Classe estática para testes em Tries
//  obs.: assume-se que a aridade das Tries testadas é 2, 4 ou 10 (tratamentos auxiliados por KeyConverter)
class Tester
{
	private:

		// Função auxiliar para exportação do resultado em CSVs
		static void exportCSVs(const std::pair<std::vector<std::vector<std::tuple<double, double, double>>>, std::vector<std::vector<std::tuple<double, double, double>>>> & result,
							   const std::vector<size_t> & amounts, const std::vector<size_t> & prefixAmounts, std::vector<DataBase<Product>> & dataBases);
	
	public:

		// Funções para o teste de uma Trie
		template <typename D, size_t M>
		static double testInsert(Trie<D, M> & trie, const std::vector<size_t> & indexes, DataBase<D> & fromDB);
		template <typename D, size_t M>
		static double testSearch(Trie<D, M> & trie, const std::vector<size_t> & indexes, DataBase<D> & fromDB, size_t repetitions = 1);
		template <typename D, size_t M>
		static double testSearchPrefix(Trie<D, M> & trie, size_t prefixSize, const std::vector<size_t> & ofIndexes, DataBase<D> & fromDB, size_t repetitions = 1);
		template <typename D, size_t M>
		static double testRemove(Trie<D, M> & trie, const std::vector<size_t> & indexes, DataBase<D> & fromDB);

		// Funções especializadas para o teste conjunto de 3 Tries: uma binária, uma quaternária e uma decimal
		template <typename D>
		static std::tuple<double, double, double> testInsert_3Tries(size_t amount, DataBase<D> & fromDB, size_t repetitions = 1);
		template <typename D>
		static std::tuple<double, double, double> testSearch_3Tries(size_t amount, DataBase<D> & fromDB, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::tuple<double, double, double> testSearchPrefix_3Tries(size_t prefixSize, size_t amount, DataBase<D> & fromDB, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::tuple<double, double, double> testRemove_3Tries(size_t amount, DataBase<D> & fromDB, size_t repetitions = 1);

		// Funções para testes triplos com diferentes quantidades
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testInsert_3Tries_variousAmounts(const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t repetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testSearch_3Tries_variousAmounts(const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testSearchPrefix_3Tries_variousAmounts(size_t prefixSize, const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testRemove_3Tries_variousAmounts(const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t repetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testInsert_3Tries_variousDataBases(size_t amount, std::vector<DataBase<D>> & fromDBs, size_t repetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testSearch_3Tries_variousDataBases(size_t amount, std::vector<DataBase<D>> & fromDBs, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testSearchPrefix_3Tries_variousDataBases(size_t prefixSize, size_t amount, std::vector<DataBase<D>> & fromDBs, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::vector<std::tuple<double, double, double>>
		testRemove_3Tries_variousDataBases(size_t amount, std::vector<DataBase<D>> & fromDBs, size_t repetitions = 1);

		// Funções amplas para testes completos nas 3 Tries
		template <typename D>
		static std::vector<std::vector<std::tuple<double, double, double>>>
		testVariousAmounts(const std::vector<size_t> & amounts, const std::vector<size_t> & prefixAmounts, size_t prefixSize, DataBase<D> & fromDB, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);
		template <typename D>
		static std::vector<std::vector<std::tuple<double, double, double>>>
		testVariousDataBases(size_t amount, size_t prefixAmount, size_t prefixSize, std::vector<DataBase<D>> & fromDBs, size_t overallRepetitions = 1, size_t eachSearchRepetitions = 1);

		// Função principal de execução dos testes pretendidos para Product's
		static std::pair<std::vector<std::vector<std::tuple<double, double, double>>>, std::vector<std::vector<std::tuple<double, double, double>>>>
		runTests(const std::string & csvFilePath);
};


///  IMPLEMENTAÇÃO  .............................

// Funções para o teste de uma Trie (implementação)
template <typename D, size_t M>
double Tester::testInsert(Trie<D, M> & trie, const std::vector<size_t> & indexes, DataBase<D> & fromDB)
{
	// Definir conversão a ser usada, se alguma
	std::string (*conversion)(const std::string &);
	switch (M)
	{
		case 2: conversion = KeyConverter::decimalToBCD; break;
		case 4: conversion = KeyConverter::decimalToQCD; break;
		default: conversion = nullptr;
	}

	// Definir as chaves que serão efetivamente usadas nas inserções
	std::vector<std::string> insertKeys;
	insertKeys.reserve(indexes.size());
	if (conversion)  // conversão necessária
	{
		for (auto index : indexes)
		{
			insertKeys.push_back(conversion(fromDB[index].key));
		}
	}
	else  // conversão desnecessária
	{
		for (auto index : indexes)
		{
			insertKeys.push_back(fromDB[index].key);
		}
	}

	auto start = std::chrono::high_resolution_clock::now();  // "inicia o cronômetro"

	size_t countAll = 0, countInserted = 0;
	for (size_t i = 0; i < indexes.size(); i++)
	{
		if (trie.insert(&fromDB[indexes[i]], insertKeys[i]))
		{
			countInserted++;
		}
		countAll++;
	}

	auto end = std::chrono::high_resolution_clock::now();  // "para o cronômetro"
	auto elapsed = std::chrono::duration<double, std::milli>(end - start);  // calcula o intervalo de tempo entre 'start' e 'end' (double representando ms)

	std::cout << "\t\t\tInsertion complete: " << countInserted << " elements (from a total of " << countAll << ") inserted in Trie of arity " << M << ".\n";
	std::cout << "\t\t\tTime required: " << elapsed.count() << "ms.\n\n";

	return elapsed.count();
}
template <typename D, size_t M>
double Tester::testSearch(Trie<D, M> & trie, const std::vector<size_t> & indexes, DataBase<D> & fromDB, size_t repetitions)
{
	// Definir conversão a ser usada, se alguma
	std::string (*conversion)(const std::string &);
	switch (M)
	{
		case 2: conversion = KeyConverter::decimalToBCD; break;
		case 4: conversion = KeyConverter::decimalToQCD; break;
		default: conversion = nullptr;
	}

	// Definir as chaves que serão efetivamente usadas nas buscas
	std::vector<std::string> searchKeys;
	searchKeys.reserve(indexes.size());
	if (conversion)  // conversão necessária
	{
		for (auto index : indexes)
		{
			searchKeys.push_back(conversion(fromDB[index].key));
		}
	}
	else  // conversão desnecessária
	{
		for (auto index : indexes)
		{
			searchKeys.push_back(fromDB[index].key);
		}
	}

	auto start = std::chrono::high_resolution_clock::now();  // "inicia o cronômetro"

	size_t countAll = 0, countFound = 0;
	for (size_t _ = 0; _ < repetitions; _++)
	{
		countAll = 0; countFound = 0;
		for (size_t i = 0; i < indexes.size(); i++)
		{
			if (trie.search(searchKeys[i]))
			{
				countFound++;
			}
			countAll++;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();  // "para o cronômetro"
	auto elapsed = std::chrono::duration<double, std::milli>(end - start);  // calcula o intervalo de tempo entre 'start' e 'end' (double representando ms)

	std::cout << "\t\t\tSearch complete: " << countFound << " elements (from a total of " << countAll << ") found in Trie of arity " << M << ".\n";
	std::cout << "\t\t\tExecuted " << repetitions << " times.\n";
	std::cout << "\t\t\tTotal time required: " << elapsed.count() << "ms.\n";
	std::cout << "\t\t\tAverage time required per execution: " << elapsed.count() / repetitions << "ms.\n\n";

	return elapsed.count() / repetitions;
}
template <typename D, size_t M>
double Tester::testSearchPrefix(Trie<D, M> & trie, size_t prefixSize, const std::vector<size_t> & ofIndexes, DataBase<D> & fromDB, size_t repetitions)
{
	// Definir conversão a ser usada, se alguma
	std::string (*conversion)(const std::string &);
	switch (M)
	{
		case 2: conversion = KeyConverter::decimalToBCD; break;
		case 4: conversion = KeyConverter::decimalToQCD; break;
		default: conversion = nullptr;
	}

	// Definir os prefixos que serão usados nas buscas
	std::vector<std::string> searchPrefixes;
	searchPrefixes.reserve(ofIndexes.size());
	if (conversion)  // conversão necessária
	{
		for (auto index : ofIndexes)
		{
			searchPrefixes.push_back(conversion(fromDB[index].key.substr(0, prefixSize)));
		}
	}
	else  // conversão desnecessária
	{
		for (auto index : ofIndexes)
		{
			searchPrefixes.push_back(fromDB[index].key.substr(0, prefixSize));
		}
	}

	auto start = std::chrono::high_resolution_clock::now();  // "inicia o cronômetro"

	size_t countAll = 0, countPrefixesFound = 0, countDataFound = 0;
	for (size_t _ = 0; _ < repetitions; _++)
	{
		countAll = 0; countPrefixesFound = 0; countDataFound = 0;
		for (size_t i = 0; i < ofIndexes.size(); i++)
		{
			std::vector<D *> collected = trie.searchPrefix(searchPrefixes[i]);
			if (not collected.empty())
			{
				countPrefixesFound++;
				countDataFound += collected.size();
			}
			countAll++;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();  // "para o cronômetro"
	auto elapsed = std::chrono::duration<double, std::milli>(end - start);  // calcula o intervalo de tempo entre 'start' e 'end' (double representando ms)

	std::cout << "\t\t\tPrefix search complete: " << countPrefixesFound << " prefixes (from a total of " << countAll << ") found in Trie of arity " << M << ".\n";
	std::cout << "\t\t\tTotal of " << countDataFound << " elements found and collected by prefix.\n";
	std::cout << "\t\t\tAverage of " << ((countPrefixesFound != 0) ? countDataFound / countPrefixesFound : 0) << " elements found and collected per prefix searched.\n";
	std::cout << "\t\t\tExecuted " << repetitions << " times.\n";
	std::cout << "\t\t\tTotal time required: " << elapsed.count() << "ms.\n";
	std::cout << "\t\t\tAverage time required per execution: " << elapsed.count() / repetitions << "ms.\n\n";

	return elapsed.count() / repetitions;
}
template <typename D, size_t M>
double Tester::testRemove(Trie<D, M> & trie, const std::vector<size_t> & indexes, DataBase<D> & fromDB)
{
	// Definir conversão a ser usada, se alguma
	std::string (*conversion)(const std::string &);
	switch (M)
	{
		case 2: conversion = KeyConverter::decimalToBCD; break;
		case 4: conversion = KeyConverter::decimalToQCD; break;
		default: conversion = nullptr;
	}

	// Definir as chaves que serão efetivamente usadas nas remoções
	std::vector<std::string> removeKeys;
	removeKeys.reserve(indexes.size());
	if (conversion)  // conversão necessária
	{
		for (auto index : indexes)
		{
			removeKeys.push_back(conversion(fromDB[index].key));
		}
	}
	else  // conversão desnecessária
	{
		for (auto index : indexes)
		{
			removeKeys.push_back(fromDB[index].key);
		}
	}

	auto start = std::chrono::high_resolution_clock::now();  // "inicia o cronômetro"

	size_t countAll = 0, countRemoved = 0;
	for (size_t i = 0; i < indexes.size(); i++)
	{
		if (trie.remove(removeKeys[i]))
		{
			countRemoved++;
		}
		countAll++;
	}

	auto end = std::chrono::high_resolution_clock::now();  // "para o cronômetro"
	auto elapsed = std::chrono::duration<double, std::milli>(end - start);  // calcula o intervalo de tempo entre 'start' e 'end' (double representando ms)

	std::cout << "\t\t\tRemoval complete: " << countRemoved << " elements (from a total of " << countAll << ") removed from Trie of arity " << M << ".\n";
	std::cout << "\t\t\tTime required: " << elapsed.count() << "ms.\n\n";

	return elapsed.count();
}

// Funções especializadas para o teste conjunto de 3 Tries: uma binária, uma quaternária e uma decimal (implementação)
template <typename D>
std::tuple<double, double, double> Tester::testInsert_3Tries(size_t amount, DataBase<D> & fromDB, size_t repetitions)
{
	if (amount == 0 or fromDB.isEmpty() or repetitions == 0) { return {}; }

	std::mt19937 engine(std::random_device {}());
	std::uniform_int_distribution<size_t> distr(0, fromDB.size() - 1);

	double accumulatedResult_2 = 0, accumulatedResult_4 = 0, accumulatedResult_10 = 0;

	for (size_t r = 0; r < repetitions; r++)
	{
		Trie<D, 2> binaryTrie;
		Trie<D, 4> quaternaryTrie;
		Trie<D, 10> decimalTrie;

		std::vector<size_t> indexes;
		indexes.reserve(amount);
		for (size_t i = 0; i < amount; i++)
		{
			indexes.push_back(distr(engine));
		}

		std::cout << "\t\t-------------------- TRIPLE TESTING: INSERT --- EXECUTION #" << r + 1 << " --------------------\n\n";
		std::cout << "\t\trunning testInsert() for a binary Trie ....................\n\n";
		accumulatedResult_2 += Tester::testInsert(binaryTrie, indexes, fromDB);
		std::cout << "\t\trunning testInsert() for a quaternary Trie ....................\n\n";
		accumulatedResult_4 += Tester::testInsert(quaternaryTrie, indexes, fromDB);
		std::cout << "\t\trunning testInsert() for a decimal Trie ....................\n\n";
		accumulatedResult_10 += Tester::testInsert(decimalTrie, indexes, fromDB);
		std::cout << "\t\t---------------------------------------------------------------------------------\n\n";
	}

	std::cout << "\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n";
	std::cout << "\tTOTAL OF " << repetitions << " EXECUTIONS OF TRIPLE TESTING (INSERT).\n\n";
	std::cout << "\tBINARY TRIE - TOTAL TIME REQUIRED: " << accumulatedResult_2 << "ms.\n";
	std::cout << "\tBINARY TRIE - AVERAGE TIME REQUIRED PER EXECUTION: " << accumulatedResult_2 / repetitions << "ms.\n\n";
	std::cout << "\tQUATERNARY TRIE - TOTAL TIME REQUIRED: " << accumulatedResult_4 << "ms.\n";
	std::cout << "\tQUATERNARY TRIE - AVERAGE TIME REQUIRED PER EXECUTION: " << accumulatedResult_4 / repetitions << "ms.\n\n";
	std::cout << "\tDECIMAL TRIE - TOTAL TIME REQUIRED: " << accumulatedResult_10 << "ms.\n";
	std::cout << "\tDECIMAL TRIE - AVERAGE TIME REQUIRED PER EXECUTION: " << accumulatedResult_10 / repetitions << "ms.\n\n\n\n";

	return {accumulatedResult_2 / repetitions, accumulatedResult_4 / repetitions, accumulatedResult_10 / repetitions};
}
template <typename D>
std::tuple<double, double, double> Tester::testSearch_3Tries(size_t amount, DataBase<D> & fromDB, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	if (amount == 0 or fromDB.isEmpty() or overallRepetitions == 0 or eachSearchRepetitions == 0) { return {}; }

	std::mt19937 engine(std::random_device {}());
	std::uniform_int_distribution<size_t> distr(0, fromDB.size() - 1);

	double accumulatedResult_2 = 0, accumulatedResult_4 = 0, accumulatedResult_10 = 0;

	for (size_t r = 0; r < overallRepetitions; r++)
	{
		Trie<D, 2> binaryTrie(fromDB, KeyConverter::decimalToBCD);
		Trie<D, 4> quaternaryTrie(fromDB, KeyConverter::decimalToQCD);
		Trie<D, 10> decimalTrie(fromDB);

		std::vector<size_t> indexes;
		indexes.reserve(amount);
		for (size_t i = 0; i < amount; i++)
		{
			indexes.push_back(distr(engine));
		}

		std::cout << "\t\t-------------------- TRIPLE TESTING: SEARCH --- EXECUTION #" << r + 1 << " --------------------\n\n";
		std::cout << "\t\trunning testSearch() for a binary Trie ....................\n\n";
		accumulatedResult_2 += Tester::testSearch(binaryTrie, indexes, fromDB, eachSearchRepetitions);
		std::cout << "\t\trunning testSearch() for a quaternary Trie ....................\n\n";
		accumulatedResult_4 += Tester::testSearch(quaternaryTrie, indexes, fromDB, eachSearchRepetitions);
		std::cout << "\t\trunning testSearch() for a decimal Trie ....................\n\n";
		accumulatedResult_10 += Tester::testSearch(decimalTrie, indexes, fromDB, eachSearchRepetitions);
		std::cout << "\t\t---------------------------------------------------------------------------------\n\n";
	}

	std::cout << "\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n";
	std::cout << "\tTOTAL OF " << overallRepetitions << " EXECUTIONS OF TRIPLE TESTING (SEARCH).\n\n";
	std::cout << "\tBINARY TRIE - TOTAL TIME REQUIRED (SUM OF AVERAGES): " << accumulatedResult_2 << "ms.\n";
	std::cout << "\tBINARY TRIE - AVERAGE TIME REQUIRED PER SINGLE EXECUTION: " << accumulatedResult_2 / overallRepetitions << "ms.\n\n";
	std::cout << "\tQUATERNARY TRIE - TOTAL TIME REQUIRED (SUM OF AVERAGES): " << accumulatedResult_4 << "ms.\n";
	std::cout << "\tQUATERNARY TRIE - AVERAGE TIME REQUIRED PER SINGLE EXECUTION: " << accumulatedResult_4 / overallRepetitions << "ms.\n\n";
	std::cout << "\tDECIMAL TRIE - TOTAL TIME REQUIRED (SUM OF AVERAGES): " << accumulatedResult_10 << "ms.\n";
	std::cout << "\tDECIMAL TRIE - AVERAGE TIME REQUIRED PER SINGLE EXECUTION: " << accumulatedResult_10 / overallRepetitions << "ms.\n\n\n\n";

	return {accumulatedResult_2 / overallRepetitions, accumulatedResult_4 / overallRepetitions, accumulatedResult_10 / overallRepetitions};
}
template <typename D>
std::tuple<double, double, double> Tester::testSearchPrefix_3Tries(size_t prefixSize, size_t amount, DataBase<D> & fromDB, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	if (amount == 0 or fromDB.isEmpty() or overallRepetitions == 0 or eachSearchRepetitions == 0) { return {}; }

	std::mt19937 engine(std::random_device {}());
	std::uniform_int_distribution<size_t> distr(0, fromDB.size() - 1);

	double accumulatedResult_2 = 0, accumulatedResult_4 = 0, accumulatedResult_10 = 0;

	for (size_t r = 0; r < overallRepetitions; r++)
	{
		Trie<D, 2> binaryTrie(fromDB, KeyConverter::decimalToBCD);
		Trie<D, 4> quaternaryTrie(fromDB, KeyConverter::decimalToQCD);
		Trie<D, 10> decimalTrie(fromDB);

		std::vector<size_t> indexes;
		indexes.reserve(amount);
		for (size_t i = 0; i < amount; i++)
		{
			indexes.push_back(distr(engine));
		}

		std::cout << "\t\t-------------------- TRIPLE TESTING: PREFIX SEARCH --- EXECUTION #" << r + 1 << " --------------------\n\n";
		std::cout << "\t\trunning testSearchPrefix() for a binary Trie ....................\n\n";
		accumulatedResult_2 += Tester::testSearchPrefix(binaryTrie, prefixSize, indexes, fromDB, eachSearchRepetitions);
		std::cout << "\t\trunning testSearchPrefix() for a quaternary Trie ....................\n\n";
		accumulatedResult_4 += Tester::testSearchPrefix(quaternaryTrie, prefixSize, indexes, fromDB, eachSearchRepetitions);
		std::cout << "\t\trunning testSearchPrefix() for a decimal Trie ....................\n\n";
		accumulatedResult_10 += Tester::testSearchPrefix(decimalTrie, prefixSize, indexes, fromDB, eachSearchRepetitions);
		std::cout << "\t\t----------------------------------------------------------------------------------------\n\n";
	}

	std::cout << "\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n";
	std::cout << "\tTOTAL OF " << overallRepetitions << " EXECUTIONS OF TRIPLE TESTING (PREFIX SEARCH).\n\n";
	std::cout << "\tBINARY TRIE - TOTAL TIME REQUIRED (SUM OF AVERAGES): " << accumulatedResult_2 << "ms.\n";
	std::cout << "\tBINARY TRIE - AVERAGE TIME REQUIRED PER SINGLE EXECUTION: " << accumulatedResult_2 / overallRepetitions << "ms.\n\n";
	std::cout << "\tQUATERNARY TRIE - TOTAL TIME REQUIRED (SUM OF AVERAGES): " << accumulatedResult_4 << "ms.\n";
	std::cout << "\tQUATERNARY TRIE - AVERAGE TIME REQUIRED PER SINGLE EXECUTION: " << accumulatedResult_4 / overallRepetitions << "ms.\n\n";
	std::cout << "\tDECIMAL TRIE - TOTAL TIME REQUIRED (SUM OF AVERAGES): " << accumulatedResult_10 << "ms.\n";
	std::cout << "\tDECIMAL TRIE - AVERAGE TIME REQUIRED PER SINGLE EXECUTION: " << accumulatedResult_10 / overallRepetitions << "ms.\n\n\n\n";

	return {accumulatedResult_2 / overallRepetitions, accumulatedResult_4 / overallRepetitions, accumulatedResult_10 / overallRepetitions};
}
template <typename D>
std::tuple<double, double, double> Tester::testRemove_3Tries(size_t amount, DataBase<D> & fromDB, size_t repetitions)
{
	if (amount == 0 or fromDB.isEmpty() or repetitions == 0) { return {}; }

	std::mt19937 engine(std::random_device {}());
	std::uniform_int_distribution<size_t> distr(0, fromDB.size() - 1);

	double accumulatedResult_2 = 0, accumulatedResult_4 = 0, accumulatedResult_10 = 0;

	for (size_t r = 0; r < repetitions; r++)
	{
		Trie<D, 2> binaryTrie(fromDB, KeyConverter::decimalToBCD);
		Trie<D, 4> quaternaryTrie(fromDB, KeyConverter::decimalToQCD);
		Trie<D, 10> decimalTrie(fromDB);

		std::vector<size_t> indexes;
		indexes.reserve(amount);
		for (size_t i = 0; i < amount; i++)
		{
			indexes.push_back(distr(engine));
		}

		std::cout << "\t\t-------------------- TRIPLE TESTING: REMOVE --- EXECUTION #" << r + 1 << " --------------------\n\n";
		std::cout << "\t\trunning testRemove() for a binary Trie ....................\n\n";
		accumulatedResult_2 += Tester::testRemove(binaryTrie, indexes, fromDB);
		std::cout << "\t\trunning testRemove() for a quaternary Trie ....................\n\n";
		accumulatedResult_4 += Tester::testRemove(quaternaryTrie, indexes, fromDB);
		std::cout << "\t\trunning testRemove() for a decimal Trie ....................\n\n";
		accumulatedResult_10 += Tester::testRemove(decimalTrie, indexes, fromDB);
		std::cout << "\t\t---------------------------------------------------------------------------------\n\n";
	}

	std::cout << "\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n";
	std::cout << "\tTOTAL OF " << repetitions << " EXECUTIONS OF TRIPLE TESTING (REMOVE).\n\n";
	std::cout << "\tBINARY TRIE - TOTAL TIME REQUIRED: " << accumulatedResult_2 << "ms.\n";
	std::cout << "\tBINARY TRIE - AVERAGE TIME REQUIRED PER EXECUTION: " << accumulatedResult_2 / repetitions << "ms.\n\n";
	std::cout << "\tQUATERNARY TRIE - TOTAL TIME REQUIRED: " << accumulatedResult_4 << "ms.\n";
	std::cout << "\tQUATERNARY TRIE - AVERAGE TIME REQUIRED PER EXECUTION: " << accumulatedResult_4 / repetitions << "ms.\n\n";
	std::cout << "\tDECIMAL TRIE - TOTAL TIME REQUIRED: " << accumulatedResult_10 << "ms.\n";
	std::cout << "\tDECIMAL TRIE - AVERAGE TIME REQUIRED PER EXECUTION: " << accumulatedResult_10 / repetitions << "ms.\n\n\n\n";

	return {accumulatedResult_2 / repetitions, accumulatedResult_4 / repetitions, accumulatedResult_10 / repetitions};
}

// Funções para testes triplos com diferentes quantidades (implementação)
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testInsert_3Tries_variousAmounts(const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t repetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < amounts.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (INSERT) FOR AMOUNT[" << i << "] = " << amounts[i] << " * *****************************\n\n";
		std::tuple<double, double, double> avgTuple = testInsert_3Tries(amounts[i], fromDB, repetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testSearch_3Tries_variousAmounts(const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < amounts.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (SEARCH) FOR AMOUNT[" << i << "] = " << amounts[i] << " * *****************************\n\n";
		std::tuple<double, double, double> avgTuple = testSearch_3Tries(amounts[i], fromDB, overallRepetitions, eachSearchRepetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testSearchPrefix_3Tries_variousAmounts(size_t prefixSize, const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < amounts.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (PREFIX SEARCH) FOR AMOUNT[" << i << "] = " << amounts[i] << " * *****************************\n\n";
		std::tuple<double, double, double> avgTuple = testSearchPrefix_3Tries(prefixSize, amounts[i], fromDB, overallRepetitions, eachSearchRepetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testRemove_3Tries_variousAmounts(const std::vector<size_t> & amounts, DataBase<D> & fromDB, size_t repetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < amounts.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (REMOVE) FOR AMOUNT[" << i << "] = " << amounts[i] << " * *****************************\n\n";
		std::tuple<double, double, double> avgTuple = testRemove_3Tries(amounts[i], fromDB, repetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testInsert_3Tries_variousDataBases(size_t amount, std::vector<DataBase<D>> & fromDBs, size_t repetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < fromDBs.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (INSERT) FOR DATABASE[" << i << "] ******************************\n\n";
		std::tuple<double, double, double> avgTuple = testInsert_3Tries(amount, fromDBs[i], repetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testSearch_3Tries_variousDataBases(size_t amount, std::vector<DataBase<D>> & fromDBs, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < fromDBs.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (SEARCH) FOR DATABASE[" << i << "] ******************************\n\n";
		std::tuple<double, double, double> avgTuple = testSearch_3Tries(amount, fromDBs[i], overallRepetitions, eachSearchRepetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testSearchPrefix_3Tries_variousDataBases(size_t prefixSize, size_t amount, std::vector<DataBase<D>> & fromDBs, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < fromDBs.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (PREFIX SEARCH) FOR DATABASE[" << i << "] ******************************\n\n";
		std::tuple<double, double, double> avgTuple = testSearchPrefix_3Tries(prefixSize, amount, fromDBs[i], overallRepetitions, eachSearchRepetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}
template <typename D>
std::vector<std::tuple<double, double, double>>
Tester::testRemove_3Tries_variousDataBases(size_t amount, std::vector<DataBase<D>> & fromDBs, size_t repetitions)
{
	std::vector<std::tuple<double, double, double>> result;

	for (size_t i = 0; i < fromDBs.size(); i++)
	{
		std::cout << "\t********** RUNNING TRIPLE TESTING (REMOVE) FOR DATABASE[" << i << "] ******************************\n\n";
		std::tuple<double, double, double> avgTuple = testRemove_3Tries(amount, fromDBs[i], repetitions);
		result.push_back(avgTuple);
	}

	std::cout << "\n\n";

	return result;
}

// Funções amplas para testes completos nas 3 Tries (implementação)
template <typename D>
std::vector<std::vector<std::tuple<double, double, double>>>
Tester::testVariousAmounts(const std::vector<size_t> & amounts, const std::vector<size_t> & prefixAmounts, size_t prefixSize, DataBase<D> & fromDB, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	std::vector<std::vector<std::tuple<double, double, double>>> result;

	std::vector<std::tuple<double, double, double>> avgTupleVector_insert = testInsert_3Tries_variousAmounts(amounts, fromDB, overallRepetitions);
	std::vector<std::tuple<double, double, double>> avgTupleVector_search = testSearch_3Tries_variousAmounts(amounts, fromDB, overallRepetitions, eachSearchRepetitions);
	std::vector<std::tuple<double, double, double>> avgTupleVector_searchPrefix = testSearchPrefix_3Tries_variousAmounts(prefixSize, prefixAmounts, fromDB, overallRepetitions, eachSearchRepetitions);
	std::vector<std::tuple<double, double, double>> avgTupleVector_remove = testRemove_3Tries_variousAmounts(amounts, fromDB, overallRepetitions);

	result.push_back(avgTupleVector_insert);
	result.push_back(avgTupleVector_search);
	result.push_back(avgTupleVector_searchPrefix);
	result.push_back(avgTupleVector_remove);

	return result;
}
template <typename D>
std::vector<std::vector<std::tuple<double, double, double>>>
Tester::testVariousDataBases(size_t amount, size_t prefixAmount, size_t prefixSize, std::vector<DataBase<D>> & fromDBs, size_t overallRepetitions, size_t eachSearchRepetitions)
{
	std::vector<std::vector<std::tuple<double, double, double>>> result;

	std::vector<std::tuple<double, double, double>> avgTupleVector_insert = testInsert_3Tries_variousDataBases(amount, fromDBs, overallRepetitions);
	std::vector<std::tuple<double, double, double>> avgTupleVector_search = testSearch_3Tries_variousDataBases(amount, fromDBs, overallRepetitions, eachSearchRepetitions);
	std::vector<std::tuple<double, double, double>> avgTupleVector_searchPrefix = testSearchPrefix_3Tries_variousDataBases(prefixSize, prefixAmount, fromDBs, overallRepetitions, eachSearchRepetitions);
	std::vector<std::tuple<double, double, double>> avgTupleVector_remove = testRemove_3Tries_variousDataBases(amount, fromDBs, overallRepetitions);

	result.push_back(avgTupleVector_insert);
	result.push_back(avgTupleVector_search);
	result.push_back(avgTupleVector_searchPrefix);
	result.push_back(avgTupleVector_remove);

	return result;
}

// Função principal de execução dos testes pretendidos para Product's (implementação)
std::pair<std::vector<std::vector<std::tuple<double, double, double>>>, std::vector<std::vector<std::tuple<double, double, double>>>>
Tester::runTests(const std::string & csvFilePath)
{
	// Quantidades e base de dados principais
	size_t mainAmount = 5000;
	size_t mainPrefixAmount = 10;
	DataBase<Product> mainDB(csvFilePath);

	// Valores necessários
	size_t initialAmount = 100, limitAmount = mainDB.size(), amountStep = 100;
	size_t initialPrefixAmount = 1, limitPrefixAmount = 50, prefixAmountStep = 1;
	size_t testPrefixSize = 3;
	size_t overallRepetitions = 50, eachSearchRepetitions = 10;

	// Quantidades e bases de dados diversas (para testes comparativos e análise de resultados)
	std::vector<size_t> amounts;
	for (size_t a = initialAmount; a < limitAmount; a += amountStep)
	{
		amounts.push_back(a);
	}
	std::vector<size_t> prefixAmounts;
	for (size_t pA = initialPrefixAmount; pA < limitPrefixAmount; pA += prefixAmountStep)
	{
		prefixAmounts.push_back(pA);
	}
	std::vector<DataBase<Product>> dbs;
	for (auto amount : amounts)
	{
		dbs.emplace_back(csvFilePath, amount);
	}

	std::cout << "\n====================================================================================================\n";
	std::cout << "\n====        T E S T I N G    T R I E S        ==========        U F A B C    2 0 2 6 . 2        ====\n";
	std::cout << "\n====================================================================================================\n\n\n\n";

	std::vector<std::vector<std::tuple<double, double, double>>> avgTupleMatrix_variousAmounts = testVariousAmounts(amounts, prefixAmounts, testPrefixSize, mainDB, overallRepetitions, eachSearchRepetitions);
	std::vector<std::vector<std::tuple<double, double, double>>> avgTupleMatrix_variousDataBases = testVariousDataBases(mainAmount, mainPrefixAmount, testPrefixSize, dbs, overallRepetitions, eachSearchRepetitions);

	std::pair<std::vector<std::vector<std::tuple<double, double, double>>>, std::vector<std::vector<std::tuple<double, double, double>>>> result;
	result.first = avgTupleMatrix_variousAmounts;
	result.second = avgTupleMatrix_variousDataBases;

	std::cout << "\n\n\n";
	std::cout << "\n====================================================================================================\n";
	std::cout << "\n==============================        E N D    O F    T E S T S        =============================\n";
	std::cout << "\n====================================================================================================\n\n\n\n";

	Tester::exportCSVs(result, amounts, prefixAmounts, dbs);

	std::string logFile = "results/testing_state.txt";
	std::ofstream fout(logFile);
	fout << "Products read from CSV file: \"" << csvFilePath << "\"\n";
	fout << "Used values:\n";
	fout << "\tmainAmount = " << mainAmount << "\n";
	fout << "\tmainPrefixAmount = " << mainPrefixAmount << "\n";
	fout << "\tinitialAmount = " << initialAmount << "\n";
	fout << "\tlimitAmount = " << limitAmount << "\n";
	fout << "\tamountStep = " << amountStep << "\n";
	fout << "\tinitialPrefixAmount = " << initialPrefixAmount << "\n";
	fout << "\tlimitPrefixAmount = " << limitPrefixAmount << "\n";
	fout << "\tprefixAmountStep = " << prefixAmountStep << "\n";
	fout << "\ttestPrefixSize = " << testPrefixSize << "\n";
	fout << "\toverallRepetitions = " << overallRepetitions << "\n";
	fout << "\teachSearchRepetitions = " << eachSearchRepetitions << "\n";
	auto now = std::chrono::system_clock::now();
	fout << "Tests performed on: " << std::format("{:%Y-%m-%d %H:%M:%S}", now) << "\n";

	std::cout << "Created: " << logFile << "\n";

	return result;
}

// Função auxiliar para exportação do resultado como CSVs (implementação)
void Tester::exportCSVs(const std::pair<std::vector<std::vector<std::tuple<double, double, double>>>, std::vector<std::vector<std::tuple<double, double, double>>>> & result,
						const std::vector<size_t> & amounts, const std::vector<size_t> & prefixAmounts, std::vector<DataBase<Product>> & dataBases)
{
	auto saveCSV_variousAmounts = [&] (size_t opIndex, const std::string & fileName)
		{
			std::ofstream fout(fileName);
			if (not fout) { return; }

			fout << "amount,binary_ms,quaternary_ms,decimal_ms\n";
			for (size_t i = 0; i < result.first[opIndex].size(); i++)
			{
				auto [b, q, d] = result.first[opIndex][i];
				fout << amounts[i] << "," << b << "," << q << "," << d << "\n";
			}

			std::cout << "Saved: " << fileName << "\n";
		};
	auto saveCSV_variousAmounts_prefix = [&] (size_t opIndex, const std::string & fileName)
		{
			std::ofstream fout(fileName);
			if (not fout) { return; }

			fout << "amount,binary_ms,quaternary_ms,decimal_ms\n";
			for (size_t i = 0; i < result.first[opIndex].size(); i++)
			{
				auto [b, q, d] = result.first[opIndex][i];
				fout << prefixAmounts[i] << "," << b << "," << q << "," << d << "\n";
			}

			std::cout << "Saved: " << fileName << "\n";
		};
	auto saveCSV_variousDataBases = [&] (size_t opIndex, const std::string & fileName)
		{
			std::ofstream fout(fileName);
			if (not fout) { return; }

			fout << "database_size,binary_ms,quaternary_ms,decimal_ms\n";
			for (size_t i = 0; i < result.second[opIndex].size(); i++)
			{
				auto [b, q, d] = result.second[opIndex][i];
				fout << dataBases[i].size() << "," << b << "," << q << "," << d << "\n";
			}

			std::cout << "Saved: " << fileName << "\n";
		};

	std::cout << "Exporting CSV files...\n";

	std::filesystem::create_directories("results");

	saveCSV_variousAmounts(0, "results/insert_amount.csv");               // result.first (variousAmounts)  ~>  ...[0] (INSERT)         ~>  ...[i] ('amount': amounts[i])
	saveCSV_variousAmounts(1, "results/search_amount.csv");               // result.first (variousAmounts)  ~>  ...[1] (SEARCH)         ~>  ...[i] ('amount': amounts[i])
	saveCSV_variousAmounts_prefix(2, "results/searchPrefix_amount.csv");  // result.first (variousAmounts)  ~>  ...[2] (PREFIX SEARCH)  ~>  ...[i] ('amount': prefixAmounts[i])
	saveCSV_variousAmounts(3, "results/remove_amount.csv");               // result.first (variousAmounts)  ~>  ...[3] (REMOVE)         ~>  ...[i] ('amount': amounts[i])

	saveCSV_variousDataBases(0, "results/insert_database.csv");        // result.second (variousDataBases)  ~>  ...[0] (INSERT)         ~>  ...[i] ('database_size': dataBases[i].size())
	saveCSV_variousDataBases(1, "results/search_database.csv");        // result.second (variousDataBases)  ~>  ...[1] (SEARCH)         ~>  ...[i] ('database_size': dataBases[i].size())
	saveCSV_variousDataBases(2, "results/searchPrefix_database.csv");  // result.second (variousDataBases)  ~>  ...[2] (PREFIX SEARCH)  ~>  ...[i] ('database_size': dataBases[i].size())
	saveCSV_variousDataBases(3, "results/remove_database.csv");        // result.second (variousDataBases)  ~>  ...[3] (REMOVE)         ~>  ...[i] ('database_size': dataBases[i].size())
}
