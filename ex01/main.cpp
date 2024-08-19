#include "RPN.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q RPN");
}

int main(int argc, char **argv)
{
	return 0;
}
