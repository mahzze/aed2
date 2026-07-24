#pragma once

#include <iostream>
#include <map>
#include <string>
#include <string_view>


///  CABEÇALHO  .................................

// Estrutura para dados quaisquer
struct Data
{
	static constexpr std::string_view keyName = "key";

	std::string key {};
	std::map<std::string, std::string> info {};  // mapa para todos os campos de Data (exceto sua chave)
												 //  obs.: usando std::map, os campos sempre estarão em ordem (alfabética)

	// Construção e Destruição (padrões)
	Data() = default;
	~Data() = default;

	// Operadores de acesso direto ao mapa
	std::string & operator [] (const std::string & field);
	const std::string & operator [] (const std::string & field) const;

	// toString e print para Data
	std::string toString(bool multiline = true) const;
	void print(bool multiline = true) const;
};


///  IMPLEMENTAÇÃO  .............................

std::string & Data::operator [] (const std::string & field)
{
	return this->info[field];
}
const std::string & Data::operator [] (const std::string & field) const
{
	return this->info.at(field);
}

std::string Data::toString(bool multiline) const
{
	if (this->info.empty()) { return this->key; }

	std::string str;

	if (multiline)
	{
		str += "<key> = " + this->key + "\n";
		for (const auto & [field, value] : this->info)
		{
			str += field + ": " + value + "\n";
		}
	}
	else
	{
		std::string separation = " | ";
		std::string keySeparation = "  >>  ";

		str += this->key + keySeparation;
		for (const auto & [field, value] : this->info)
		{
			str += value + separation;
		}
		str.erase(str.size() - separation.size());
	}

	return str;
}
void Data::print(bool multiline) const
{
	std::cout << this->toString(multiline) << "\n";
}
