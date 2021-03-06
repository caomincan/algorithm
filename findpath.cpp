#include "utils.h"
#include <assert.h>

struct Edge;
using GRAPH = vector<Edge>;

struct Edge {
	Edge(int src, int dst, int w) :
		fSrc(src),
		fDst(dst),
		fWeight(w)
	{}

    Edge():
        fSrc(-1),
        fDst(-1),
        fWeight(INT_MAX)
    {
    }
	int fSrc;
	int fDst;
	int fWeight;
};




bool compareEdge(const Edge& e1, const Edge& e2)
{
    return e1.fWeight > e2.fWeight;
}

class Node {
public:
    Node(const int id, vector<int>& dist) :
        fId(id),
        fIsFinished(false),
        fDistance(dist)
    {
    }

    ~Node() = default;

    int id() const { return fId; }

    vector<Edge*>& getEdges() { return fEdges; }
    vector<Node*>& getPath() { return fPath; }

    int getKeyValue() const { 
        if (!fKey || fKey->fSrc == -1) return INT_MAX;
        int src = fKey->fSrc;
        return fDistance[src] + fKey->fWeight;
    }
    Edge* getKey() { return fKey; }
    void setKey(Edge* edge) { fKey = edge;}

    bool isFinished() const { return fIsFinished; }
    void markFinished() { fIsFinished = true; }

    vector<int>& getDistanceTable() { return fDistance; }
    int getDistance() const {
        return fDistance[fId];
    }
    void setDistance(int d) {
        fDistance[fId] = d;
    }

private:
    int fId;
    bool fIsFinished;
    Edge* fKey;
    vector<Edge*> fEdges;
    vector<Node*> fPath;
    vector<int>& fDistance;
};

bool compareNodeInHeap(Node* p1, Node* p2) {
    return p1->getKeyValue() > p2->getKeyValue();
}

bool CreateGraph(vector<Node*>& graph, vector<Edge*>& edgePool, vector<int>& dist, const int numNdoes, const char* datafile)
{
    assert(graph.size() == numNdoes + 1);
    ifstream data(datafile);
    if (data.is_open())
    {
        string line;
        while (getline(data, line)) {
            int idx = 0;
            while (line[idx] != '\t') { idx++; }
            int label = stoi(line.substr(0, idx));
            Node* node = new Node(label, dist);
            graph[label] = node;
            //cout << "Node: " << label << " ";
            // reading edges and weights
            while (idx < line.size()) {
                int st = ++idx;
                while (idx < line.size() && line[idx] != '\t') { idx++; }
                int ed = idx;
                string edge = line.substr(st, ed - st);
                //cout << edge << " ";
                for (int i = 0; i < edge.size(); i++) {
                    if (edge[i] == ',') {
                        int dst = stoi(edge.substr(0, i));
                        int w = stoi(edge.substr(i + 1));
                        Edge* edge = new Edge(label, dst, w);
                        edgePool.push_back(edge);
                        node->getEdges().push_back(edge);
                        break;
                    }
                }
            }
            // cout << endl;
        }
        return true;
    }
    return false;
}

void CalculateDijkstraShortestPath(vector<Node*>& graph)
{
    const int n = static_cast<int>(graph.size());
    // initlized first node and rest node's key
    graph[1]->markFinished();
    graph[1]->setDistance(0);
    auto dsts = graph[1]->getEdges();
    for (auto& e : dsts) {
        graph[e->fDst]->setKey(e);
    }
    // initlize heap 
    vector<Node*> heap(graph.begin() + 2, graph.end());
    make_heap(heap.begin(), heap.end(), compareNodeInHeap);

    // start loop util heap is empty
    while (!heap.empty()) {
        // select minimum edge
        auto w = heap.front();
        // pop out selected node
        pop_heap(heap.begin(), heap.end()); heap.pop_back();
        auto lvw= w->getKey();
        // update node's shortest path value and path
        auto v = graph[lvw->fSrc];
        assert(lvw->fDst == w->id());
        // update result of w
        auto wPath(v->getPath()); wPath.push_back(w);
        w->setDistance(v->getDistance() + lvw->fWeight);
        w->getPath() = wPath;
        w->markFinished();
        // RELAX
        for (auto edge : w->getEdges()) {
            assert(edge->fSrc == w->id());
            auto x = graph[edge->fDst];
            // x still inside unfinished set
            if (!x->isFinished() &&  (w->getDistance() + edge->fWeight) < x->getKeyValue()) {
                // means not finish calculation and new weight is smaller
                x->setKey(edge);
            }
        }
        make_heap(heap.begin(), heap.end(), compareNodeInHeap);
    }
}

