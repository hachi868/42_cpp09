#include "BitcoinExchange.hpp"

const std::string BitcoinExchange::RESET = "\033[0m";
const std::string BitcoinExchange::DEBUG = "\033[90m";
const std::string BitcoinExchange::STATE = "\033[36m";
const std::string BitcoinExchange::ALERT = "\033[31m";
const std::string BitcoinExchange::MSG = "\033[34m";
const std::string BitcoinExchange::ERROR = "\033[31m";

BitcoinExchange::BitcoinExchange(const std::string& csvData) : dataRate_() {
    std::cout << BitcoinExchange::DEBUG << "[BitcoinExchange] constructor called" << BitcoinExchange::RESET << std::endl;
    try {
        dataRate_ = parseCsvToMap(csvData);
    } catch (const BitcoinExchange::ParseException& e) {
        //std::cerr << "ParseException caught: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        //std::cerr << "[Error] " << e.what() << std::endl;
        throw;
    }
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &obj) : dataRate_(obj.dataRate_) {
    std::cout << BitcoinExchange::DEBUG << "[BitcoinExchange] copy constructor called" << BitcoinExchange::RESET << std::endl;
}

BitcoinExchange &BitcoinExchange::operator = (const BitcoinExchange &obj) {
    std::cout << BitcoinExchange::DEBUG << "[BitcoinExchange] assignation operator called" << BitcoinExchange::RESET << std::endl;
    if (this != &obj) {
        this->dataRate_ = obj.dataRate_;
    }
    return (*this);
}

BitcoinExchange::~BitcoinExchange() {
    std::cout << BitcoinExchange::DEBUG << "[BitcoinExchange] destructor called" << BitcoinExchange::RESET << std::endl;
}

void BitcoinExchange::putValue(std::string date) {
    std::cout << "date: " << date << std::endl;
}

const char* BitcoinExchange::ParseException::what() const throw()
{
    return ("Invalid data format for exchange rate");
}

std::map<std::string, double> parseCsvToMap(const std::string& csvData) {
    std::map<std::string, double> map;
    std::ifstream file(csvData);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open database file.");
    }
    std::string line;
    bool isFirstLine = true;  // ヘッダー行スキップ

    // 各行を読み込む
    while (std::getline(file, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;  // ヘッダー行スキップ
        }

        std::istringstream lineStream(line);
        std::string date;
        std::string rateStr;
        if (std::getline(lineStream, date, ',') && std::getline(lineStream, rateStr)) {
            try {
                std::istringstream rateStream(rateStr);
                double exchangeRate;
                //stodが使えないのでstd::stringstreamで数値変換可能かチェックする
                if (!(rateStream >> exchangeRate) || !rateStream.eof()) {
                    throw BitcoinExchange::ParseException();
                }
                map[date] = exchangeRate;
            } catch (const BitcoinExchange::ParseException& e) {
                throw std::runtime_error("Error: Could not parse exchangeRate");
            } catch (const std::exception& e) {
                throw std::runtime_error("Error: Could not init map");
            }
        } else {
            throw std::runtime_error("Error: invalid line format in CSV file.");
        }
    }
    return (map);
}
