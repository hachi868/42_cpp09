#include "PmergeMe.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q PmergeMe");
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "[error]Invalid args." << argc << std::endl;
        return 1;
    }
    PmergeMe p_merge_me = PmergeMe();
    for (int i = 1; i < argc; i++) {
        p_merge_me.parser(argv[i]);
    }
    std::cout << std::endl;
    return 0;
}
