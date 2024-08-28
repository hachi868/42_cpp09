#include "RPN.hpp"

const std::string RPN::RESET = "\033[0m";
const std::string RPN::DEBUG = "\033[90m";
const std::string RPN::STATE = "\033[36m";
const std::string RPN::ALERT = "\033[31m";
const std::string RPN::MSG = "\033[34m";
const std::string RPN::ERROR = "\033[31m";

RPN::RPN() : stack_() {
    std::cout << RPN::DEBUG << "[RPN] constructor called" << RPN::RESET << std::endl;
}

RPN::RPN(const RPN &obj) : stack_(obj.stack_) {
    std::cout << RPN::DEBUG << "[RPN] copy constructor called" << RPN::RESET << std::endl;
}

RPN &RPN::operator = (const RPN &obj) {
    std::cout << RPN::DEBUG << "[RPN] assignation operator called" << RPN::RESET << std::endl;
    if (this != &obj) {
        this->stack_ = obj.stack_;
    }
    return (*this);
}

RPN::~RPN() {
    std::cout << RPN::DEBUG << "[RPN] destructor called" << RPN::RESET << std::endl;
}

void RPN::parser(std::string expression) {
    std::stringstream ss(expression);
    std::string token;
    while (ss >> token) {
        if (isDigitPositive(token)) {
            stack_.push(token[0] - '0');
        } else if (isDigitNegative(token)) {
            stack_.push((token[1] - '0') * -1);
        } else if (isOperator(token)) {
            runCalc(token[0]);
        } else {
            std::cout << "Error: invalid token." << std::endl;
            return;
        }
    }
//    while (!stack_.empty()) {
//        std::cout << stack_.top() << " ";
//        stack_.pop();
//    }
    if (stack_.size() != 1) {
        std::cout << "Error" << std::endl;
    }
    std::cout << stack_.top() << std::endl;
}

bool RPN::isDigitPositive(std::string token) {
    if (token.size() == 1 && (token[0] >= '0' && token[0] <= '9')) {
        return true;
    }
    return false;
}

bool RPN::isDigitNegative(std::string token) {
    if (token.size() == 2 && token[0] == '-' && (token[1] >= '0' && token[0] <= '1')) {
        return true;
    }
    return false;
}

bool RPN::isOperator(std::string token) {
    if (token.size() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '/' || token[0] == '*')) {
        return true;
    }
    return false;
}

void RPN::runCalc(char op) {
    if (stack_.size() < 2) {
        std::cout << "Error: cant run calc" << std::endl;
    }
    int b = stack_.top();
    stack_.pop();
    int a = stack_.top();
    stack_.pop();
    if (op == '+') {
        calcAdd(a, b);
    } else if (op == '-') {
        calcSubtract(a, b);
    } else if (op == '/') {
        calcMultiply(a, b);
    } else if (op == '*') {
        calcDivide(a, b);
    } else {
        std::cout << "Error: something wrong with calc" << std::endl;
        return ;
    }
}

void RPN::calcAdd(int a, int b) {
    stack_.push(a + b);
}

void RPN::calcSubtract(int a, int b) {
    stack_.push(a - b);
}

void RPN::calcMultiply(int a, int b) {
    if (b == 0) {
        std::cout << "Error: Division by 0" << std::endl;
        return ;
    }
    stack_.push(a / b);
}

void RPN::calcDivide(int a, int b) {
    stack_.push(a * b);
}