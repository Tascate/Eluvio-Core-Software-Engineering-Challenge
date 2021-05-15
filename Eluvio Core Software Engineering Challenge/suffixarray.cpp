#include "suffixarray.h"

bool SuffixArray::addSource(std::istream &in)
{
	//Read in all data from a stream
	unsigned int letter;
	while (in >> letter)
	{
		sources.push_back(letter);
	}
	//Sentinel Character attached to end of data
	//Used in Suffix Array and LCP Construction
	sources.push_back(sentinelCharacter--);
	++numStrings;
	return true;
}

bool SuffixArray::addString(const std::string inputString)
{
	for (auto it = inputString.cbegin(); it != inputString.cend(); it++) {
		sources.push_back(*it);
	}
	//Sentinel Character attached to end of data
	//Used in Suffix Array and LCP Construction
	sources.push_back(sentinelCharacter--);
	++numStrings;
	return true;
}

bool SuffixArray::initializeSuffixArray()
{
	suffixArray.resize(sources.size());
	std::vector<int> sourcesCopy(sources);

	//Skew Algorithim Implementation assumption
	//See function declaration comments
	sourcesCopy.push_back(sentinelCharacter);
	sourcesCopy.push_back(sentinelCharacter);
	sourcesCopy.push_back(sentinelCharacter);
	skewSuffixArray(sourcesCopy, suffixArray, sources.size(), maxAlphabetSize, numStrings+1);
	return true;
}

void SuffixArray::printSuffixArray()
{
	std::cout << "Suffix Array : ";
	printVector(suffixArray);
}

/*
	Skew-Algorithim for constructing Suffix Arrays of an integer alphabet given a source from s[1...n]
	Assumes s[n] = s[n+1] = s[n+2] = unused sentinelCharacter
	Supports for a source consisting of different sentinel characters (for multiple given sources)
*/
void SuffixArray::skewSuffixArray(const std::vector<int> &s, std::vector<int> &SA, int length, int alphabetSize, int alphabetOffset)
{
	int n0 = (length + 2) / 3, n1 = (length + 1) / 3, n2 = length / 3, n02 = n0 + n2;
	std::vector<int> s12(n02 + 3); s12[n02] = s12[n02 + 1] = s12[n02 + 2] = sentinelCharacter;
	std::vector<int> SA12(n02 + 3); SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = sentinelCharacter;
	std::vector<int> s0(n0);
	std::vector<int> SA0(n0);

	//Indexes of Triples for s12
	for (int i = 0, j = 0; i < length + (n0 - n1); i++)
		if (i % 3 != 0)
			s12[j++] = i;

	//RadixSort to construct SA12
	radixSort(s12, SA12, s, 2, n02, alphabetSize, alphabetOffset);
	radixSort(SA12, s12, s, 1, n02, alphabetSize, alphabetOffset);
	radixSort(s12, SA12, s, 0, n02, alphabetSize, alphabetOffset);

	int name = 0, c0 = -1, c1 = -1, c2 = -1;

	for (int i = 0; i < n02; i++)
	{
		if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2)
		{
			name++;
			c0 = s[SA12[i]]; c1 = s[SA12[i] + 1]; c2 = s[SA12[i] + 2];
		}
		if (SA12[i] % 3 == 1)
			s12[SA12[i] / 3] = name;
		else
			s12[SA12[i] / 3 + n0] = name;
	}

	//Create Suffix Array for s12
	//recursion if names are not unique
	if (name < n02)
	{
		skewSuffixArray(s12, SA12, n02, name, alphabetOffset);
		for (int i = 0; i < n02; i++)
			s12[SA12[i]] = i + 1;
	}
	else
		for (int i = 0; i < n02; i++)
			SA12[s12[i] - 1] = i;
	
	//construct suffix array for s0 and sort
	for (int i = 0, j = 0; i < n02; i++)
		if (SA12[i] < n0)
			s0[j++] = 3 * SA12[i];
	radixSort(s0, SA0, s, 0, n0, alphabetSize, alphabetOffset);

	//merge SA0 and SA12 suffixes
	auto leqPair = [](int a1,int a2, int b1, int b2)->bool{
		return a1 < b1 || a1 == b1 && a2 <= b2;
	};
	auto leqTriple = [leqPair](int a1, int a2, int a3, int b1, int b2, int b3)->bool {
		return a1 < b1 || a1 == b1 && leqPair(a2,a3,b2,b3);
	};
	for (int p = 0, t = n0 - n1, k = 0; k < length; k++)
	{
		int i = SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2; //offset os 12 suffix
		int j = SA0[p]; //offset of 0 suffix
		if (SA12[t] < n0 ? leqPair(s[i],s12[SA12[t]+n0],s[j],s12[j/3]) : 
			leqTriple(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0]))
		{
			SA[k] = i;
			t++;
			if (t == n02)
				for (k++; p < n0; p++, k++) SA[k] = SA0[p];
		}
		else
		{
			SA[k] = j;
			p++;
			if (p == n0)
				for (k++; t < n02; t++, k++)
					SA[k] = SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2;
		}
	}
}

