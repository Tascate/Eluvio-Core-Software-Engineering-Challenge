# Eluvio Core Software Engineering Challenge
The Eluvio Core Software Engineering Challenge gives the task of finding the longest common strand of bytes that appears in at least two files.
## Solution Approach
### Suffix Arrays
This solution uses a **Suffix Array** consisting of suffixes for the entire strand of bytes spanning across all files. Strands are separated by sentinel characters. 

The specific algorithm used is the **Skew Algorithm** that has time complexity **O(n)** and is also modified to support sentinel characters.
### LCP Array
After the Suffix Array is constructed then an **LCP Array** is calculated to assist with finding the longest common strand.

The specific algorithm used is the **Kasai Algorithm** which has time complexity **O(n)**.

In my code, the LCP Array is calculated after the Suffix Array construction but is also capable of being calculated in-place with the **Skew Algorithm**. This would further optimize the program but for ease of coding and understanding, I decided to calculate LCP after.

### Longest Common Strand for k-files
Using two indexes, **i** and **j**, a sliding window over the LCP Array is used. Whenever the sliding window has suffixes for at least **k-files**, the LCP value for the current sliding window is the minimum LCP value from **i+1** to **j**. The maximum LCP value for **k-files** and corresponding offsets are kept track of while iterating.

Finding out the parent file for a given suffix takes **O(logm)** time and **O(1)** space using binary search.

### Time & Space Complexity
For **n = length of combined strand** and **m = total # of files**

Time complexity is O(nlogm) in the worst case; however, m is largely insignificant which means an **O(n)** time complexity

Space Complexity is **O(n)** to store suffix array, inverted suffix array, and the array of bytes.
## Compiling & Running
The project was compiled using Visual Studio 2019. Using my computer with the Release x86 preset, the total computation time averaged around **~95ms** according to the profilier.
1. Download the Project and open Solution in VS2019
2. Run project in the desired configuration
3. Output is given by **length** then **files + their offsets**
