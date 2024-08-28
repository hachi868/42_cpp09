#ifndef RPN_HPP
#define RPN_HPP

//#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <stack>

class RPN {
public:
    RPN();
    RPN(const RPN &obj);
    RPN &operator = (const RPN &obj);
    ~RPN();

    void parser(std::string expression);
    void runCalc(char op);

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

private:
    std::stack<int> stack_;

    bool isDigitPositive(std::string token);
    bool isDigitNegative(std::string token);
    bool isOperator(std::string token);
    void calcAdd(int a, int b);
    void calcSubtract(int a, int b);
    void calcMultiply(int a, int b);
    void calcDivide(int a, int b);
};


#endif //RPN_HPP
