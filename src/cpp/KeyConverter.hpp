#pragma once

#include <string>


///  CABEÇALHO  .................................

// Classe estática para conversões de chaves (pré-inserção nas Tries, por exemplo)
class KeyConverter
{
	public:

		static std::string decimalToBCD(const std::string & decimalKey);  // BCD: Binary-Coded Decimal
		static std::string decimalToQCD(const std::string & decimalKey);  // QCD: "Quaternary-Coded Decimal"
																		  //  (codificação correspondente a selecionar bits 2 a 2 no formato BCD)
};


///  IMPLEMENTAÇÃO  .............................

std::string KeyConverter::decimalToBCD(const std::string & decimalKey)
{
	std::string result;
	result.reserve(4 * decimalKey.size());

	for (char c : decimalKey)
	{
		size_t digit = c - '0';
		if (digit > 9) { return {}; }

		for (int bit = 3; bit >= 0; bit--)
		{
			result += ((digit >> bit) & 1) ? '1' : '0';
		}
	}

	return result;
}
std::string KeyConverter::decimalToQCD(const std::string & decimalKey)
{
	std::string result;
	result.reserve(2 * decimalKey.size());

	for (char c : decimalKey)
	{
		size_t digit = c - '0';
		if (digit > 9) { return {}; }

		size_t msb = digit / 4;
		size_t lsb = digit % 4;

		result += static_cast<char>('0' + msb);
		result += static_cast<char>('0' + lsb);
	}

	return result;
}
