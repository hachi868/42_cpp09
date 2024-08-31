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
        bool isFirstLine = true;
        std::string date;
        std::string rateStr;

        while (std::getline(file, line)) {
            try {
                std::istringstream lineStream(line);
                if (!BitcoinExchange::split_line(lineStream, '|', date, rateStr)) {
                    std::cerr << "[error]Invalid Line: " << line << std::endl;
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
                    std::cerr << "[error]Invalid Date: " << date << std::endl;
                    continue;
                }
//                if (!BitcoinExchange::isValidRate(rateStr)) {
//                    std::cerr << "[error]Invalid Rate: " << rateStr << std::endl;
//                    continue;
//                }
                //rate
                if (rateStr[0] == '-') {
                    std::cerr << "[error]not a positive number: " << rateStr << std::endl;
                    continue;
                }
                std::stringstream rateStream(rateStr);
                if (rateStr.find('.') != std::string::npos) {
                    //double
                    double exchangeRateDouble;
                    if (!(rateStream >> exchangeRateDouble) || !rateStream.eof()) {
                        std::cerr << "[error]Invalid Rate: " << rateStr << "/" << exchangeRateDouble << std::endl;
                        continue;
                    }
                    if (exchangeRateDouble > 1000.0) {
                        std::cerr << "[error]too large a number: " << rateStr << std::endl;
                        continue;
                    }
                    std::cout << "[ok]Rate:Double: " << exchangeRateDouble << std::endl;
                    btcEx.putValue(date, exchangeRateDouble);
                } else {
                    //int
                    unsigned int exchangeRateInt;
                    if (!(rateStream >> exchangeRateInt) || !rateStream.eof()) {
                        std::cerr << "[error]Invalid Rate: " << rateStr << std::endl;
                        continue;
                    }
                    if (exchangeRateInt > 1000) {
                        std::cerr << "[error]too large a number: " << rateStr << std::endl;
                        continue;
                    }
                    std::cout << "[ok]Rate:Int: " << exchangeRateInt << std::endl;
                    btcEx.putValue(date, exchangeRateInt);
                }
            } catch (const BitcoinExchange::ParseException& e) {
                throw BitcoinExchange::ParseException();
            } catch (const std::exception& e) {
                throw std::runtime_error("Error: Could not init map");
            }
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