void CalculateShortestPath()
{
    const int n = 200;
    vector<Node*> graph(n + 1, nullptr);
    vector<Edge*> pool;
    vector<int> dist(n + 1, INT_MAX);
    if (CreateGraph(graph, pool, dist, n, "dijkstraData.txt")) {
        CalculateDijkstraShortestPath(graph);
    }
    else {
        cout << "Fail to read graph" << endl;
        return;
    }

    cout << "result: " << endl;
    vector<int> idx{ 7, 37, 59, 82, 99, 115, 133, 165, 188, 197 };
    for (auto id : idx) {
        cout << dist[id] << ",";
    }
    cout << endl;
    for (auto n : graph) { if (n) delete n; }
    for (auto e : pool) { if (e) delete e; }
}

#if 0
bool ReadWeightedGraph_v1(GRAPH& graph, const char* filename)
{
    ifstream data(filename);
    if (data.is_open())
    {
        string line;
        while (getline(data, line)) {
            int idx = 0;
            while (line[idx] != '\t') { idx++; }
            int src = stoi(line.substr(0, idx));
            //cout << "Node: " << label << " ";
            // reading edges and weights
            while (idx < line.size()) {
                int st = ++idx;
                while (idx < line.size() && line[idx] != '\t') { idx++; }
                int ed = idx;
                string edge = line.substr(st, ed - st);
                //cout << edge << " ";
                for (int i = 0; i < edge.size(); i++) {
                    if (edge[i] == ',') {
                        int dst = stoi(edge.substr(0, i));
                        int w = stoi(edge.substr(i + 1));
                        graph.push_back(Edge(src, dst, w));
                        break;
                    }
                }
            }
            // cout << endl;
        }
        return true;
    }
    return false;
}

void CalculateShortestPath_v1()
{
    const int n = 200;
    GRAPH graph(n + 1);
    vector<int> dist(n + 1, INT_MAX);
    // "dijkstraData.txt"
    if (ReadWeightedGraph_v1(graph, "dijkstraData.txt")) {
        // initilization
        dist[1] = 0;
        unordered_set<int> v;
        unordered_set<int> v_x;
        v.insert(1);
        for (int i = 2; i <= n; i++) { v_x.insert(i); }
        //  start algorithm
        while (!v_x.empty()) {
            Edge choice;
            // loop over all unfinished nodes
            for (auto& e : graph) {
                int this_src = e.fSrc;
                int this_dst = e.fDst;
               
                if (v.find(e.fSrc) != v.end() && v_x.find(e.fDst) != v_x.end())
                {
                    if (choice.fSrc == -1) choice = e;
                    else if ((dist[this_src] + e.fWeight) < (dist[choice.fSrc]+ choice.fWeight)) {
                        choice = e;
                    }
                }
            }
            // find the edge
            int src = choice.fSrc;
            int dst = choice.fDst;
            dist[dst] = dist[src] + choice.fWeight;
            v.insert(dst);
            v_x.erase(dst);
        }


        cout << "result: " << endl;
        vector<int> idx{ 7, 37, 59, 82, 99, 115, 133, 165, 188, 197 };
        for (auto id : idx) {
            cout << dist[id] << ",";
        }
        cout << endl;
        
    }
    else {
        cout << "Fail to read graph!" << endl;
    }
}
#endif