#include "utils.h"
#include "Heap.h"
enum VISIT{
    YES,NO
};
class Edge {
public:
    Edge(int s,int d,int w):
        src(s), dst(d), weight(w)
    {}
    Edge(): src(-1), dst(-1), weight(INT_MAX) {}
    ~Edge() = default;
    int src;
    int dst;
    int weight;
    friend bool operator<(const Edge& e1, const Edge& e2) { return e1.weight < e2.weight; }
    friend bool operator>(const Edge& e1, const Edge& e2) { return e1.weight > e2.weight; }
    friend bool operator==(const Edge& e1, const Edge& e2) 
    { 
        return e1.src == e2.src &&
            e1.dst == e2.dst &&
            e1.weight == e2.weight; 
    }
};

bool compareEdge(const Edge* e1, const Edge* e2) { return e1->weight < e2->weight; }


bool ReadGraph(unordered_map<int, vector<Edge>>& graph, const char* datafile)
{
    ifstream data(datafile);
    if (data.is_open())
    {
        string line;
        getline(data, line);
        int idx = 0;
        while (idx < line.size() && line[idx] != ' ') idx++;
        const int n = stoi(line.substr(0, idx));
        const int m = stoi(line.substr(idx + 1));
        // reading edges
        while (getline(data, line)) {
            idx = 0;
            while (line[idx] != ' ') { idx++; }
            int src = stoi(line.substr(0, idx));
            int st = idx + 1;
            idx++;
            while (line[idx] != ' ') { idx++;}
            int dst = stoi(line.substr(st, idx));
            int w = stoi(line.substr(idx+1));
            auto n = graph.find(src);
            Edge e(src, dst, w);
            if (n == graph.end()) {
                vector<Edge> tmp(1,e);
                graph.insert(make_pair(src, tmp));
            } else {
                n->second.push_back(e);
            }
            n = graph.find(dst);
            Edge d(dst, src, w);
            if (n == graph.end()) {
                vector<Edge> tmp(1, d);
                graph.insert(make_pair(dst, tmp));
            }
            else {
                n->second.push_back(d);
            }
        }
        return true;
    }
    return false;
}

long long findCost(unordered_map<int, vector<Edge>>& g)
{
    long long cost = 0;
    unordered_set<int> memo;
    for (auto p : g)
    {
        if (memo.find(p.first) != memo.end()) continue;
        memo.insert(p.first);
        for (auto& e : p.second) {
            if (memo.find(e.dst) != memo.end()) {
                cost += e.weight;
                memo.insert(e.dst);
            }
        }
    }

    return cost;
}

void insertEdgeToTree(unordered_map<int, vector<Edge>>& tree, const Edge& e)
{
    auto src = tree.find(e.src);
    auto dst = tree.find(e.dst);
    if (src == tree.end()) {
        vector<Edge> tmp(1, e);
        tree.insert(make_pair(e.src, tmp));
    }
    else {
        src->second.push_back(e);
    }
    Edge rev(e.dst, e.src, e.weight);
    if (dst == tree.end()) {
        vector<Edge> tmp(1, rev);
        tree.insert(make_pair(rev.src, tmp));
    }
    else {
        dst->second.push_back(rev);
    }
}
void findMST()
{
    unordered_map<int, vector<Edge>> graph;
    unordered_map<int, vector<Edge>> tree;
    long long cost = 0;
    //vector<shared_ptr<Node>> tree;
    if (ReadGraph(graph, "edges.txt")) {
        // init heap
        unordered_set<int> v; v.insert(1);
        Test::Heap<Edge> heap(false);
        for (auto e : graph[1]) { heap.push(e);}
        // start working
        while (!heap.isEmpty())
        {
            Edge e = heap.min(); heap.popMin();
            v.insert(e.src);
            v.insert(e.dst);
            insertEdgeToTree(tree, e);
            heap.clear();
            for (auto p : graph)
            {
                if (v.find(p.first) != v.end()) continue;
                for (auto& e : p.second) {
                    if (v.find(e.dst) != v.end()) {
                        heap.push(e);
                    }
                }
            }
        }
    }
    cout << "The total cost of MST: " << findCost(tree) << endl;
}