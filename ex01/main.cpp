#include "RPN.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q RPN");
}

//subject
// ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
// ./RPN "7 7 * 7 -"
// ./RPN "1 2 * 2 / 2 * 2 4 - +"
// ./RPN "(1 + 1)"

//invalid arg
// ./RPN
// ./RPN 7 7 * 7 -

//invalid token
// ./RPN ""
// ./RPN "8 9 * 9 - 9 - 9 - 4 - 1"
// ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 + +"
// ./RPN "7 7 % 7 -"
// ./RPN "7 7- * 7 -"
// ./RPN "70 4 +"
// ./RPN "7 -4 +"

//overflow
// ./RPN "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *" => Overflow
// ./RPN "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *" => OK
// ./RPN "0 1 - 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *" => Overflow
// ./RPN "0 1 - 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *" => OK

//Division by 0
// ./RPN "8 0 /"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << RPN::ERROR << "[ERROR]Invalid args." << RPN::RESET << std::endl;
        return 1;
    }
    try {
        RPN rpn = RPN();
        rpn.parser(argv[1]);
    } catch (const std::overflow_error& e) {
        std::cout << RPN::ERROR << "[ERROR]" << e.what() << RPN::RESET << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cout << RPN::ERROR << "[ERROR]" << e.what() << RPN::RESET << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cout << RPN::ERROR << "[ERROR]" << e.what() << RPN::RESET << std::endl;
        return 1;
    }
	return 0;
}
