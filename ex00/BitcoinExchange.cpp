#include "BitcoinExchange.hpp"

const std::string BitcoinExchange::RESET = "\033[0m";
const std::string BitcoinExchange::DEBUG = "\033[90m";
const std::string BitcoinExchange::STATE = "\033[36m";
const std::string BitcoinExchange::ALERT = "\033[31m";
const std::string BitcoinExchange::MSG = "\033[34m";
const std::string BitcoinExchange::ERROR = "\033[31m";

BitcoinExchange::BitcoinExchange() : dataRate_() {
    std::cout << BitcoinExchange::DEBUG << "[BitcoinExchange] constructor called" << BitcoinExchange::RESET << std::endl;
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

std::map<std::string, double> parseCsvToMap(const std::string& csvData) {
    std::map<std::string, double> map;

    return (map);
}

void putValue(std::string date) {

}
