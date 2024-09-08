#ifndef P_MERGE_ME_HPP
#define P_MERGE_ME_HPP

#include <iostream>
#include <sstream>
//#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>

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
    void printDebugPair(const std::vector<std::pair<long, long> > &pair) const;

    const static std::string RESET;
    const static std::string DEBUG;
    const static std::string STATE;
    const static std::string ALERT;
    const static std::string MSG;
    const static std::string ERROR;

private:
    std::vector<long> cont_vec_;
    std::deque<long> cont_deque_;

    //vector
    std::vector<long> splitIntoPairs(std::vector<long> &cont, std::vector<long> &cont_merge);
    std::vector<std::vector<std::pair<long, long> > > splitPairsByJacobsthal(const std::vector<std::pair<long, long> >& cont_pairs);
    void mergeSort(std::vector<std::pair<long, long> > &cont_pairs, std::vector<long> &cont_merge);
    bool isSorted(const std::vector<long> &cont);

    //deque
    std::deque<long> splitIntoPairs(std::deque<long> &cont, std::deque<long> &cont_merge);
    std::vector<std::deque<std::pair<long, long> > > splitPairsByJacobsthal(const std::deque<std::pair<long, long> >& cont_pairs);
    void mergeSort(std::deque<std::pair<long, long> > &cont_pairs, std::deque<long> &cont_merge);
    bool isSorted(const std::deque<long> &cont);

    static bool comparePairs(const std::pair<long, long>& a, const std::pair<long, long>& b);

    std::vector<long> generateJacobsthalSequence(long n);
};

#endif //P_MERGE_ME_HPP
