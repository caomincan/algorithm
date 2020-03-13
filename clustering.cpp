#include "utils.h"
#include "Heap.h"

struct Edge {
	Edge(int s, int d, int w) :
		src(s), dst(d), w(w)
	{}

	Edge() : src(0), dst(0), w(INT_MAX) {}
	int src;
	int dst;
	int w;

	friend bool operator>(const Edge& e1, const Edge& e2) { return e1.w > e2.w; }
	friend bool operator<(const Edge& e1, const Edge& e2) { return e1.w < e2.w; }
	friend bool operator==(const Edge& e1, const Edge& e2) 
	{ 
		return e1.src == e2.src
			&& e1.dst == e2.dst
			&& e1.w == e2.w;
	}
};

struct Node {
	Node(int i, int k):
		id(i), cluster(k), rank(0)
	{}
	int id;
	int cluster;
	int rank;
	friend bool operator==(const Node& n1, const Node& n2) { return n1.id == n2.id; }
};

namespace std {
	template<>
	struct hash<Node> {
		size_t operator()(const Node& n) const {
			return hash<int>()(n.id);
		}
	};
}

using EdgeMap = unordered_map<int, vector<Edge>>;
using Nodes= vector<Node>;

int findLeadingNode(Nodes& nodes, int cur)
{
	if (nodes[cur].id == nodes[cur].cluster) return nodes[cur].id;
	nodes[cur].cluster = findLeadingNode(nodes, nodes[cur].cluster);
	return nodes[cur].cluster;
}
int countClusters(Nodes& nodes, unordered_set<int>& cluster)
{
	for (int i = 1; i < nodes.size(); i++)
	{
		cluster.insert(findLeadingNode(nodes, i));
	}
	return static_cast<int>(cluster.size());
}

void calculateEachClusterDistance(Nodes& nodes, EdgeMap& edges, unordered_set<int>& cluster, vector<vector<int>>& res)
{
	vector<int> c(cluster.begin(), cluster.end());
	sort(c.begin(), c.end());
	auto clusterIdx = [&c](int val) -> int {
		auto it = find(c.begin(), c.end(), val);
		return static_cast<int>(it - c.begin());
	};

	for (auto p : edges)
	{
		for (auto e : p.second) {
			int k1 = findLeadingNode(nodes, e.src);
			int k2 = findLeadingNode(nodes, e.dst);
			if (k1 == k2) continue;
			int k1Idx = clusterIdx(k1);
			int k2Idx = clusterIdx(k2);
			assert(k1Idx != k2Idx);
			if (e.w < res[k1Idx][k2Idx]) {
				res[k1Idx][k2Idx] = e.w;
				res[k2Idx][k1Idx] = e.w;
			}
		}
	}
}

void init(EdgeMap& edges, Nodes& nodes, const int n)
{
	nodes.resize(n + 1, Node(0, 0));
	for (int i = 1; i <= n; i++)
	{
		vector<Edge> tmp;
		edges.insert(make_pair(i, tmp));
		nodes[i] = Node(i, i);
	}
}

void readClusterGraph(EdgeMap& edges, Nodes& nodes, const char* file)
{
	ifstream data(file);
	if (data.is_open()){
		string line;
		getline(data, line);
		const int n = stoi(line); // number nodes
		// init clusters: each node is it is own cluster
		init(edges, nodes, n);
		while (getline(data, line))
		{
			int idx = 0;
			while (idx < line.size() && line[idx] != ' ') { idx++; }
			int src = stoi(line.substr(0, idx));			
			line = line.substr(idx + 1);
		    idx = 0;
			while (idx < line.size() && line[idx] != ' ') { idx++; }
			int dst = stoi(line.substr(0, idx));
			int weight = stoi(line.substr(idx+1));
			// insert into edge map
			edges[src].push_back(Edge(src, dst, weight));
		}

	}
	else {
		cout << "Fail to open file " << file << endl;
	}
}

void findMaxSpacingCluster4()
{
	EdgeMap edges;
	Nodes nodes;
	readClusterGraph(edges, nodes, "clustering1.txt");
	Test::Heap<Edge> heap(false);
	for (auto p : edges){
		for (auto e : p.second) {
			heap.push(e);
		}
	}
	unordered_set<int> cluster;
	for (int i = 1; i < nodes.size(); i++) cluster.insert(i);
	while (cluster.size() > 4 && !heap.isEmpty()) {
		Edge e = heap.min(); heap.popMin();
		int src = findLeadingNode(nodes, e.src);
		int dst = findLeadingNode(nodes, e.dst);
		if ( src != dst) {
			// merge cluster;
			if (nodes[src].rank == nodes[dst].rank) {
				nodes[dst].cluster = src;
				nodes[src].rank++;
				cluster.erase(dst);
			} else if (nodes[src].rank > nodes[dst].rank) {
				nodes[dst].cluster = src;
				cluster.erase(dst);
			} else {
				nodes[src].cluster = dst;
				cluster.erase(src);
			}
		}
	}
	unordered_set<int> clusters;
	int k = countClusters(nodes, clusters);
	assert(k == 4);
	vector<vector<int>> dist(k, vector<int>(k, INT_MAX));
	calculateEachClusterDistance(nodes, edges, cluster, dist);
	int maxDist = INT_MAX;
	for (int i = 0; i < k-1; i++) {
		for (int j = i + 1; j < k;j++ ) {
			if (dist[i][j] < maxDist) {
				maxDist = dist[i][j];
			}
		}
	}

	cout << "Maximum spacing is: " << maxDist<< endl;
}