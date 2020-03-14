#include "utils.h"
#include <assert.h>

void readData(vector<long>& nums)
{
	ifstream data("mwis.txt");
	if (data.is_open())
	{
		string line;
		getline(data, line);
		const int n = stoi(line);
		nums.reserve(n);
		while (getline(data, line))
		{
			long value = stol(line);
			nums.push_back(value);
		}
	}
	else {
		cout << "Fail to open file: mwis.txt" << endl;
	}
}

vector<bool> reconstruct(vector<long>& nums, vector<long long>& dp)
{
	vector<bool> res(nums.size(), false);
	for (int i = static_cast<int>(dp.size()) - 1; i > 1; )
	{
		if (dp[i - 1] >= dp[i - 2] + nums[i - 1]) {
			i--;
		} else {
			res[i - 1] = true;
			i -= 2;
		}
	}
	res[0] = !res[1];
	return res;
}

void printAnswer(vector<bool>& res)
{
	//vertices 1, 2, 3, 4, 17, 117, 517, and 997
	cout << res[0] << res[1] << res[2] << res[3];
	cout << res[16] << res[116] << res[516] << res[998] << endl;
}

void findMaximumIndependentSet()
{
	vector<long> nums;
	readData(nums);
	vector<long long> dp(nums.size() + 1, 0);
	dp[1] = nums[0];
	for (int i = 2; i < dp.size(); i++) {
		dp[i] = max(dp[i-1], dp[i-2] + nums[i-1]);
	}
	auto ans = reconstruct(nums, dp);
	printAnswer(ans);
}