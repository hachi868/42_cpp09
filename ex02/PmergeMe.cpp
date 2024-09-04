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
    long uIntArg;
    if (!(argStream >> uIntArg) || !argStream.eof()) {
        throw std::runtime_error("Invalid args. (cannot convert long)");
    }
    cont_vec_.push_back(uIntArg);
    cont_deque_.push_back(uIntArg);
}

void PmergeMe::printContainers(std::string prefix) const {
    //オーバーヘッド的に軽量と思われるvectorを使用
    std::cout << prefix << ": ";
    printDebug(cont_vec_);
}

void PmergeMe::printDebug(const std::vector<long> &cont) const {
    for (std::vector<long>::const_iterator it = cont.begin(); it != cont.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}


void PmergeMe::printDebugPair(const std::vector<std::pair<long, long> > &pair) const {
    for (size_t i = 0; i < pair.size(); ++i) {
        std::cout << "(" << pair[i].first << ", " << pair[i].second << ") ";
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
    std::vector<long> cont_merge;

    splitIntoPairs(cont_vec_, cont_merge);

    std::cout << "cont_sorted: ";
    for (size_t i = 0; i < cont_merge.size(); ++i) {
        std::cout << cont_merge[i] << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::runSortDeque() {
}


bool PmergeMe::comparePairs(const std::pair<long, long>& a, const std::pair<long, long>& b) {
    return a.first < b.first;
}

std::vector<long> PmergeMe::splitIntoPairs(std::vector<long> &cont, std::vector<long> &cont_merge) {
    std::vector<long> cont_larger;
    std::vector<std::pair<long, long> > cont_pairs;

    if (cont.size() <= 1) {
        if (!cont.empty()) {
            cont_larger.push_back(cont[0]);
        }
        return cont_larger;
    }

    for (size_t i = 0; i < cont.size(); i += 2) {
        if (i + 1 < cont.size()) {
            long first = cont[i];
            long second = cont[i + 1];
            if (first < second) {
                std::swap(first, second);
            }
            cont_larger.push_back(first);
            cont_pairs.push_back(std::make_pair(first, second));
        } else {
            cont_larger.push_back(cont[i]);
            cont_pairs.push_back(std::make_pair(cont[i], -1));
        }
    }

    splitIntoPairs(cont_larger, cont_merge);
    std::cout << "///////splitIntoPairs: " << std::endl;
    std::cout << "cont_larger: " << std::endl;
    printDebug(cont_larger);
    std::cout << "cont_merge: " << std::endl;
    printDebug(cont_merge);
    std::cout << "cont_pairs: " << std::endl;
    printDebugPair(cont_pairs);

    margeSort(cont_pairs, cont_larger, cont_merge);

    std::cout << "----- ***splitIntoPairs//" << std::endl << std::endl;
    return cont_larger;
}

void PmergeMe::margeSort(std::vector<std::pair<long, long> > &cont_pairs, std::vector<long> &cont_larger, std::vector<long> &cont_merge) {
    (void)cont_larger;

    std::cout << "/////cont_merge.size: " << cont_merge.size() << std::endl;
    if (cont_merge.size() == 0) {
        cont_merge.push_back(cont_pairs[0].second);
        cont_merge.push_back(cont_pairs[0].first);
    }

    //todo:pair sort
    if (cont_pairs.size() > 1) {
        std::sort(cont_pairs.begin(), cont_pairs.end(), PmergeMe::comparePairs);
    }

    std::cout << "///////margeSort: " << std::endl;
//    std::cout << "cont_larger: " << std::endl;
//    printDebug(cont_larger);
    std::cout << "cont_merge: " << std::endl;
    printDebug(cont_merge);
    std::cout << "cont_pairs: " << std::endl;
    printDebugPair(cont_pairs);

//    size_t i = 0, j = 0;
//    while (i < cont_smaller.size() && j < cont_larger.size()) {
//        if (cont_smaller[i] < cont_larger[j]) {
//            cont_merge.push_back(cont_smaller[i]);
//            std::cout << "//merge1: " << cont_smaller[i] << " (::" << cont_larger[j] << std::endl;
//            i++;
//        } else {
//            cont_merge.push_back(cont_larger[j]);
//            std::cout << "//merge2: " << cont_larger[j] << std::endl;
//            j++;
//        }
//    }

    // 残りの要素を追加
//    while (i < cont_smaller.size()) {
//        cont_merge.push_back(cont_smaller[i]);
//        std::cout << "//merge3: " << cont_smaller[i] << std::endl;
//        i++;
//    }
//
//    while (j < cont_larger.size()) {
//        cont_merge.push_back(cont_larger[j]);
//        std::cout << "//merge4: " << cont_larger[j] << std::endl;
//        j++;
//    }
    //todo: ここまでにcont_pairsで渡された要素がcont_mergeに並ぶ
    std::cout << "----- ***margeSort//" << std::endl << std::endl;
}