/*
	RadixSort
	From Skew Algorithim Implementation
	Allows for alphabet that includes negative integers
		Assumes alphabetOffset offsets the smallest character of the alphabet to 0
*/
void SuffixArray::radixSort(std::vector<int>& a, std::vector<int>& b, const std::vector<int> r, int offset, int n, int alphabetSize, int alphabetOffset) 
{
	std::vector<int> count(alphabetSize + 1+alphabetOffset,0);
	for (int i = 0; i < n; i++)
		count[r[a[i]+offset]+alphabetOffset]++;
	for (int i = 0, sum = 0; i <= alphabetSize+alphabetOffset; i++)
	{
		int t = count[i];
		count[i] = sum;
		sum += t;
	}
	for (int i = 0; i < n; i++)
		b[count[r[a[i]+offset]+alphabetOffset]++] = a[i];
}

int SuffixArray::findSuffixOriginSource(int offset)
{
	//Given offset is out of range
	if (offset > suffixArray[0])
	{
		std::cerr << "Out of range offset given for locateSuffx(): " << offset << std::endl;
		return -1;
	}
	//Trivial answer
	if (numStrings <= 1)
		return numStrings;
	int high = numStrings - 1;
	int low = 0;
	int mid = 0;

	//Binary Search for closest value
	while (low <= high)
	{
		mid = low + (high - low) / 2;
		if (suffixArray[mid] == offset)
			return numStrings - mid;
		if (suffixArray[mid] > offset)
			low = mid + 1;
		else
			high = mid - 1;
	}
	//Edge-case where offset is greater then closest value
	if (offset > suffixArray[mid])
		mid--;
	return numStrings - mid;

}

/*
	Constructs LCP Array using Kasai's Algorithim
*/
std::vector<int> SuffixArray::makeLCPArray()
{
	int length = suffixArray.size();
	std::vector<int> lcp(length, 0);
	std::vector<int> invertedSuffixArray(length, 0);

	int l = 0;
	//Create Inverted Suffix Array
	for (int i = 0; i < length; i++)
		invertedSuffixArray[suffixArray[i]] = i;

#ifdef _DEBUG
	std::cout << "- Constructing LCP Array -" << std::endl;
	std::cout << "Inverted Suffix Array:" << std::endl;
	printVector(invertedSuffixArray);
	std::cout << "Iterating LCP Array: " << std::endl;
#endif 

	for (int i = 0; i < length - 1; i++)
	{
		int k = invertedSuffixArray[i];
		int j = suffixArray[k - 1];
		while (sources[i + l] == sources[j + l])
			l += 1;
		lcp[k] = l;
		if (l > 0)
			l -= 1;

#ifdef _DEBUG
		printVector(lcp);
#endif
	}
#ifdef _DEBUG
	std::cout << "Finished LCP Array Construction\n" << std::endl;
#endif
	return lcp;
}

