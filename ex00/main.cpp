#include "BitcoinExchange.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q btc");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Error: could not open file." << std::endl;
        return 1;
    }
    try {
        BitcoinExchange btcEx = BitcoinExchange("./data/data.csv");
        if (btcEx.dataRate_.empty()) {
            throw std::runtime_error("Error: dataRate_ is empty.");
        }
        // open
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cout << "Error: could not open file." << std::endl;
            return 1;
        }
        // read line
        std::string line;
        while (std::getline(file, line)) {
            btcEx.putValue(line);
        }
        // close
        file.close();
    } catch (const BitcoinExchange::ParseException& e) {
        std::cerr << "ParseException caught: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
