#if 0

#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <assert.h>
#include <stdlib.h>
#include <cstdlib>
#include <limits>
#include <ctime>

/*****************  THIS NOT RIGHT  ***********************/
using namespace std;
enum VISIT {
	YES, NO
};

class Node {
public:
	Node(int value):
		fValue(value),
		fActiveValue(fValue)
	{}
	~Node() = default;

	void addEdge(shared_ptr<Node>& node) {
		assert(node);
		fEdges.push_back(node);
	}

	void setActiveValue(int value) { fActiveValue = value; }
	int getActiveValue() const { return fActiveValue; }

	void reset() { 
		fActiveValue = fValue;
	}

	bool isAlive() {
		return fActiveValue == fValue;
	}

	vector<shared_ptr<Node>>& getEdges() {
		return fEdges;
	}

	

	int countActiveEdges(int targetSrc, vector<VISIT>& memo) {	
		// must start from root
		if (memo[fValue] == YES || fActiveValue != targetSrc) return 0;
		memo[fValue] = YES;
		int count = 0;
		for (auto& n : fEdges) {
			if (fActiveValue != n->getActiveValue()) {
				count++;
			} else {
				count += n->countActiveEdges(targetSrc, memo);
			}
		}
		return count;
	}

	int countEdgesTo(int src, int dst, vector<VISIT>& memo) {
		// must start from root
		if (memo[fValue] == YES || fActiveValue != src) return 0;
		memo[fValue] = YES;
		int count = 0;
		for (auto& n : fEdges) {
			if (dst == n->getActiveValue()) {
				count++;
			}
			else if(src == n->getActiveValue()){
				count += n->countEdgesTo(src, dst, memo);
			}
		}
		return count;
	}

private:
	vector<shared_ptr<Node>> fEdges;
	int fValue;
	int fActiveValue;
};

class GRAPH {
public :
	GRAPH(int num) :
		fTotalNumNodes(num),
		fNodes(num+1, nullptr)
	{
	}

	~GRAPH() = default;

	void createNode(int value) {
		assert(fNodes.size() >= value);
		if (fNodes[value] == nullptr) {
			fNodes[value].reset(new Node(value));
		}
	}

	void addEdge(int src, int dst) {
		assert(fNodes.size() >= src && fNodes.size() >= dst);
		createNode(src);
		createNode(dst);
		fNodes[src]->addEdge(fNodes[dst]);
	}

	void reset() {
		for (auto n : fNodes) {
			if (n) n->reset();
		}
	}

	vector<shared_ptr<Node>>& getNodeList() {
		return fNodes;
	}

	void contractionOnce(vector<int>& activeNodes) {
		if (activeNodes.size() <= 2) return;
		int srcIdx= rand() % static_cast<int>(activeNodes.size());
		int dstIdx = srcIdx;
		while (srcIdx == dstIdx) {
			dstIdx = rand() % static_cast<int>(activeNodes.size());
		}
		int src = activeNodes[srcIdx];
		int dst = activeNodes[dstIdx];
		//cout << "Keep src: " << src << " Merge dst: " << dst << endl;
		auto it = activeNodes.begin() + dstIdx;
		activeNodes.erase(it);
		
		for (auto n : fNodes) {
			if (n && n->getActiveValue() == dst) {
				n->setActiveValue(src);
			}
		}
	}

	int findMinimumCut() {
		vector<int> remainNode;		
		int minCut = INT_MAX;
		for (int i = 0; i < fTotalNumNodes* 50; i++) {
			remainNode.resize(fTotalNumNodes);
			for (int j = 1; j <= fTotalNumNodes; j++) remainNode[j - 1] = j;
			reset();
			while (remainNode.size() > 2) {
				contractionOnce(remainNode);
			}
			vector<VISIT> memo(fTotalNumNodes + 1, NO);
			int currentCuts = fNodes[remainNode[0]]->countEdgesTo(remainNode[0], remainNode[1], memo);
			cout << i << "th round cuts: " << currentCuts << endl;
			if (currentCuts < minCut) {
				minCut = currentCuts;
			}
		}		
		return minCut;
	}

private:
	int fTotalNumNodes;
	vector<shared_ptr<Node>> fNodes;
};

void readData(GRAPH& graph, const char* textfile) {
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
	srand(time(NULL));
	const int num = 200;
	GRAPH graph(num);
	readData(graph, "kargerMinCut.txt");
	//readData(graph, "testCut.txt");
	cout << "find min cuts: " << graph.findMinimumCut() << endl;;

	


}

#endif