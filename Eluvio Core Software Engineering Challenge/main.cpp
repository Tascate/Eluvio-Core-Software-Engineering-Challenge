// Eluvio Core Software Engineering Challenge.cpp
//

#include <iostream>
#include <fstream>
#include <string>
#include "suffixarray.h"

std::string fileName = "sample.";
int numFiles = 10;

int main()
{
    SuffixArray sa;
    
    for (int i = 1; i <= numFiles; i++)
    {
        std::ifstream fin(fileName + std::to_string(i), std::ios::binary);
        sa.addSource(fin);
        fin.close();
    }
    sa.initializeSuffixArray();
    std::set<int> result;
    int max = sa.findLongestCommonStrand(2,result);
    std::cout << "Length of Longest Common Strand: " << max << std::endl;
    std::cout << "Located in:" << std::endl;
    for (auto it = result.cbegin(); it != result.cend(); it++)
    {
        int origin = sa.findSuffixOriginSource(*it);
        std::cout << fileName << origin << " Offset: " << sa.findTrueSuffixOffset(origin, *it) << std::endl;
    }
    return 0;
}