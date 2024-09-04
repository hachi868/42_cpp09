#ifndef P_MERGE_ME_HPP
#define P_MERGE_ME_HPP

#include <iostream>
#include <sstream>
//#include <algorithm>
#include <vector>
#include <deque>

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe &obj);
    PmergeMe &operator = (const PmergeMe &obj);
    ~PmergeMe();

    void parser(std::string arg);
    void printContainers(std::string prefix) const;
    bool isContainersEqual() const;
    void runSortVec();
    void runSortDeque();
    void printDebug(const std::vector<long> &cont) const;

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

private:
    std::vector<long> cont_vec_;
    std::deque<long> cont_deque_;

    std::vector<long> splitIntoPairs(std::vector<long> &cont, std::vector<long> &cont_merge);
    void margeSort(std::vector<long> &cont_merge, std::vector<long> &cont_larger, std::vector<long> &cont_smaller);
};


#endif //P_MERGE_ME_HPP
