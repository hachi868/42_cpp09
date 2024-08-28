#ifndef P_MERGE_ME_HPP
#define P_MERGE_ME_HPP

#include <iostream>
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
    void runSort();

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

private:
    std::vector<unsigned int> cont_vec_;
    std::deque<unsigned int> cont_deque_;
};


#endif //P_MERGE_ME_HPP
