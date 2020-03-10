#include "utils.h"

using namespace std;

enum VISIT {
	YES, NO
};

using GRAPH = vector<vector<long>>;

bool compareL(long x, long y) {
	return x > y;
}

long label = 0;
long leadNode = -1;


void dfs(GRAPH& g, const long current, vector<VISIT>& memo, vector<long>* leaders = nullptr, vector<long>* f = nullptr) {
	memo[current] = YES;
	if(leaders) (*leaders)[current] = leadNode;
	auto edges = g[current];
	if (edges.empty()) return;
	for (auto n : edges) {
		if (memo[n] == NO) {
			dfs(g, n, memo, leaders, f);
		}
	}
	label++;
	if (f) { (*f)[current] = label; }
}

void dfs_loop(GRAPH& g, vector<long>* leaders, vector<long>* f, const long maxNumNodes) {
	vector<VISIT> memo(maxNumNodes + 1, NO);
	for (long i = maxNumNodes; i >= 1; i--) {
		if (memo[i] == NO) {
			leadNode = i;
			dfs(g, i, memo, leaders, f);
		}
	}
}

void dfs_loop(GRAPH& g, vector<long>* leaders, unordered_map<long, long>& orders, const long maxNumNodes) {
	vector<VISIT> memo(maxNumNodes + 1, NO);
	for (long i = maxNumNodes; i >= 1; i--) {
		long idx = orders[i];
		if (memo[idx] == NO) {
			leadNode = idx;
			dfs(g, idx, memo, leaders, nullptr);
		}
	}
}

void findSCC(GRAPH& g, GRAPH& g_reverse, vector<long>& leaders, const long maxNumNodes) {
	vector<long> f(maxNumNodes + 1, -1);
	// count leaders in reversed graph
	dfs_loop(g_reverse, nullptr, &f, maxNumNodes);
	// reset global variables
	leadNode = -1;
	label = 0;
	// find next order copy fDst to fSrc
	unordered_map<long, long> orders;
	for (int i = 1; i <= maxNumNodes; i++) {
		int new_idx = f[i];
		if (new_idx != -1) orders[new_idx] = i;
	}
	// second run of graph
	dfs_loop(g, &leaders, orders, maxNumNodes);
}


void solve_scc_problem() {

	const long num = 875714;
	GRAPH graph(num+1, vector<long>());
	GRAPH graphRev(num + 1, vector<long>());

	ifstream data("SCC.txt");
	long max = 0;
	long max_v = 0;
	if (data.is_open()) {
		string line;
		while (getline(data, line)) {
			auto idx = line.find_first_of(' ');
			auto key = stol(line.substr(0, idx));
			auto value = stol(line.substr(idx + 1));
			graph[key].push_back(value);
			graphRev[value].push_back(key);
		}
	}
	cout << "graph size: " << graph.size() << endl;
	
	vector<long> leaders(num + 1, -1);
	findSCC(graph, graphRev, leaders, num);
	// sorting leaders;
	vector<long> counter(num+1, 0);
	for (int i = 1; i <= num; i++) {
		if (leaders[i] != -1) {
			counter[leaders[i]]++;
		}
	}
	cout << "First 5 larger SCC: ";
	sort(counter.begin(), counter.end(), compareL);
	for (int i = 0; i < 5; i++) {
		if (i < counter.size()) cout << counter[i] << " ";
		else cout << "0 ";
	}
	cout << endl;
	
}