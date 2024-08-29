#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

//#include <algorithm>
//#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

class BitcoinExchange {
public:
    BitcoinExchange(const std::string& csvData);
    BitcoinExchange(const BitcoinExchange &obj);
    BitcoinExchange &operator = (const BitcoinExchange &obj);
    ~BitcoinExchange();

    void putValue(std::string date);

    std::map<std::string, double> dataRate_;

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

    //throw
    class ParseException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

//    class ParseException : public std::runtime_error {
//    public:
//        explicit ParseException(const std::string& message)
//                : std::runtime_error(message) {}
//    };
private:
    BitcoinExchange();
};

std::map<std::string, double> parseCsvToMap(const std::string& csvData);


#endif //BITCOIN_EXCHANGE_HPP
