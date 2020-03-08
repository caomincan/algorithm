#include "utils.h"
#include <assert.h>

using namespace Test;

void CalculateMedianFromStream()
{
	ifstream data("Median.txt");
	Heap maxHeap(true);  // take care left side
	Heap minHeap(false); // take care right side
	int medianMod = 0;
	if (data.is_open()){
		string line;
		while (getline(data, line))
		{
			int value = stoi(line);
			int maxLeft = maxHeap.max();
			if (maxLeft == -1) {
				// first value;
				maxHeap.push(value);
			}else if (value <= maxLeft) {
				maxHeap.push(value);
			} else {
				minHeap.push(value);
			}
			// keep balance
			int totalnumber = maxHeap.size() + minHeap.size();
			int diff = maxHeap.size() - minHeap.size();
			while (diff > 1 || diff <= -1) {
				if (maxHeap.size() - minHeap.size() > 1) {
					// move elements to right
					int tmp = maxHeap.max();
					maxHeap.popMax();
					minHeap.push(tmp);
				} else {
					int tmp = minHeap.min();
					minHeap.popMin();
					maxHeap.push(tmp);
				}
				diff = maxHeap.size() - minHeap.size();
			}
			assert(diff == 1 || diff == 0);
			// find out current median;
		    medianMod +=  maxHeap.max();
			medianMod = medianMod % 10000;
		}
		cout << "sum of these 10000 medians, modulo 10000 = " << medianMod << endl;
	}
	else {
		cout << "Fail to open file!" << endl;
	}
}