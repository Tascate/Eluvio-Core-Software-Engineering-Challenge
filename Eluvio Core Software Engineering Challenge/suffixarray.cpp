#include "suffixarray.h"

bool SuffixArray::addSource(std::istream &in)
{
	//Read in all data from a stream
	unsigned int letter;
	unsigned int length = 0;
	while (in >> letter)
	{
		Byte b;
		b.byte = letter;
		b.location = numStrings + 1;
		sources.push_back(b);
		length++;
	}
	//Sentinel Character attached to end of data
	//Used in Suffix Array
	Byte b;
	b.byte = sentinelCharacter++;
	b.location = ++numStrings; //Increment numStrings
	sources.push_back(b);
	sourceLengths.push_back(++length);
	return true;
}

bool SuffixArray::addString(const std::string inputString)
{
	unsigned int length = 0;
	for (auto it = inputString.cbegin(); it != inputString.cend(); it++) {
		Byte b;
		b.byte = *it;
		b.location = numStrings + 1;
		sources.push_back(b);
		length++;
	}
	//Sentinel Character attached to end of data
	//Used in Suffix Array
	Byte b;
	b.byte = sentinelCharacter++;
	b.location = ++numStrings; //Increment numStrings
	sources.push_back(b);

	//Append length
	sourceLengths.push_back(++length);
	return true;
}

bool SuffixArray::initializeSuffixArray()
{
	std::vector<int> bucketSize = fillBucket(sources, maxAlphabetSize+numStrings);
	//Store stream contents and bucketSize for each letter
	suffixArray = makeSuffixArray(sources, bucketSize);
	return false;
}

void SuffixArray::printSuffixArray()
{
	printVector(suffixArray);
}

std::vector<int> SuffixArray::makeSuffixArray(std::vector<Byte>& source, std::vector<int> &bucket)
{
	std::cout << "Beginning to make Suffix Array of ";
	printVector(source);
	std::vector<bool> LType = fillLTypeArray(source);
	printVector(LType);

	std::vector<int> guess = guessLMSSort(source, bucket, LType);
	printVector(guess);
	inducedSortL(source, guess, bucket, LType);
	printVector(guess);
	inducedSortS(source, guess, bucket, LType);
	printVector(guess);

	std::vector<Byte> summarySource;
	std::vector<int> summaryOffset;
	int summaryAlphabetSize = summariseSuffixArray(source, guess, LType, summarySource, summaryOffset);
	printVector(summarySource);
	printVector(summaryOffset);

	std::vector<int> summarySuffixArray = makeSummarySuffixArray(summarySource, summaryAlphabetSize);
	printVector(summarySuffixArray);

	std::vector<int> finalSuffixArray = LMSsort(source, bucket, LType, summarySuffixArray, summaryOffset);
	printVector(finalSuffixArray);
	inducedSortL(source, finalSuffixArray, bucket, LType);
	inducedSortS(source, finalSuffixArray, bucket, LType);
	return finalSuffixArray;
}

std::vector<int> SuffixArray::fillBucket(std::vector<Byte>& source, int alphabetSize)
{
	std::vector<int> bucketSize(alphabetSize, 0);
	for (int i = 0; i < source.size(); i++)
	{
		bucketSize[source[i].byte]++;
	}
	return bucketSize;
}

std::vector<bool> SuffixArray::fillLTypeArray(const std::vector<Byte>& source)
{
	std::vector<bool> target(source.size(), false);
	*target.rbegin() = false;
	*(target.rbegin() + 1) = true;
	if (source.size() == 0)
	{
		return target;
	}
	for (int i = target.size()-3; i >= 0; i--)
	{
		//Special Case if source byte is a sentinelCharacter
		if (source[i].byte >= maxAlphabetSize) {
			target[i] = false;
			target[--i] = true;
		}	
		//L-Type Suffix Criteria
		else if (source[i].byte > source[i + 1].byte)
			target[i] = true;
		else if (source[i].byte == source[i + 1].byte && target[i + 1])
			target[i] = true;
		else
			target[i] = false;
	}
	return target;
}

