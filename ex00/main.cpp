#include "BitcoinExchange.hpp"

__attribute__((destructor))
static void destructor() {
	system("leaks -q btc");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << BitcoinExchange::ERROR << "[error]Invalid args." << BitcoinExchange::RESET << std::endl;
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
            std::cerr << BitcoinExchange::ERROR << "[error]could not open file." << BitcoinExchange::RESET << std::endl;
            return 1;
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
                    std::cerr << BitcoinExchange::ERROR << "[error]Invalid Line: " << line << BitcoinExchange::RESET << std::endl;
                    continue;
                }
                if (isFirstLine) {
                    if (BitcoinExchange::isValidFirstLine(date, rateStr, "date", "value")) {
                        isFirstLine = false;
                        continue;
                    } else {
                        throw BitcoinExchange::ParseException();
                    }
                }
                if (!BitcoinExchange::isValidDate(date)) {
                    std::cerr << BitcoinExchange::ERROR << "[error]Invalid Date: " << date << BitcoinExchange::RESET << std::endl;
                    continue;
                }
//                if (!BitcoinExchange::isValidRate(rateStr)) {
//                    std::cerr << "[error]Invalid Rate: " << rateStr << std::endl;
//                    continue;
//                }
                //rate
                if (rateStr[0] == '-') {
                    std::cerr << BitcoinExchange::ERROR << "[error]not a positive number: " << rateStr << BitcoinExchange::RESET << std::endl;
                    continue;
                }
                std::stringstream rateStream(rateStr);
                if (rateStr.find('.') != std::string::npos) {
                    //double
                    double exchangeRateDouble;
                    if (!(rateStream >> exchangeRateDouble) || !rateStream.eof()) {
                        std::cerr << BitcoinExchange::ERROR << "[error]Invalid Rate: " << rateStr << "/" << exchangeRateDouble << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    if (exchangeRateDouble > 1000.0) {
                        std::cerr << BitcoinExchange::ERROR << "[error]too large a number: " << rateStr << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    btcEx.putValue(date, exchangeRateDouble);
                } else {
                    //int
                    unsigned int exchangeRateInt;
                    if (!(rateStream >> exchangeRateInt) || !rateStream.eof()) {
                        std::cerr << BitcoinExchange::ERROR << "[error]Invalid Rate: " << rateStr << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    if (exchangeRateInt > 1000) {
                        std::cerr << BitcoinExchange::ERROR << "[error]too large a number: " << rateStr << BitcoinExchange::RESET << std::endl;
                        continue;
                    }
                    btcEx.putValue(date, exchangeRateInt);
                }
            } catch (const BitcoinExchange::ParseException& e) {
                throw BitcoinExchange::ParseException();
            } catch (const std::exception& e) {
                throw std::runtime_error("Error: Could not print");
            }
        }
        // close
        file.close();
    } catch (const BitcoinExchange::ParseException& e) {
        std::cerr << BitcoinExchange::ERROR << "ParseException caught: " << e.what() << BitcoinExchange::RESET << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << BitcoinExchange::ERROR << e.what() << BitcoinExchange::RESET << std::endl;
        return 1;
    }
    return 0;
}
