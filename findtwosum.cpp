#include "utils.h"

int findNumberOfTwoSum()
{
	fstream data("prob-2-Sum.txt");
	if (data.is_open()) {
		string line;
		unordered_map<long, unordered_set<int>> nums;
		unordered_set<int> count;
		while (getline(data, line)) {
			long long number = stoll(line);
			long key = static_cast<long>(number / 10000);
			int value = static_cast<int>(number % 10000);
			auto it = nums.find(key);
			if (it == nums.end()) {
				unordered_set<int> tmp;
				tmp.insert(value);
				nums.insert(make_pair(key, tmp));
			} else {
				it->second.insert(value);
			}
		}
		cout << " Total number pairs: " << nums.size() << endl;

		for (auto p1 = nums.begin(); p1 != nums.end(); p1++)
		{
			for (int target = -1; target <= 1; target++)
			{
				long to_find = target - p1->first;
				auto p2 = nums.find(to_find);
				if (p2 != nums.end()) {
					for (auto x : p1->second) {
						for (auto y : p2->second) {
							long long realX = static_cast<long long>(p1->first) * 10000 + x;
							long long realY = static_cast<long long>(p2->first) * 10000 + y;
							long long sum = realX + realY;
							if (sum <= 10000 && sum >= -10000) count.insert(static_cast<int>(sum));
						}
					}
				}
			}
		}
		return static_cast<int>(count.size());
	}else {
		cout << "Fail to open file!" << endl;
	}
	return -1;
}