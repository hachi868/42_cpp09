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
    long longArg;
    if (!(argStream >> longArg) || !argStream.eof()) {
        throw std::runtime_error("Invalid args. (cannot convert long)");
    }
    cont_vec_.push_back(longArg);
    cont_deque_.push_back(longArg);
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
    cont_vec_ = cont_merge;
    //std::cout << "runSortVec: isSorted: " << isSorted(cont_vec_) << std::endl;
//    for (size_t i = 0; i < cont_vec_.size(); ++i) {
//        std::cout << cont_vec_[i] << " ";
//    }
//    std::cout << std::endl;
}

void PmergeMe::runSortDeque() {
    std::deque<long> cont_merge;

    splitIntoPairs(cont_deque_, cont_merge);
    cont_deque_ = cont_merge;
    //std::cout << "runSortDeque: isSorted: " << isSorted(cont_deque_) << std::endl;
//    for (size_t i = 0; i < cont_deque_.size(); ++i) {
//        std::cout << cont_deque_[i] << " ";
//    }
//    std::cout << std::endl;
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
            //余りはfirstを-1にして格納しておく。ソートして頭に来るように。
            cont_pairs.push_back(std::make_pair(-1, cont[i]));
        }
    }

    splitIntoPairs(cont_larger, cont_merge);
    mergeSort(cont_pairs, cont_merge);
    return cont_larger;
}

std::vector<std::vector<std::pair<long, long> > > PmergeMe::splitPairsByJacobsthal(const std::vector<std::pair<long, long> >& cont_pairs) {
    std::vector<std::vector<std::pair<long, long> > > splitedVectors;

    std::vector<long> jacobsthalSequence = generateJacobsthalSequence(cont_pairs.size());
//    std::cout << "///generateJacobsthalSequence: (pair size" << cont_pairs.size() << ") ";
//    printDebug(jacobsthalSequence);

    size_t startIndex = 0;
    for (size_t i = 0; i < jacobsthalSequence.size(); ++i) {
        long size = jacobsthalSequence[i];
        if (startIndex >= cont_pairs.size()) {
            break;
        }
        size_t endIndex = std::min(startIndex + size, cont_pairs.size());
        std::vector<std::pair<long, long> > currentPartition(cont_pairs.begin() + startIndex, cont_pairs.begin() + endIndex);
        splitedVectors.push_back(currentPartition);
        startIndex = endIndex;
    }
    return splitedVectors;
}

void PmergeMe::mergeSort(std::vector<std::pair<long, long> > &cont_pairs, std::vector<long> &cont_merge) {
    //splitIntoPairsのbaseCaseの受け取り
    if (cont_merge.size() == 0) {
        cont_merge.push_back(cont_pairs[0].second);
        if (cont_pairs[0].first > 0) {
            cont_merge.push_back(cont_pairs[0].first);
        }
        return;
    }

    //pair sort
    if (cont_pairs.size() > 1) {
        std::sort(cont_pairs.begin(), cont_pairs.end(), PmergeMe::comparePairs);
    }
    //奇数があればpairの最後に送る。firstはpairのlastのfirstとcont_mergeの最後で大きい方(つまり最大値)と同じにしておく。検索範囲を全体にするため。
    if (cont_pairs[0].first < 0) {
        if (cont_merge[cont_merge.size()-1] < cont_pairs[cont_pairs.size()-1].first) {
            cont_pairs[0].first = cont_pairs[cont_pairs.size()-1].first;
        } else {
            cont_pairs[0].first = cont_merge[cont_merge.size()-1];
        }
        std::pair<long, long> elResidue = cont_pairs.front();
        cont_pairs.erase(cont_pairs.begin());
        cont_pairs.push_back(elResidue);
    }
    //
    std::vector<std::vector<std::pair<long, long> > > splitedVectors = splitPairsByJacobsthal(cont_pairs);
    for (size_t p = 0; p < splitedVectors.size(); ++p) {
        const std::vector<std::pair<long, long> > &partition = splitedVectors[p];

        size_t index = 0;
        for (std::vector<std::pair<long, long> >::const_reverse_iterator it = partition.rbegin(); it != partition.rend(); ++it, ++index) {
//            std::cout << "// " << it->second << "(L : " << it->first << ")" << std::endl;
            std::vector<long>::iterator it_first = std::find(cont_merge.begin(), cont_merge.end(), it->first);
            if (it_first != cont_merge.end()) {
                // first の位置を基準にするための位置を決定
                size_t pos = std::distance(cont_merge.begin(), it_first);

                // second の挿入位置を見つける
                if (pos <= cont_merge.size()) {
                    // second の挿入位置を見つける
                    std::vector<long>::iterator it_insert = std::lower_bound(cont_merge.begin(), cont_merge.begin() + pos, it->second);
                    cont_merge.insert(it_insert, it->second);
                } else {
                    std::cout << "Invalid position: " << pos << std::endl;
                }
            } else {
                std::cout << it->first << " not found in cont_merge" << std::endl;
            }
        }
    }
//    std::cout << "///mergeSort: " << std::endl;
//    std::cout << "cont_merge: ";
//    printDebug(cont_merge);
//    std::cout << "cont_pairs: " << std::endl;
//    printDebugPair(cont_pairs);
//    std::cout << "----- ***mergeSort//" << std::endl << std::endl;
}