/*
	Finds Longest Common Strand that appears in k substrings
	Returns length of LCS and saves offsets to a set
*/
int SuffixArray::findLongestCommonStrand(const int k, std::set<int> &offsets)
{
	if (k < 2)
	{
		std::cout << "Given k-value invalid: " << k << std::endl;
	}
	std::vector<int> lcp = makeLCPArray();
	unsigned int length = lcp.size();
	unsigned int index1 = numStrings; //sliding window, first index
	unsigned int index2 = numStrings; //sliding window, second index
	std::map<int, int> suffixSourcesMap; //Tracks # of suffixes for multiple strings in the sliding window
	auto decrementIndex = [](std::map<int, int> &map, int index) {
		if (map[index] - 1 <= 0)
			map.erase(index);
		else
			map[index] -= 1;
	};
	suffixSourcesMap[findSuffixOriginSource(suffixArray[index1])] += 1;

	int max = 0;
	int currentMinIndex = 1; //index for LCP minimum value of sliding window from index1+1 to index2
	while (index1 < length && index2 < length)
	{
		int suffix1 = suffixArray[index1];
		int suffix2 = suffixArray[index2];
		if (suffixSourcesMap.size() >= k) //We are looking for K-unqiue strings in our sliding window
		{
			//Only need to check if the first byte of the suffix is equal
			if (sources[suffix1] == sources[suffix2])
			{
				//Update offsets and max
				if (lcp[currentMinIndex] > max)
				{
					max = lcp[currentMinIndex];
					offsets.clear();
					offsets.insert(suffix1);
					offsets.insert(suffix2);
				}
				//Edge-case if the LCS has two unique answers of same length
				else if (lcp[currentMinIndex] == max)
				{
					offsets.insert(suffix1);
					offsets.insert(suffix2);
				}
			}
			//Reduce sliding window
			decrementIndex(suffixSourcesMap, findSuffixOriginSource(suffix1));
			/*
				Update currentMinIndex to new sliding window
				currentMinIndex may be set outside sliding window
					this will only happen when window1 == window2
				sliding window will catch up
			*/
			if (++index1 == currentMinIndex) {
				currentMinIndex = index1 + 1;
				for (int i = index1 + 2; i <= index2; i++)
					if (lcp[currentMinIndex] > lcp[i])
						currentMinIndex = i;
			}
		}
		//Enlarge sliding window
		else if (++index2 + 1 < length) {
			suffixSourcesMap[findSuffixOriginSource(suffixArray[index2])] += 1;
			if (lcp[index2] <= lcp[currentMinIndex])
				currentMinIndex = index2;
		}
	}
	
	//Look at any missed lcp values
	int suffix2 = *suffixArray.crbegin();
	while (++index1 < length && suffixSourcesMap.size() >= k)
	{
		int suffix1 = suffixArray[index1];
		decrementIndex(suffixSourcesMap, findSuffixOriginSource(suffix1-1));
		currentMinIndex = index1 + 1;
		for (int i = index1 + 1; i <= index2; i++)
			if (lcp[currentMinIndex] > lcp[i])
				currentMinIndex = i;
		//Only need to check if the first byte of the suffix is equal
		if (sources[suffix1] == sources[suffix2])
		{
			//Update offsets and max
			if (lcp[currentMinIndex] > max)
			{
				max = lcp[currentMinIndex];
				offsets.clear();
				offsets.insert(suffix1);
				offsets.insert(suffix2);
			}
			//Edge-case if the LCS has two unique answers of same length
			else if (lcp[currentMinIndex] == max)
			{
				offsets.insert(suffix1);
				offsets.insert(suffix2);
			}
		}
	}
	return max;
}

template <typename T>
void SuffixArray::printVector(std::vector<T> v) {
	for (auto i = v.begin(); i != v.end(); i++)
	{
		std::cout << *i << ' ';
	}
	std::cout << std::endl;
}
