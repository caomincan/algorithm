#include "utils.h"
#include <assert.h>
#include <sstream>

struct NodeB {
	NodeB() : key(""), leader(""), rank(0)
	{}

	NodeB(string& line) : key(line), rank(0)
	{
		key.erase(remove(key.begin(), key.end(), ' '), key.end());
		leader = key;
	}
	string key;
	string leader;
	int rank;

#if 0
	int getValue() const {
		int res = 0x00000000;
		int idx = 0;
		for (int i = 0; i < 24; i++)
		{
			res = res << 1;
			if (value[i] == '0') {
				continue;
			}
			else if (value[i] == '1') {
				res |= 0x00000001;
			}
		}
		return res;
	}
#endif
	friend bool operator==(const NodeB& n1, const NodeB& n2) 
	{ 
		return n1.key == n2.key; 
	}
};

#if 0
namespace std {
	template<>
	struct hash<NodeB> {
		size_t operator()(const NodeB& n) const {
			return hash<string>()(n.key);
		}
	};
}
#endif

using Nodes = unordered_map<string, NodeB>;
using Clusters = unordered_set<string>;

string findLeadingNode(Nodes& nodes, string& key)
{
	if (nodes[key].key == nodes[key].leader) return nodes[key].leader;
	nodes[key].leader = findLeadingNode(nodes, nodes[key].leader);
	return nodes[key].leader;
}

void unionNodes(Nodes& nodes, string& key1, string& key2, Clusters& cluster)
{
	string srcLead = findLeadingNode(nodes, key1);
	string dstLead = findLeadingNode(nodes, key2);
	if (srcLead != dstLead) {
		if (nodes[srcLead].rank == nodes[dstLead].rank) {
			nodes[dstLead].leader = srcLead;
			nodes[srcLead].rank++;
			cluster.erase(dstLead);
		}
		else if (nodes[srcLead].rank > nodes[dstLead].rank) {
			nodes[dstLead].leader = srcLead;
			cluster.erase(dstLead);
		}
		else {
			nodes[srcLead].leader = dstLead;
			cluster.erase(srcLead);
		}
	}
}

void UnionDiff1(Nodes& nodes, Clusters& cluster)
{
	for (auto n : nodes)
	{
		string key = n.first;		
		for (int i = 0; i < 24; i++) {
			// change each bit create string
			string to_search = key;
			to_search[i] = to_search[i] == '0' ? '1' : '0';
			auto found = nodes.find(to_search);
			if (found != nodes.end()) {
				unionNodes(nodes, key, to_search, cluster);
			}
		}
	}
}

void UnionDiff2(Nodes& nodes, Clusters& cluster)
{
	for (auto n : nodes)
	{
		string key = n.first;
		for (int i = 0; i < 24; i++) {
			for (int j = 0; j < 24 && j!=i; j++) {
				string to_search = key;
				to_search[i] = to_search[i] == '0' ? '1' : '0';
				to_search[j] = to_search[j] == '0' ? '1' : '0';
				auto found = nodes.find(to_search);
				if (found != nodes.end()) {
					unionNodes(nodes, key, to_search, cluster);
				}
			}
		}
	}
}


void processClustering(const char* file)
{
	ifstream data(file);
	if (data.is_open()) {
		string line;
		getline(data, line);
		int idx = 0;
		while (idx < line.size() && line[idx] != ' ') { idx++; }
		const int n = stoi(line.substr(0, idx)); // number nodes
		const int nb = stoi(line.substr(idx + 1)); // number bits

		Nodes nodes; nodes.reserve(n);
		Clusters cluster;
		while (getline(data, line))
		{			
			NodeB n(line);
			if (nodes.find(n.key) == nodes.end()) {
				nodes.insert(make_pair(n.key, n));
				cluster.insert(n.key);
			}
		}
		// processing different 1
		UnionDiff1(nodes, cluster);
		UnionDiff2(nodes, cluster);
		cout << "The largest number of k: " << cluster.size() << endl;
	}
	else {
		cout << "Fail to open file " << file << endl;
	}
}