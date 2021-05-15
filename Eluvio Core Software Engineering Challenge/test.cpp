#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include "suffixarray.h"

std::vector<int> testSuffixArray();
void printVector(std::vector<int>);

int test()
{
    std::vector<int> firstSA = testSuffixArray();
    printVector(firstSA);

    SuffixArray test1;
    test1.addString("horihjiwuoerjhuweihuiewhnuiwehuiewuiyweuiweuihuihuiwehruiewhyiurendkjzsnbva");
    test1.addString("aorihjiwuoerjhuweihuiewhnuiwehuiewuiyweuiweuihuihuiwehruiewhyiurendkjzsnbve");


    test1.initializeSuffixArray();
    test1.printSuffixArray();
    std::vector<int> lcp = test1.makeLCPArray();

    std::set<int> result;
    int max = test1.findLongestCommonStrand(2, result);
    std::cout << "Length of LCS: " << max << std::endl;
    for (auto it = result.cbegin(); it != result.cend(); it++)
    {
        int origin = test1.findSuffixOriginSource(*it);
        std::cout << "File " << origin << ", Offset: " << test1.findTrueSuffixOffset(origin, *it) << std::endl;
    }
}

void printVector(std::vector<int> v)
{
    for (auto i = v.begin(); i != v.end(); i++)
    {
        std::cout << *i << ' ';
    }
    std::cout << std::endl;
}

std::vector<int> testSuffixArray()
{
    std::string str = "cabbage";
    std::vector<std::string> suffixes;
    for (unsigned int i = 0; i < str.length(); i++)
    {
        suffixes.push_back(str.substr(i, str.length()));
    }
    suffixes.push_back(std::string(""));
    sort(suffixes.begin(), suffixes.end());

    std::vector<int> suffixArray;
    for (auto& suffix : suffixes)
    {
        size_t offset = str.length() - suffix.length();
        suffixArray.push_back(offset);
    }
    return suffixArray;
}