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

bool FloydWarshallAlgorithm(Graph& graph)
{
	
	const int n = static_cast<int>(graph.size()) - 1;
	for (int k = 0; k <= n; k++) {	
		auto dp = graph;
		for (int i = 1; i <= n; i++){
			for (int j = 1; j <= n; j++) {
				long long preValue = dp[i][j];
				long long viaK = dp[i][k] + dp[k][j];
				graph[i][j]= min(preValue, viaK);
				if (i == j && graph[i][j] < 0) return false;
			}
		}
	}
	return true;
}

long long findMinimumPath(Graph& graph)
{
	const int n = static_cast<int>(graph.size()) - 1;
	long long path = INT_MAX;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i == j) continue;
			if (graph[i][j] < path) {
				path = graph[i][j];
			}
		}
	}
	return path;
}

void computerAllPairPath(const char* file)
{
	auto graph = ReadGraph(file);
	if (FloydWarshallAlgorithm(graph)) {
		long long minPath = findMinimumPath(graph);
		cout << "Minimum path: " << minPath << endl;
	}
	else {
		cout << file << " contains negative cycle!" << endl;
	}
}