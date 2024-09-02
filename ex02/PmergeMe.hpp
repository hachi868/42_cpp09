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

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

private:
    std::vector<unsigned int> cont_vec_;
    std::deque<unsigned int> cont_deque_;

    void splitIntoPairs(std::vector<unsigned int> &cont_org, std::vector<unsigned int> &cont_larger, std::vector<unsigned int> &cont_smaller);
};


#endif //P_MERGE_ME_HPP
