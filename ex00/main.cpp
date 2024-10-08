#include "BitcoinExchange.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q btc");
}

//subject
// ./btc ./data/input.txt
//Invalid firstLine
// ./btc ./data/input2.txt
//Invalid firstLine
// ./btc ./data/input3.txt
//empty
// ./btc ./data/input_empty.txt
//404
// ./btc ./data/input_404.txt

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << BitcoinExchange::ERROR << "Error: could not open file." << BitcoinExchange::RESET << std::endl;
        return 1;
    }
    try {
        BitcoinExchange btcEx = BitcoinExchange("./data/data.csv");
        //Invalid firstLine
        //BitcoinExchange btcEx = BitcoinExchange("./data/data2.csv");
        //empty
        //BitcoinExchange btcEx = BitcoinExchange("./data/data_empty.csv");
        //404
        //BitcoinExchange btcEx = BitcoinExchange("./data/data404.csv");
        if (btcEx.dataRate_.empty()) {
            throw std::runtime_error("dataRate_ is empty.");
        }
        // open
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            throw std::runtime_error("could not open input file.");
        }

        // read line
        std::string line;
        bool isFirstLine = true;
        std::string date;
        std::string rateStr;

        while (std::getline(file, line)) {
            try {
                std::istringstream lineStream(line);
                if (!BitcoinExchange::split_line(lineStream, '|', date, rateStr)) {
                    std::cerr << BitcoinExchange::ERROR << "Error: Invalid Line: " << line << BitcoinExchange::RESET << std::endl;
                    continue;
                }
                if (isFirstLine) {
                    if (BitcoinExchange::isValidFirstLine(date, rateStr, "date", "value")) {
                        isFirstLine = false;
                        continue;
                    } else {
                        throw BitcoinExchange::ParseExceptionInput();
                    }
                }
                if (!BitcoinExchange::isValidDate(date)) {
                    std::cerr << BitcoinExchange::ERROR << "Error: bad input => " << date << BitcoinExchange::RESET << std::endl;
                    continue;
                }
                //rate
                if (rateStr[0] == '-') {
                    std::cerr << BitcoinExchange::ERROR << "Error: not a positive number." << BitcoinExchange::RESET << std::endl;
                    continue;
                }
                std::stringstream rateStream(rateStr);
                if (rateStr.find('.') != std::string::npos) {
                    //double
                    double exchangeRateDouble;
                    if (!(rateStream >> exchangeRateDouble) || !rateStream.eof()) {
                        std::cerr << BitcoinExchange::ERROR << "Error: invalid Rate." << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    if (exchangeRateDouble > 1000.0) {
                        std::cerr << BitcoinExchange::ERROR << "Error: too large a number." << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    btcEx.putValue(date, exchangeRateDouble);
                } else {
                    //int
                    unsigned int exchangeRateInt;
                    if (!(rateStream >> exchangeRateInt) || !rateStream.eof()) {
                        std::cerr << BitcoinExchange::ERROR << "Error: Invalid Rate." << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    if (exchangeRateInt > 1000) {
                        std::cerr << BitcoinExchange::ERROR << "Error: too large a number." << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    btcEx.putValue(date, exchangeRateInt);
                }
            } catch (const BitcoinExchange::InvalidPastDateException& e) {
                std::cerr << BitcoinExchange::ERROR << "Error: " << e.what() << BitcoinExchange::RESET << std::endl;
            } catch (const BitcoinExchange::ParseExceptionInput& e) {
                std::cerr << BitcoinExchange::ERROR << "Error: " << e.what() << BitcoinExchange::RESET << std::endl;
                break;
            } catch (const std::exception& e) {
                std::cerr << BitcoinExchange::ERROR << "Error: " << e.what() << BitcoinExchange::RESET << std::endl;
                break;
            }
        }
        // close
        file.close();
    } catch (const BitcoinExchange::ParseExceptionDb& e) {
        std::cerr << BitcoinExchange::ERROR << "Error: " << e.what() << BitcoinExchange::RESET << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << BitcoinExchange::ERROR << "Error: " << e.what() << BitcoinExchange::RESET << std::endl;
        return 1;
    }
    return 0;
}
