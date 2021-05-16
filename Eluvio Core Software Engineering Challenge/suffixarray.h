#pragma once
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

/*
	Suffix-Array Implementation in C++
	Using Skew & Kasai Algorithim
*/
class SuffixArray
{
private:
	const int maxAlphabetSize = 256;
	std::vector<int> suffixArray;
	std::vector<int> sources;
	int numStrings = 0;
	int sentinelCharacter = -1;

public:
	bool addSourceFromFile(std::string);
	bool addSourceFromString(std::string);
	bool constructSuffixArray();
	std::vector<int> constructLCPArray();
	int findLongestCommonStrand(const unsigned int, std::set<int>&);
	int findSuffixParentSource(int offset);
	int findTrueSuffixOffset(int, int offset);
	void printSuffixArray();

private:
	void skewSuffixArray(const std::vector<int>&, std::vector<int>&, int, int, int);
	void radixSort(std::vector<int>&, std::vector<int>&, const std::vector<int>&, int, int, int, int);

	template <typename T>
	void printVector(std::vector<T> suffixArray);
};

