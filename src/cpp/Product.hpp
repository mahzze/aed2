#pragma once

#include <string>
#include <string_view>
#include "Data.hpp"


///  CABEÇALHO  .................................

// Estrutura de um produto; herda tudo que Data tem
struct Product: public Data
{
	static constexpr std::string_view keyName = "code";

	// Construção e Destruição (padrões)
	Product() = default;
	~Product() = default;

	// Métodos específicos para produtos
	std::string getGS1Prefix() const;
};


///  IMPLEMENTAÇÃO  .............................

std::string Product::getGS1Prefix() const
{
	return this->key.substr(0, 3);
}
