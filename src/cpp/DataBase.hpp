#pragma once

#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


///  CABEÇALHO  .................................

// Modelo genérico para uma base de dados que guarda tipos D (ex.: Product)
//  obs.1: assume-se que D possui: 'D::keyName' (string), 'key' (string) e 'info' (string -> string)
//  obs.2: assume-se também que D possui um toString próprio
//  em geral, pensa-se que D seja Data ou derivado de Data
template <typename D>
class DataBase
{
	private:

		std::vector<D> container {};  // <!> alterações na DataBase podem invalidar quaisquer estruturas que apontem para seus dados

		// Método privado auxiliar para a manipulação de CSV
		std::vector<std::string> parseCSVLine(const std::string & csvLine);  // transforma linha no formato CSV em um vetor de valores (strings)

	public:

		// Construtores e Destrutor para DataBase (incluindo criá-la a partir de um CSV)
		DataBase() = default;
		DataBase(const std::string & csvFilePath)
		{
			this->readFromCSVFile(csvFilePath);
		}
		DataBase(const std::string & csvFilePath, size_t amount)
		{
			this->readFromCSVFile(csvFilePath, amount);
		}
		~DataBase() = default;

		// Operadores de acesso direto aos dados por índice
		D & operator [] (size_t index);
		const D & operator [] (size_t index) const;

		// Métodos gerais úteis
		size_t size() const;
		bool isEmpty() const;
		void reserve(size_t newCapacity);
		auto begin();
		auto begin() const;
		auto end();
		auto end() const;

		// Métodos de adição/remoção na DataBase
		D & newData();  // retorna uma referência ao dado recém-adicionado
		D & insert(D data);  // retorna uma referência ao dado recém-adicionado
		bool erase(size_t index);  // retorna 'false' caso nada tenha sido removido
		bool clear();  // retorna 'false' caso nada tenha sido removido

		// Métodos para manipulação de CSV
		D & setDataFromCSVLine(D & data, const std::string & csvLine, const std::vector<std::string> & fields);
		D & newDataFromCSVLine(const std::string & csvLine, const std::vector<std::string> & fields);
		void readFromCSVFile(const std::string & csvFilePath);
		void readFromCSVFile(const std::string & csvFilePath, size_t amount);

		// toString e print para DataBase
		std::string toString(bool multiline = true) const;
		void print(bool multiline = true) const;
};


///  IMPLEMENTAÇÃO  .............................

template <typename D>
D & DataBase<D>::operator [] (size_t index)
{
	return this->container[index];
}
template <typename D>
const D & DataBase<D>::operator [] (size_t index) const
{
	return this->container.at(index);
}

template <typename D>
size_t DataBase<D>::size() const
{
	return this->container.size();
}
template <typename D>
bool DataBase<D>::isEmpty() const
{
	return this->container.empty();
}
template <typename D>
void DataBase<D>::reserve(size_t newCapacity)
{
	this->container.reserve(newCapacity);
}
template <typename D>
auto DataBase<D>::begin()
{
	return this->container.begin();
}
template <typename D>
auto DataBase<D>::begin() const
{
	return this->container.begin();
}
template <typename D>
auto DataBase<D>::end()
{
	return this->container.end();
}
template <typename D>
auto DataBase<D>::end() const
{
	return this->container.end();
}

template <typename D>
D & DataBase<D>::newData()
{
	this->container.emplace_back();

	return this->container.back();
}
template <typename D>
D & DataBase<D>::insert(D data)
{
	this->container.push_back(data);

	return this->container.back();
}
template <typename D>
bool DataBase<D>::erase(size_t index)
{
	if (index >= this->container.size()) { return false; }

	this->container.erase(this->container.begin() + index);
	return true;
}
template <typename D>
bool DataBase<D>::clear()
{
	if (this->container.empty()) { return false; }

	this->container.clear();
	return true;
}