bool PmergeMe::isSorted(const std::vector<long> &cont) {
    if (cont.size() <= 1) {
        return true;
    }
    for (size_t i = 1; i < cont.size(); ++i) {
        if (cont[i] < cont[i - 1]) {
            return false;
        }
    }
    return true;
}

std::deque<long> PmergeMe::splitIntoPairs(std::deque<long> &cont, std::deque<long> &cont_merge) {
    std::deque<long> cont_larger;
    std::deque<std::pair<long, long> > cont_pairs;

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
            //余りはfirstを-1にして格納しておく。ソートして頭に来るように。
            cont_pairs.push_back(std::make_pair(-1, cont[i]));
        }
    }

    splitIntoPairs(cont_larger, cont_merge);
    mergeSort(cont_pairs, cont_merge);
    return cont_larger;
}

std::vector<std::deque<std::pair<long, long> > > PmergeMe::splitPairsByJacobsthal(const std::deque<std::pair<long, long> >& cont_pairs) {
    std::vector<std::deque<std::pair<long, long> > > splitedVectors;

    std::vector<long> jacobsthalSequence = generateJacobsthalSequence(cont_pairs.size());
//    std::cout << "///generateJacobsthalSequence: (pair size" << cont_pairs.size() << ") ";
//    printDebug(jacobsthalSequence);

    size_t startIndex = 0;
    for (size_t i = 0; i < jacobsthalSequence.size(); ++i) {
        long size = jacobsthalSequence[i];
        if (startIndex >= cont_pairs.size()) {
            break;
        }
        size_t endIndex = std::min(startIndex + size, cont_pairs.size());
        std::deque<std::pair<long, long> > currentPartition(cont_pairs.begin() + startIndex, cont_pairs.begin() + endIndex);
        splitedVectors.push_back(currentPartition);
        startIndex = endIndex;
    }
    return splitedVectors;
}

void PmergeMe::mergeSort(std::deque<std::pair<long, long> > &cont_pairs, std::deque<long> &cont_merge) {
    //splitIntoPairsのbaseCaseの受け取り
    if (cont_merge.size() == 0) {
        cont_merge.push_back(cont_pairs[0].second);
        if (cont_pairs[0].first > 0) {
            cont_merge.push_back(cont_pairs[0].first);
        }
        return;
    }

    //pair sort
    if (cont_pairs.size() > 1) {
        std::sort(cont_pairs.begin(), cont_pairs.end(), PmergeMe::comparePairs);
    }
    //奇数があればpairの最後に送る。firstはpairのlastのfirstとcont_mergeの最後で大きい方(つまり最大値)と同じにしておく。検索範囲を全体にするため。
    if (cont_pairs[0].first < 0) {
        if (cont_merge[cont_merge.size()-1] < cont_pairs[cont_pairs.size()-1].first) {
            cont_pairs[0].first = cont_pairs[cont_pairs.size()-1].first;
        } else {
            cont_pairs[0].first = cont_merge[cont_merge.size()-1];
        }
        std::pair<long, long> elResidue = cont_pairs.front();
        cont_pairs.erase(cont_pairs.begin());
        cont_pairs.push_back(elResidue);
    }
    //
    std::vector<std::deque<std::pair<long, long> > > splitedVectors = splitPairsByJacobsthal(cont_pairs);
    for (size_t p = 0; p < splitedVectors.size(); ++p) {
        const std::deque<std::pair<long, long> > &partition = splitedVectors[p];

        size_t index = 0;
        for (std::deque<std::pair<long, long> >::const_reverse_iterator it = partition.rbegin(); it != partition.rend(); ++it, ++index) {
//            std::cout << "// " << it->second << "(L : " << it->first << ")" << std::endl;
            std::deque<long>::iterator it_first = std::find(cont_merge.begin(), cont_merge.end(), it->first);
            if (it_first != cont_merge.end()) {
                // first の位置を基準にするための位置を決定
                size_t pos = std::distance(cont_merge.begin(), it_first);

                // second の挿入位置を見つける
                if (pos <= cont_merge.size()) {
                    // second の挿入位置を見つける
                    std::deque<long>::iterator it_insert = std::lower_bound(cont_merge.begin(), cont_merge.begin() + pos, it->second);
                    cont_merge.insert(it_insert, it->second);
                } else {
                    std::cout << "Invalid position: " << pos << std::endl;
                }
            } else {
                std::cout << it->first << " not found in cont_merge" << std::endl;
            }
        }
    }
//    std::cout << "///mergeSort: " << std::endl;
//    std::cout << "cont_merge: ";
//    printDebug(cont_merge);
//    std::cout << "cont_pairs: " << std::endl;
//    printDebugPair(cont_pairs);
//    std::cout << "----- ***mergeSort//" << std::endl << std::endl;
}

bool PmergeMe::isSorted(const std::deque<long> &cont) {
    if (cont.size() <= 1) {
        return true;
    }
    for (size_t i = 1; i < cont.size(); ++i) {
        if (cont[i] < cont[i - 1]) {
            return false;
        }
    }
    return true;
}

std::vector<long> PmergeMe::generateJacobsthalSequence(long n) {
    std::vector<long> sequence;

    if (n <= 0) {
        return sequence;
    }

    long i = 1;
    long nextValue = 0;
    while (nextValue < n) {
        nextValue = (std::pow(2, i + 1) + std::pow(-1, i)) / 3;
        sequence.push_back(nextValue);
        i++;
    }
    return sequence;
}
