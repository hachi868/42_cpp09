#include "RPN.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q RPN");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "[error]Invalid args." << std::endl;
        return 1;
    }
    RPN calculator;
    calculator.parser(argv[1]);
	return 0;
}