template <typename D>
std::vector<std::string> DataBase<D>::parseCSVLine(const std::string & csvLine)
{
	const char sep = ',';

	std::vector<std::string> values;
	std::string value;
	bool insideQuotes = false;

	for (size_t i = 0; i < csvLine.size(); i++)
	{
		char c = csvLine[i];
		if (c == '"')
		{
			if (insideQuotes and (i + 1 < csvLine.size() and csvLine[i + 1] == '"'))  // encontrou '""'
			{
				value += '"';
				i++;
			}
			else  // entrou ou saiu de aspas
			{
				insideQuotes = !insideQuotes;
			}
		}
		else if (c == sep and not insideQuotes)  // terminou de construir um valor
		{
			values.push_back(value);
			value.clear();
		}
		else
		{
			value += c;
		}
	}
	values.push_back(value);  // adiciona ao vetor o que sobrou em 'value' no final

	return values;
}
template <typename D>
D & DataBase<D>::setDataFromCSVLine(D & data, const std::string & csvLine, const std::vector<std::string> & fields)
{
	data.key.clear();
	data.info.clear();

	std::vector<std::string> values = parseCSVLine(csvLine);

	for (size_t i = 0; i < fields.size() and i < values.size(); i++)
	{
		if (fields[i] == D::keyName)
		{
			data.key = values[i];
		}
		else
		{
			data.info[fields[i]] = values[i];
		}
	}

	return data;
}
template <typename D>
D & DataBase<D>::newDataFromCSVLine(const std::string & csvLine, const std::vector<std::string> & fields)
{
	D & data = this->newData();
	this->setDataFromCSVLine(data, csvLine, fields);

	return data;
}
template <typename D>
void DataBase<D>::readFromCSVFile(const std::string & csvFilePath)
{
	std::ifstream fin(csvFilePath);
	if (not fin) { return; }

	std::string line;

	if (not std::getline(fin, line)) { return; }  // pega a primeira linha

	std::vector<std::string> fields = parseCSVLine(line);  //  <!> assume-se que a primeira linha denota os nomes dos campos

	// Tratar leituras inválidas no início do arquivo
	//  <!> a primeira linha (campos) deve começar com um caractere alfanumérico,
	//   pois o primeiro campo só será considerado a partir do primeiro deles
	while (not fields[0].empty() and not std::isalnum(static_cast<unsigned char>(fields[0].front())))
	{
		fields[0].erase(fields[0].begin());
	}

	while (std::getline(fin, line))
	{
		if (not line.empty())
		{
			newDataFromCSVLine(line, fields);
		}
	}
}
template <typename D>
void DataBase<D>::readFromCSVFile(const std::string & csvFilePath, size_t amount)
{
	this->container.reserve(amount);

	std::ifstream fin(csvFilePath);
	if (not fin) { return; }

	std::string line;

	if (not std::getline(fin, line)) { return; }  // pega a primeira linha

	std::vector<std::string> fields = parseCSVLine(line);  //  <!> assume-se que a primeira linha denota os nomes dos campos

	// Tratar leituras inválidas no início do arquivo
	//  <!> a primeira linha (campos) deve começar com um caractere alfanumérico,
	//   pois o primeiro campo só será considerado a partir do primeiro deles
	while (not fields[0].empty() and not std::isalnum(static_cast<unsigned char>(fields[0].front())))
	{
		fields[0].erase(fields[0].begin());
	}

	size_t count = 0;
	while (count < amount and std::getline(fin, line))
	{
		if (not line.empty())
		{
			newDataFromCSVLine(line, fields);
			count++;
		}
	}
}

template <typename D>
std::string DataBase<D>::toString(bool multiline) const
{
	if (this->container.empty()) { return {}; }

	std::string str;

	for (const auto & data : this->container)
	{
		str += data.toString(multiline) + "\n";
	}

	return str;
}
template <typename D>
void DataBase<D>::print(bool multiline) const
{
	std::cout << this->toString(multiline) << "\n";
}
