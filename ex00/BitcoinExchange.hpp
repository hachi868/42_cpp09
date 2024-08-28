#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

//#include <algorithm>
//#include <stdio.h>
//#include <iostream>
#include <sstream>
#include <map>

class BitcoinExchange {
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange &obj);
    BitcoinExchange &operator = (const BitcoinExchange &obj);
    ~BitcoinExchange();

    std::map<std::string, double> parseCsvToMap(const std::string& csvData);
    void putValue(std::string date);

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

private:
    std::map<std::string, double> dataRate_;
};


#endif //BITCOIN_EXCHANGE_HPP
