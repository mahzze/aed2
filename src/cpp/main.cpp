#include "Tester.hpp"


int main()
{
	Tester::runTests("../../data/ean13_dataset.csv");
	
	std::cout << "\n\n\n";
	system("pause");
	return 0;
}
