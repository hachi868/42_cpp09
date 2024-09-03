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

void PmergeMe::runSortVec() {
    std::vector<unsigned int> cont_merge;
    //std::vector<unsigned int> cont_smaller;

    splitIntoPairs(cont_vec_, cont_merge);

//    std::cout << "cont_larger: ";
//    for (size_t i = 0; i < cont_larger.size(); ++i) {
//        std::cout << cont_larger[i] << " ";
//    }
//    std::cout << std::endl;
//
//    std::cout << "cont_smaller: ";
//    for (size_t i = 0; i < cont_smaller.size(); ++i) {
//        std::cout << cont_smaller[i] << " ";
//    }
//    std::cout << std::endl;
}

void PmergeMe::runSortDeque() {
}

void PmergeMe::splitIntoPairs(std::vector<unsigned int> &cont_org, std::vector<unsigned int> &cont_merge) {
    std::vector<unsigned int> cont_smaller;
    std::vector<unsigned int> cont_larger;
    std::vector<unsigned int> tmp_cont_smaller;
    std::vector<unsigned int> tmp_cont_larger;

    if (cont_org.size() <= 1) {
        if (!cont_org.empty()) {
            tmp_cont_larger.push_back(cont_org[0]);
        }
        return;
    }
    for (size_t i = 0; i < cont_org.size(); i += 2) {
        if (i + 1 < cont_org.size()) {
            if (cont_org[i] < cont_org[i + 1]) {
                tmp_cont_smaller.push_back(cont_org[i]);
                tmp_cont_larger.push_back(cont_org[i + 1]);
            } else {
                tmp_cont_smaller.push_back(cont_org[i + 1]);
                tmp_cont_larger.push_back(cont_org[i]);
            }
        } else {
            tmp_cont_larger.push_back(cont_org[i]);
        }
    }
    splitIntoPairs(tmp_cont_smaller, cont_merge);
    splitIntoPairs(tmp_cont_larger, cont_merge);

    //merge
    mergeLS(cont_merge, tmp_cont_larger, tmp_cont_smaller);

    std::cout << "--- cont_merge ---" << std::endl;
    for (size_t i = 0; i < cont_merge.size(); ++i) {
        std::cout << cont_merge[i] << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::mergeLS(std::vector<unsigned int> &cont_merge, std::vector<unsigned int> &cont_larger, std::vector<unsigned int> &cont_smaller) {
    std::cout << std::endl << "***mergeLS" << std::endl;
    std::cout << "cont_larger: not sorted?:";
    for (size_t i = 0; i < cont_larger.size(); ++i) {
        std::cout << cont_larger[i] << " ";
    }
    std::cout << "(size:"<< cont_larger.size() << ")"<<std::endl;

    std::cout << "cont_smaller: not sorted?:";
    for (size_t i = 0; i < cont_smaller.size(); ++i) {
        std::cout << cont_smaller[i] << " ";
    }
    std::cout << "(size:"<< cont_smaller.size() << ")"<<std::endl;

    size_t i = 0, j = 0;
    while (i < cont_smaller.size() && j < cont_larger.size()) {
        if (cont_smaller[i] < cont_larger[j]) {
            cont_merge.push_back(cont_smaller[i]);
            std::cout << "//merge1: " << cont_smaller[i] << ";;" << cont_larger[j] << std::endl;
            i++;
        } else {
            cont_merge.push_back(cont_larger[j]);
            std::cout << "//merge2: " << cont_larger[j] << std::endl;
            j++;
        }
    }

    std::cout << "cont_merge.size() 1: " << cont_merge.size() << std::endl;

    std::cout << "cont_merge: fail:";
    for (size_t i = 0; i < cont_merge.size(); ++i) {
        std::cout << cont_merge[i] << " ";
    }
    std::cout << std::endl;

    // 残りの要素を追加
    while (i < cont_smaller.size()) {
        cont_merge.push_back(cont_smaller[i]);
        std::cout << "//merge3: " << cont_smaller[i] << std::endl;
        i++;
    }

    while (j < cont_larger.size()) {
        cont_merge.push_back(cont_larger[j]);
        std::cout << "//merge4: " << cont_larger[j] << std::endl;
        j++;
    }

    std::cout << "cont_merge.size() 2: " << cont_merge.size() << std::endl;

    std::cout << "cont_merge: ";
    for (size_t i = 0; i < cont_merge.size(); ++i) {
        std::cout << cont_merge[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "----- ***mergeLS//" << std::endl << std::endl;
}