std::vector<int> SuffixArray::findBucketHeads(std::vector<int>& bucket)
{
	unsigned int offset = 0; //Suffix for Sentinel Characters start at the beginning
	unsigned int length = bucket.size();
	std::vector<int> result(length);
	for (auto i = sentinelCharacter-1; i >= maxAlphabetSize && i < bucket.size(); i--)
	{
		result[i] = offset++;
	}
	for (auto i = 0; i < maxAlphabetSize && i < bucket.size(); i++)
	{
		result[i] = offset;
		offset += bucket[i];
	}
	return result;
}

std::vector<int> SuffixArray::findBucketTails(std::vector<int>& bucket)
{
	unsigned int offset = 0; //Suffix for Sentinel Characters start at the beginning
	unsigned int length = bucket.size();
	std::vector<int> result(length);
	for (auto i = sentinelCharacter - 1; i >= maxAlphabetSize && i < bucket.size(); i--)
	{
		result[i] = offset++;
	}
	for (auto i = 0; i < maxAlphabetSize && i < bucket.size(); i++)
	{
		offset += bucket[i];
		result[i] = offset - 1;
	}
	return result;
}

std::vector<int> SuffixArray::guessLMSSort(const std::vector<Byte>& source, std::vector<int>& bucket, const std::vector<bool>& Ltype)
{
	std::vector<int>guessSuffixArray(source.size(), -1);
	auto bucketTails = findBucketTails(bucket);
	printVector(bucketTails);

	//Bucket sort all LMS suffixes
	for (int i = 0; i < source.size(); i++)
	{
		if (isLMSChar(i, Ltype))
		{
			unsigned int bucketIndex = source[i].byte;
			guessSuffixArray[bucketTails[bucketIndex]] = i;
			bucketTails[bucketIndex] -= 1;
			printVector(guessSuffixArray);
		}
	}
	//Empty suffix goes at front
	guessSuffixArray[0] = source.size()-1;
	return guessSuffixArray;
}

std::vector<int> SuffixArray::LMSsort(const std::vector<Byte>& source, std::vector<int>& bucketSizes, const std::vector<bool>& Ltype, std::vector<int>& sumSuffixArray, std::vector<int>& sumOffsets)
{
	std::vector<int> suffixOffsets(source.size(), -1);
	std::vector<int> bucketTails = findBucketTails(bucketSizes);

	//Reverse order to add suffixes to respective buckets
	for (int i = sumSuffixArray.size() - 1; i > 1; --i)
	{
		int stringIndex = sumOffsets[sumSuffixArray[i]];
		int bucketIndex = source[stringIndex].byte;
		suffixOffsets[bucketTails[bucketIndex]] = stringIndex;
		bucketTails[bucketIndex] -= 1;
	}

	//Empty suffix placement
	suffixOffsets[0] = source.size()-1;
	return suffixOffsets;
}

int SuffixArray::summariseSuffixArray(const std::vector<Byte>& source, std::vector<int>& guessArray, const std::vector<bool>& Ltype,
									  std::vector<Byte>& targetSummarySource,
									  std::vector<int>& targetSummaryOffset)
{
	std::vector<int> lmsNames(source.size(), -1);
	int currentName = 0; //Track allocated names
	//First LMS-substring represents the empty suffix
	lmsNames[guessArray[0]] = currentName;
	int lastLMSSuffixOffset = guessArray[0];

	for (int i = 1; i < guessArray.size(); i++)
	{
		int suffixOffset = guessArray[i];
		if (!isLMSChar(suffixOffset, Ltype))
			continue;
		if (!isLMSSubstringsEqual(source, Ltype, lastLMSSuffixOffset, suffixOffset))
			currentName += 1;
		lastLMSSuffixOffset = suffixOffset;
		lmsNames[suffixOffset] = currentName;
		printVector(lmsNames);

	}

	for (int i = 0; i < lmsNames.size(); i++)
	{
		if (lmsNames[i] != -1)
		{
			targetSummaryOffset.push_back(i);
			Byte b;
			b.byte = lmsNames[i];
			b.location = source[i].location;
			targetSummarySource.push_back(b);
		}
	}
	return currentName + 1;
}

