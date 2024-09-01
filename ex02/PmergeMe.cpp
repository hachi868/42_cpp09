#include "PmergeMe.hpp"

const std::string PmergeMe::RESET = "\033[0m";
const std::string PmergeMe::DEBUG = "\033[90m";
const std::string PmergeMe::STATE = "\033[36m";
const std::string PmergeMe::ALERT = "\033[31m";
const std::string PmergeMe::MSG = "\033[34m";
const std::string PmergeMe::ERROR = "\033[31m";

PmergeMe::PmergeMe() : cont_vec_(), cont_deque_() {
    std::cout << PmergeMe::DEBUG << "[PmergeMe] constructor called" << PmergeMe::RESET << std::endl;
}

PmergeMe::PmergeMe(const PmergeMe &obj) : cont_vec_(obj.cont_vec_), cont_deque_(obj.cont_deque_) {
    std::cout << PmergeMe::DEBUG << "[PmergeMe] copy constructor called" << PmergeMe::RESET << std::endl;
}

PmergeMe &PmergeMe::operator = (const PmergeMe &obj) {
    std::cout << PmergeMe::DEBUG << "[PmergeMe] assignation operator called" << PmergeMe::RESET << std::endl;
    if (this != &obj) {
        this->cont_vec_ = obj.cont_vec_;
        this->cont_deque_ = obj.cont_deque_;
    }
    return (*this);
}

PmergeMe::~PmergeMe() {
    std::cout << PmergeMe::DEBUG << "[PmergeMe] destructor called" << PmergeMe::RESET << std::endl;
}

void PmergeMe::parser(std::string arg) {
    if (arg[0] == '-') {
        throw std::runtime_error("Invalid args. (negative number)");
    }
    std::stringstream argStream(arg);
    unsigned int uIntArg;
    if (!(argStream >> uIntArg) || !argStream.eof()) {
        throw std::runtime_error("Invalid args. (cannot convert unsigned int)");
    }
    cont_vec_.push_back(uIntArg);
    cont_deque_.push_back(uIntArg);
}

void PmergeMe::printContainers(std::string prefix) const {
    //オーバーヘッド的に軽量と思われるvectorを使用
    std::cout << prefix << ": ";
    for (std::vector<unsigned int>::const_iterator it = cont_vec_.begin(); it != cont_vec_.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

bool PmergeMe::isContainersEqual() const {
    if (cont_vec_.size() != cont_deque_.size()) {
        return false;
    }
    return std::equal(cont_vec_.begin(), cont_vec_.end(), cont_deque_.begin());
}

void PmergeMe::runSort() {

}
