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
            throw std::runtime_error("Invalid token.");
        }
    }
    if (stack_.size() != 1) {
        throw std::runtime_error("Invalid token.");
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
        throw std::runtime_error("Cant run calc");
    }
    int b = stack_.top();
    stack_.pop();
    int a = stack_.top();
    stack_.pop();
    //std::cout << a << "/" << b << "/" << op << std::endl;
    if (op == '+') {
        calcAdd(a, b);
    } else if (op == '-') {
        calcSubtract(a, b);
    } else if (op == '/') {
        calcDivide(a, b);
    } else if (op == '*') {
        calcMultiply(a, b);
    } else {
        throw std::runtime_error("Invalid token.");
    }
}

void RPN::calcAdd(int a, int b) {
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
        (b < 0 && a < std::numeric_limits<int>::min() - b)) {
        throw std::overflow_error("Addition overflow");
    }
    stack_.push(a + b);
}

void RPN::calcSubtract(int a, int b) {
    if ((b > 0 && a < std::numeric_limits<int>::min() + b) ||
        (b < 0 && a > std::numeric_limits<int>::max() + b)) {
        throw std::overflow_error("Subtraction overflow");
    }
    stack_.push(a - b);
}

void RPN::calcMultiply(int a, int b) {
    if (b != 0 &&
        ((a > 0 && b > 0 && static_cast<double>(a) > static_cast<double>(std::numeric_limits<int>::max()) / b) ||
        (a < 0 && b < 0 && static_cast<double>(a) < static_cast<double>(std::numeric_limits<int>::max()) / b) ||
        (a < 0 && b > 0 && static_cast<double>(a) < static_cast<double>(std::numeric_limits<int>::min()) / b) ||
        (a > 0 && b < 0 && static_cast<double>(a) > static_cast<double>(std::numeric_limits<int>::min()) / b))
       ) {
        throw std::overflow_error("Multiplication overflow");
    }
    stack_.push(a * b);
}

void RPN::calcDivide(int a, int b) {
    if (b == 0) {
        throw std::domain_error("Division by 0");
    }
    stack_.push(a / b);
}