std::vector<int> SuffixArray::makeSummarySuffixArray(std::vector<Byte>& summarySource, int summaryAlphabetSize)
{
	if (summaryAlphabetSize == summarySource.size())
	{
		std::vector<int> summarySuffixArray(summarySource.size()+1);
		summarySuffixArray[0] = summarySource.size();

		for (int i = 0; i < summarySource.size(); i++)
		{
			summarySuffixArray[summarySource[i].byte + 1] = i;
		}
		return summarySuffixArray;
	}
	else {
		std::vector<int> bucket = fillBucket(summarySource, summaryAlphabetSize);
		return makeSuffixArray(summarySource, bucket);
	}
}

void SuffixArray::inducedSortL(const std::vector<Byte>& source, std::vector<int>& guessArray, std::vector<int>& bucket, const std::vector<bool>& Ltype)
{
	auto bucketHeads = findBucketHeads(bucket);

	for (auto i = 0U; i < guessArray.size(); i++)
	{
		//Looking for L-Type
		if (guessArray[i] == -1)
			//No offset
			continue;
		//Look at suffix beginning to left
		int j = guessArray[i] - 1;
		if (j < 0)
			//J is out of bounds
			continue;
		if (!Ltype[j])
			//Offset j is a S-Type suffix
			continue;

		//Offset J is a L-Type suffix
		//Place it at bucket index
		unsigned int bucketIndex = source[j].byte;
		guessArray[bucketHeads[bucketIndex]] = j; //
		bucketHeads[bucketIndex] += 1; //increment head pointer
	}
}

void SuffixArray::inducedSortS(const std::vector<Byte>& source, std::vector<int>& guessArray, std::vector<int>& bucket, const std::vector<bool>& Ltype)
{
	auto bucketTails = findBucketTails(bucket);

	for (long i = guessArray.size()-1; i >= 0; i--)
	{
		int j = guessArray[i] - 1;
		if (j < 0)
			//J is out of bounds
			continue;
		if (Ltype[j])
			//Offset j is a L-Type suffix
			continue;

		//Offset j is a S-Type suffix
		//Place suffix at bucket index
		unsigned int bucketIndex = source[j].byte;
		guessArray[bucketTails[bucketIndex]] = j; //
		bucketTails[bucketIndex] -= 1; //increment head pointer
		printVector(guessArray);
	}
}

bool SuffixArray::isLMSChar(unsigned int offset, const std::vector<bool>& Ltype)
{
	if (offset == 0)
		return false;
	if (!Ltype[offset] && Ltype[offset - 1])
		return true;
	return false;
}

bool SuffixArray::isLMSSubstringsEqual(const std::vector<Byte>& source, const std::vector<bool>& Ltype, unsigned int offset1, unsigned int offset2)
{
	//Substrings not equal if equal to the empty suffix
	if (source[offset1].byte >= maxAlphabetSize || source[offset2].byte >= maxAlphabetSize)
		return false;
	unsigned int i = 0;
	while (true) 
	{
		bool firstIsLMS = isLMSChar(i + offset1, Ltype);
		bool secondIsLMS = isLMSChar(i + offset2, Ltype);

		//True only if no difference is found
		if (i > 0 and firstIsLMS && secondIsLMS)
			return true;
		//Reached end of one substring
		if (firstIsLMS != secondIsLMS)
			return false;
		//If character difference
		if (source[i + offset1].byte != source[i + offset2].byte)
			return false;
		i += 1;
	}
	
}

std::vector<int> SuffixArray::makeLCPArray()
{
	int length = suffixArray.size();
	std::vector<int> lcp(length, 0);
	std::vector<int> invertedSuffixArray(length, 0);

	
	int l = 0;
	for (int i = 0; i < length; i++)
		invertedSuffixArray[suffixArray[i]] = i;
	printVector(invertedSuffixArray);
	for (int i = 0; i < length - 1; i++)
	{
		int k = invertedSuffixArray[i];
		int j = suffixArray[k - 1];
		while (sources[i + l].byte == sources[j + l].byte)
			l += 1;
		lcp[k] = l;
		if (l > 0)
			l -= 1;
		printVector(lcp);
	}
	printVector(lcp);
	return lcp;
}

template <typename T>
void SuffixArray::printVector(std::vector<T> v)
{
	for (auto i = v.begin(); i != v.end(); i++)
	{
		std::cout << *i << ' ';
	}
	std::cout << std::endl;
}
