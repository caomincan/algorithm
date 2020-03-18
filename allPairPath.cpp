#include "utils.h"
#include <assert.h>
using Graph = vector <vector<long long>>;
Graph ReadGraph(const char* file)
{
	cout << "Working on " << file << endl;
	ifstream data(file);
	if (data.is_open()) {
		string line;
		getline(data, line);
		int idx = findSpace(line, 0);
		const int n = stoi(line.substr(0, idx));
		const int m = stoi(line.substr(idx + 1));
		Graph graph(n + 1, vector<long long>(n + 1, INT_MAX));
		while (getline(data, line)) {
			idx = findSpace(line, 0);
			int dst = stoi(line.substr(0, idx));
			line = line.substr(idx + 1);
			idx = findSpace(line, 0);
			int src = stoi(line.substr(0, idx));
			long long w = stoll(line.substr(idx + 1));
			graph[src][dst] = w;
		}

		for (int i = 0; i <= n; i++) {
			graph[i][i] = 0;
		}
		return graph;
	}
	else {
		cout << "Fail to open file: " << file << endl;
		return Graph();
	}
}

bool FloydWarshallAlgorithm(Graph& graph, vector<Graph>& dp)
{
	
	const int n = static_cast<int>(graph.size()) - 1;
	for (int k = 1; k <= n; k++) {				
		for (int i = 1; i <= n; i++){
			for (int j = 1; j <= n; j++) {
				long long preValue = dp[k - 1][i][j];
				long long viaK = dp[k - 1][i][k] + dp[k - 1][k][j];
				dp[k][i][j]= min(preValue, viaK);
				if (i == j && k== n && dp[k][i][j] < 0) return false;
			}
		}
	}
	return true;
}

long long findMinimumPath(vector<Graph>& dp)
{
	const int n = static_cast<int>(dp.size()) - 1;
	long long path = INT_MAX;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i == j) continue;
			if (dp[n][i][j] < path) {
				path = dp[n][i][j];
			}
		}
	}
	return path;
}

void computerAllPairPath(const char* file)
{
	auto graph = ReadGraph(file);
	vector<Graph> dp(graph.size(), graph);
	if (FloydWarshallAlgorithm(graph,dp)) {
		long long minPath = findMinimumPath(dp);
		cout << "Minimum path: " << minPath << endl;
	}
	else {
		cout << file << " contains negative cycle!" << endl;
	}
}