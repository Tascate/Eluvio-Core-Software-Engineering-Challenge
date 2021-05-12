// Eluvio Core Software Engineering Challenge.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include "suffixarray.h"

std::string fileName = "sample";
int numFiles = 10;
std::vector<int> testSuffixArray();
void printVector(std::vector<int>);

int main()
{
    std::cout << "Hello World!\n";
    std::vector<int> test = testSuffixArray();
    //std::vector<bool> hi(200);
    //std::cout << hi.size() << std::endl;
    printVector(test);
    SuffixArray test1;
    test1.addString("cabbage");
    test1.addString("idontevenknow");
    test1.initializeSuffixArray();
    test1.printSuffixArray();
    std::vector<int> lcp = test1.makeLCPArray();
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
    for (int i = 0 ; i < str.length(); i++)
    {
        suffixes.push_back(str.substr(i, str.length()));
    }
    suffixes.push_back(std::string(""));
    sort(suffixes.begin(), suffixes.end());

    std::vector<int> suffixArray;
    for (auto& suffix : suffixes)
    {
        int offset = str.length() - suffix.length();
        suffixArray.push_back(offset);
    }
    return suffixArray;
}
