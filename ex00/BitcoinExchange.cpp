#include "BitcoinExchange.hpp"

const std::string BitcoinExchange::RESET = "\033[0m";
const std::string BitcoinExchange::DEBUG = "\033[90m";
const std::string BitcoinExchange::STATE = "\033[36m";
const std::string BitcoinExchange::ALERT = "\033[31m";
const std::string BitcoinExchange::MSG = "\033[34m";
const std::string BitcoinExchange::ERROR = "\033[31m";

BitcoinExchange::BitcoinExchange(const std::string &csvData) : dataRate_() {
    std::cout << BitcoinExchange::DEBUG << "[BitcoinExchange] constructor called" << BitcoinExchange::RESET << std::endl;
    try {
        dataRate_ = parseCsvToMap(csvData);
    } catch (const BitcoinExchange::ParseExceptionDb& e) {
        throw;
    } catch (const std::exception& e) {
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

std::map<std::string, double>::iterator BitcoinExchange::getRefData(const std::string &date) {
    std::map<std::string, double>::iterator it = dataRate_.begin();
    while (it != dataRate_.end() && it->first <= date) {
        //std::cout << it->first << " => " << it->second << std::endl;
        it++;
    }
    if (it == dataRate_.begin()) {
        throw BitcoinExchange::InvalidPastDateException();
    } else {
        --it;
    }
    return it;
}

void BitcoinExchange::putValue(std::string &date, double &exchangeRateDouble) {
    std::map<std::string, double>::iterator it = getRefData(date);
    //std::cout << "///ref::" << it->first << " => " << it->second << " = " << it->second * exchangeRateDouble << "///input::" << date << ":" << exchangeRateDouble << std::endl;
    std::cout << date << " => " << exchangeRateDouble << " = " << it->second * exchangeRateDouble << std::endl;
}

void BitcoinExchange::putValue(std::string &date, unsigned int &exchangeRateInt) {
    std::map<std::string, double>::iterator it = getRefData(date);
    //std::cout << "///ref::" << it->first << " => " << it->second << " = " << it->second * exchangeRateInt << "///input::" << date << ":" << exchangeRateInt << std::endl;
    std::cout << date << " => " << exchangeRateInt << " = " << it->second * exchangeRateInt << std::endl;
}

const char* BitcoinExchange::ParseExceptionDb::what() const throw()
{
    return ("Invalid data format(dataBase)");
}

const char* BitcoinExchange::ParseExceptionInput::what() const throw()
{
    return ("Invalid data format(input)");
}

const char* BitcoinExchange::InvalidPastDateException::what() const throw()
{
    return ("Input date is too far in the past.");
}

//parseCsvToMap
std::string trim(std::string &str) {
    int start = 0;
    int end = str.size() - 1;

    while (start <= end && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }
    while (end >= start && std::isspace(static_cast<unsigned char>(str[end]))) {
        --end;
    }
    return str.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidFirstLine(const std::string &date, const std::string &rateStr, const std::string &dateHead, const std::string &rateStrHead) {
    if (date == dateHead && rateStr == rateStrHead) {
        return true;
    }
    return false;
}

bool BitcoinExchange::isValidDate(std::string date) {
    if (date.size() != 10) {
        return false;
    }
    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') {
                return false;
            }
        } else {
            if (!std::isdigit(date[i])) {
                return false;
            }
        }
    }

    int year, month, day;
    char hyphen1, hyphen2;
    std::istringstream dateStream(date);
    dateStream >> year >> hyphen1 >> month >> hyphen2 >> day;
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }
    return true;
}

bool BitcoinExchange::isValidRate(std::string rateStr) {
    if (rateStr[0] == '-') {
        return false;
    }
    //stodが使えないのでstd::stringstreamで数値変換可能かチェックする
    std::istringstream rateStream(rateStr);
    double exchangeRate;
    if (!(rateStream >> exchangeRate) || !rateStream.eof()) {
        return false;
    }
    return true;
}

bool BitcoinExchange::split_line(std::istringstream &lineStream, char dlm, std::string &date, std::string &rateStr) {
    std::getline(lineStream, date, dlm);
    std::getline(lineStream, rateStr);
    //trim space
    date = trim(date);
    rateStr = trim(rateStr);
    if (date.size() == 0 || rateStr.size() == 0) {
        return false;
    }
    return true;
}

std::map<std::string, double> parseCsvToMap(const std::string &csvData) {
    std::map<std::string, double> map;
    std::ifstream file(csvData);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open database file.");
    }

    // read line
    std::string line;
    bool isFirstLine = true;
    std::string date;
    std::string rateStr;

    // 各行を読み込む
    while (std::getline(file, line)) {
        try {
            std::istringstream lineStream(line);
            if (!BitcoinExchange::split_line(lineStream, ',', date, rateStr)) {
                throw BitcoinExchange::ParseExceptionDb();
            }
            if (isFirstLine) {
                if (BitcoinExchange::isValidFirstLine(date, rateStr, "date", "exchange_rate")) {
                    isFirstLine = false;
                    continue;
                } else {
                    throw BitcoinExchange::ParseExceptionDb();
                }
            }
            if (!BitcoinExchange::isValidDate(date)) {
                throw BitcoinExchange::ParseExceptionDb();
            }
            if (!BitcoinExchange::isValidRate(rateStr)) {
                throw BitcoinExchange::ParseExceptionDb();
            }
            //1行目、date,exchange_rate 全て問題なければ格納
            std::istringstream rateStream(rateStr);
            double exchangeRate;
            rateStream >> exchangeRate;
            map[date] = exchangeRate;
        } catch (const BitcoinExchange::ParseExceptionDb& e) {
            throw;
        } catch (const std::exception& e) {
            throw;
        }
    }
    return (map);
}
