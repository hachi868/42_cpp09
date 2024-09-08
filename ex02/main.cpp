#include "PmergeMe.hpp"
#include <sys/time.h>

__attribute__((destructor))
static void destructor() {
	system("leaks -q PmergeMe");
}

//subject
// ./PmergeMe 3 5 9 7 4

// ./PmergeMe "-1" "2"
// ./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
// ./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '` => mac
// ./PmergeMe 56 5 908 3
// ./PmergeMe 3 500 100 790
// ./PmergeMe 3 500 100 790 6 9 38 806 577
// ./PmergeMe 8 9 2 21 15 20 3 7 1 6 11 17 4 19 16 10 13 18 5 12 22 23 14
// ./PmergeMe 8 9 2 21 15 20 3 7 1 6 11 17 4 19 16 10 13 18 5 12 22 23 14 8

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
            throw std::runtime_error("The contents of the cont_vec_ and cont_deque_ are not equal.(before)");
        }
        p_merge_me.printContainers("Before");

        //time
        struct timeval start,end;

        //sort(vec)
        gettimeofday(&start, NULL);
        p_merge_me.runSortVec();
        gettimeofday(&end, NULL);
        double time_vec = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

        //sort(deque)
        gettimeofday(&start, NULL);
        p_merge_me.runSortDeque();
        gettimeofday(&end, NULL);
        double time_deque = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

        if(!p_merge_me.isContainersEqual()) {
            throw std::runtime_error("The contents of the cont_vec_ and cont_deque_ are not equal.(after)");
        }

        //print
        p_merge_me.printContainers("After");
        std::cout << "Time to process a range of " << argc - 1 << " elements with std::vector : " << time_vec << " µs" <<  std::endl;
        std::cout << "Time to process a range of " << argc - 1 << " elements with std::deque : " << time_deque << " µs" <<  std::endl;
    } catch (const std::exception& e) {
        std::cerr << PmergeMe::ERROR << "[ERROR] " << e.what() << PmergeMe::RESET << std::endl;
        return 1;
    }

    return 0;
}
