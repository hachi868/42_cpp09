#include "PmergeMe.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q PmergeMe");
}

int main(int argc, char **argv)
{
	return 0;
}
