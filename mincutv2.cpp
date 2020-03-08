#include "utils.h"
#include <ctime>
#include <cstdlib>
#include <sstream>

using uint = unsigned int;

struct Edge {
	Edge(int src, int dst) :
		fSrc(src), fDst(dst)
	{
	}
	~Edge() = default;
	int fSrc;
	int fDst;
};

class Graph {
public:
	Graph(int num) : size(num)
	{
		for (int i = 1; i <= size; i++) fNodes.insert(i);
	}
	~Graph() = default;
	Graph(const Graph& g) = default;

	void addEdge(int src, int dst) {
		fEdges.push_back(Edge(src,dst));
	}

	int getNumEdges() const { return static_cast<int>(fEdges.size()); }

	void contraction() 
	{
		int edgeIdx = rand() % getNumEdges();
		Edge edge = fEdges[edgeIdx];
		// delete node
		fNodes.erase(edge.fDst);
		// keep source and delete dst
		auto it = fEdges.begin();
		while( it != fEdges.end()) {
			// update src when original src be deleted
			if (it->fSrc == edge.fDst) it->fSrc = edge.fSrc;
			// update dst when original dst be deleted
			if (it->fDst == edge.fDst) it->fDst = edge.fSrc;
			// delete self circle
			if (it->fSrc == it->fDst) {
				it = fEdges.erase(it);
			} else {
				it++;
			}
		}
	}

	int findMinimumCut()
	{
		while (fNodes.size() > 2) {
			contraction();
		}
		vector<int> nodes;
		for (auto n : fNodes) {
			nodes.push_back(n);
		}
		int src = nodes[0];
		int count = 0;
		for (auto e : fEdges) {
			if (src == e.fSrc) count++;
		}
		return count;
	}
private:
	int size;
	unordered_set<int> fNodes;
	vector<Edge> fEdges;
};

void readData(Graph& graph, const char* textfile) {
	ifstream data(textfile);
	if (data.is_open()) {
		string line;
		while (getline(data, line)) {
			stringstream key;
			int i = 0;
			while (line[i] != '\t') {
				key << line[i++];
			}
			// create node
			int src = stoi(key.str());
			// continue reading numbers
			for (auto it = line.begin() + i; it != line.end();) {
				stringstream other;
				// skip space
				while (it != line.end() && *it == '\t') it++;
				while (it != line.end() && *it != '\t') {
					other << *it;
					it++;
				}
				if (!other.str().empty()) {
					graph.addEdge(src, stoi(other.str()));
				}
			}
		}
	}
}

void findMinimumCut() {
	srand(static_cast<uint>(time(NULL)));
	const int num = 200;
	Graph originalGraph(num);
	readData(originalGraph, "kargerMinCut.txt");

	int minCuts = INT_MAX;
	for (int i = 0; i < num; i++) {
		Graph graph(originalGraph);
		int count = graph.findMinimumCut();
		if (count < minCuts)
		{
			minCuts = count;
			cout << i << "th run: " << count << endl;
		}
		
	}
	//readData(graph, "testCut.txt");
	cout << "find min cuts: " << minCuts << endl;;




}