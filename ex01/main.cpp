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
    try {
        RPN rpn = RPN();
        rpn.parser(argv[1]);
    } catch (const std::overflow_error& e) {
        std::cerr << RPN::ERROR << "[ERROR]" << e.what() << RPN::RESET << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << RPN::ERROR << "[ERROR]" << e.what() << RPN::RESET << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << RPN::ERROR << "[ERROR]" << e.what() << RPN::RESET << std::endl;
        return 1;
    }
	return 0;
}
