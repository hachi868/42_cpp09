#include "PmergeMe.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q PmergeMe");
}

//subject
// ./PmergeMe 3 5 9 7 4
// ./PmergeMe "-1" "2"
// ./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
// ./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '` => mac

//invalid arg
// ./PmergeMe
// ./PmergeMe 42


int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "[ERROR]Invalid args" << std::endl;
        return 1;
    }
    try {
        PmergeMe p_merge_me = PmergeMe();
        for (int i = 1; i < argc; i++) {
            p_merge_me.parser(argv[i]);
        }
        //print
        if(!p_merge_me.isContainersEqual()) {
            throw std::runtime_error("The contents of the cont_vec_ and cont_deque_ are not equal.");
        }
        p_merge_me.printContainers("Before");

    } catch (const std::exception& e) {
        std::cerr << PmergeMe::ERROR << "[ERROR] " << e.what() << PmergeMe::RESET << std::endl;
        return 1;
    }

    return 0;
}
