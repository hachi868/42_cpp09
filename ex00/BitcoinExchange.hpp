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
    BitcoinExchange(const std::string &csvData);
    BitcoinExchange(const BitcoinExchange &obj);
    BitcoinExchange &operator = (const BitcoinExchange &obj);
    ~BitcoinExchange();

    void putValue(std::string &date, double &exchangeRateDouble);
    void putValue(std::string &date, unsigned int &exchangeRateInt);

    std::map<std::string, double> dataRate_;

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

    //static std::string trim(std::string &str);
    static bool isValidFirstLine(const std::string &date, const std::string &rateStr, const std::string &dateHead, const std::string &rateStrHead);
    static bool split_line(std::istringstream &lineStream, char dlm, std::string &date, std::string &rateStr);
    static bool isValidDate(std::string date);
    static bool isValidRate(std::string rateStr);

    //throw
    class ParseExceptionDb : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class ParseExceptionInput : public std::exception {
    public:
        virtual const char* what() const throw();
    };
private:
    BitcoinExchange();
    std::map<std::string, double>::iterator getRefData(const std::string &date);
};

std::map<std::string, double> parseCsvToMap(const std::string &csvData);


#endif //BITCOIN_EXCHANGE_HPP
