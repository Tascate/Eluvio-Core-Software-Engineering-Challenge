#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

/*
	Suffix-Array Implementation in C++
	Understanding and overall Implementation from https://zork.net/~st/jottings/sais.html
*/
class SuffixArray
{
private:
	struct Byte {
		unsigned int byte;
		int location;
		friend std::ostream& operator<<(std::ostream& os, const Byte& b) { //For debug printing
			os << (unsigned char)b.byte;
			return os;
		}
	};
	std::vector<int> suffixArray;
	std::vector<Byte> sources;
	std::vector<unsigned int> sourceLengths;
	unsigned int numStrings = 0;
	const int maxAlphabetSize = 256;
	int sentinelCharacter = 256;

public:
	bool addSource(std::istream&);
	bool addString(std::string);
	bool initializeSuffixArray();
	void printSuffixArray();
	std::vector<int> makeLCPArray();
	void findLongestCommonStrand(int);

private:
	std::vector<int> makeSuffixArray(std::vector<Byte>&, std::vector<int>&);

	std::vector<int> fillBucket(std::vector<Byte>&, int alphabetSize);
	std::vector<bool> fillLTypeArray(const std::vector<Byte>&);
	std::vector<int> findBucketHeads(std::vector<int>&);
	std::vector<int> findBucketTails(std::vector<int>&);

	std::vector<int> guessLMSSort(const std::vector<Byte>&, std::vector<int> &bucketSizes, const std::vector<bool> &Ltype);
	std::vector<int> LMSsort(const std::vector<Byte>&, std::vector<int> &bucketSizes, const std::vector<bool> &Ltype, std::vector<int> &sumArray, std::vector<int> &sumOffsets);
	
	//Returns
	int summariseSuffixArray(const std::vector<Byte>&, std::vector<int>&, const std::vector<bool>&,
								std::vector<Byte>& targetSummarySource,
								std::vector<int>& targetSummaryOffset);
	std::vector<int> makeSummarySuffixArray(std::vector<Byte>&, int);

	void inducedSortL(const std::vector<Byte>&, std::vector<int> &, std::vector<int>&, const std::vector<bool> &);
	void inducedSortS(const std::vector<Byte>&, std::vector<int>&, std::vector<int>&, const std::vector<bool>&);

	bool isLMSChar(unsigned int offset, const std::vector<bool>&);
	bool isLMSSubstringsEqual(const std::vector<Byte>&, const std::vector<bool>&, unsigned int, unsigned int);

	template <typename T>
	void printVector(std::vector<T> suffixArray);

	
	
};

