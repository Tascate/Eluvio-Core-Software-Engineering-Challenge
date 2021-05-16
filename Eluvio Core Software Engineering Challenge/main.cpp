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
    
    //Read files in, one by one
    for (int i = 1; i <= numFiles; i++)
    {
        sa.addSourceFromFile(fileName + std::to_string(i));
    }

    //Make Suffix Array
    sa.constructSuffixArray();
    std::set<int> result;

    //Calculate LCS that appears in at least 2 files
    int max = sa.findLongestCommonStrand(2,result);

    //Print results
    std::cout << "Length of Longest Common Strand: " << max << std::endl;
    std::cout << "Located in:" << std::endl;
    for (auto it = result.cbegin(); it != result.cend(); it++)
    {
        int origin = sa.findSuffixParentSource(*it);
        std::cout << fileName << origin << " Offset: " << sa.findTrueSuffixOffset(origin, *it) << std::endl;
    }
    return 0;
}