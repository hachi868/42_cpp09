#include "BitcoinExchange.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q btc");
}

int main(int argc, char **argv)
{
	return 0;
}
