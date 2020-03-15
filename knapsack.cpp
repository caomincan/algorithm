#include "utils.h"
#include <assert.h>

#define PRINT 0
struct Item {
	Item(int v, int w) : value(v), weight(w) {}
	int value;
	int weight;
};

struct ItemKey {
	ItemKey(int i, int x) : idx(i), W(x)
	{}
	int idx;
	int W;
	friend bool operator==(const ItemKey& k1, const ItemKey& k2) {
		return k1.idx == k2.idx && k1.W == k2.W;
	}
};

namespace std {
	template<>
	struct hash<ItemKey> {
		size_t operator()(const ItemKey& k) const {
			return hash<int>()(k.idx)
				^ (hash<int>()(k.W) << 1);
		}
	};
}

vector<Item> ReadFile(const char* file, int& W)
{
	ifstream data(file);
	vector<Item> res;
	if (data.is_open())
	{
		string line;		
		int idx = 0;
		getline(data, line);
		while (idx < line.size() && line[idx] != ' ') { idx++; }
		W = stoi(line.substr(0, idx));
		const int n = stoi(line.substr(idx + 1));
		res.reserve(n);
		while (getline(data, line)) {
			idx = 0;
			while (idx < line.size() && line[idx] != ' ') { idx++; }
			int value = stoi(line.substr(0, idx));
			int weight = stoi(line.substr(idx+1));
			res.push_back(Item(value, weight));
		}		
	}
	else {
		cout << "Fail to open file: " << file << endl;
	}
	return res;
}

long long KnapSackProblemHelp(vector<Item>& data, int curItem, int curW, unordered_map<ItemKey, long long>& dp)
{
	if (curItem < 0 || curW < 0) return 0;
	if (curItem < 1) {
		auto ans = max((long long)0, KnapSackProblemHelp(data, curItem-1, curW, dp));
		dp.insert(make_pair(ItemKey(curItem, curW), 0));
		return 0;
	}
	const long v = data[curItem - 1].value;
	const long w = data[curItem - 1].weight;
	if (curW < w) {
		auto ans = max((long long)0, KnapSackProblemHelp(data, curItem-1, curW, dp));
		dp.insert(make_pair(ItemKey(curItem, curW), ans));
		return ans;
	}
	auto found = dp.find(ItemKey(curItem, curW));
	if (found != dp.end()) return found->second;
	long long case1 = KnapSackProblemHelp(data, curItem - 1, curW, dp);
	long long case2 = KnapSackProblemHelp(data, curItem - 1, curW - w, dp) + v;
	auto ans = max(case1, case2);
	dp.insert(make_pair(ItemKey(curItem, curW), ans));
	return ans;
}

long long KnapSackProblemRecursive(vector<Item>& data, const int W)
{
	// init
	const int n = static_cast<int>(data.size());
	unordered_map<ItemKey, long long> dp;
#if 0
	for (int x = 0; x <= data[0].weight; x++) {
		for (int i = 1; i <=n ; i++) {
			const long long w = data[i - 1].weight;
			const long long v = data[i - 1].value;
			if (x < w) dp[x][i] = 0;
			else dp[x][i] = max(dp[x][i - 1], v);
		}
	}
#endif
	auto ans = KnapSackProblemHelp(data, n, W, dp);
#if PRINT
	for (int i = W; i >= 0; i--) {
		for (auto j : dp[i]) {
			cout << j << " ";
		}
		cout << endl;
	}
#endif
	return ans;
}
long long KnapSackProblem(vector<Item>& data, const int W)
{
	const int n = static_cast<int>(data.size());
	vector<vector<long long>> dp(W, vector<long long>(n, 0));
	for (int i = 0; i < n; i++) {
		const long v = data[i].value;
		const long w = data[i].weight;
		for (int x = 0; x < W; x++) {			
			long long case1 = (i == 0 ? 0 : dp[x][i - 1]);
			long long case2 = (x+1-w>=0 ? v + (i==0 || x-w <0 ? 0 : dp[x - w][i - 1]) : 0);
			dp[x][i] = max(case1, case2);
		}
	}

#if PRINT
	for (int i = W-1; i >= 0; i--) {
		for (auto j : dp[i]) {
			cout << j << " ";
		}
		cout << endl;
	}
#endif
	return dp[W-1][n-1];
}


void solveKnapSackProblem(const char* file)
{
	int W = 0;
	auto data = ReadFile(file, W);

	//auto ans = KnapSackProblem(data, W);
	auto ans = KnapSackProblemRecursive(data, W);
	cout << "The optimal result is: " << ans << endl;
}