#include "utils.h"
#include <assert.h>

struct Edge2 {
    Edge2(int src, int dst, int w) :
        fSrc(src),
        fDst(dst),
        fWeight(w)
    {}

    Edge2() :
        fSrc(-1),
        fDst(-1),
        fWeight(INT_MAX)
    {
    }
    int fSrc;
    int fDst;
    int fWeight;
};


class Node2 {
public:
    Node2(const int id) :
        fId(id),
        fIsFinished(false),
        fKey(nullptr),
        fDistance(nullptr)
    {
    }

    ~Node2() = default;

    int id() const { return fId; }

    vector<Edge2*>& getEdges() { return fEdges; }
    vector<Edge2*>& getIncomings() { return fIncomings; }
    //vector<Node2*>& getPath() { return fPath; }

    int getKeyValue() const {
        if (!fKey || fKey->fSrc == -1) return INT_MAX;
        int src = fKey->fSrc;
        return (*fDistance)[src] + fKey->fWeight;
    }
    Edge2* getKey() { return fKey; }
    void setKey(Edge2* edge) { fKey = edge; }

    bool isFinished() const { return fIsFinished; }
    void markFinished() { fIsFinished = true; }

    vector<int>* getDistanceTable() { return fDistance; }
    int getDistance() const {
        return (*fDistance)[fId];
    }
    void setDistance(int d) {
        (*fDistance)[fId] = d;
    }

    void reset(vector<int>* dist) {
        fIsFinished = false;
        fKey = nullptr;
        fDistance = dist;
        //fPath.clear();
    }

private:
    int fId;
    bool fIsFinished;
    Edge2* fKey;
    vector<Edge2*> fEdges;
    vector<Edge2*> fIncomings;
    //vector<Node2*> fPath;
    vector<int>* fDistance;
};

bool compareNode(Node2* p1, Node2* p2) {
    return p1->getKeyValue() > p2->getKeyValue();
}

bool CreateGraph(vector<Node2*>& graph, vector<Edge2*>& edgePool, const int numNdoes, const char* datafile)
{
    assert(graph.size() == numNdoes + 1);
    ifstream data(datafile);
    if (data.is_open())
    {
        string line;
        getline(data, line);
        int idx = findSpace(line, 0);
        const int n = stoi(line.substr(0, idx));
        const int m = stoi(line.substr(idx + 1));
        while (getline(data, line)) {
            idx = findSpace(line, 0);
            int dst = stoi(line.substr(0, idx));
            line = line.substr(idx + 1);
            idx = findSpace(line, 0);
            int src = stoi(line.substr(0, idx));
            int w = stoi(line.substr(idx + 1));
            Edge2* edge = new Edge2(src, dst, w);
            edgePool.push_back(edge);
            graph[src]->getEdges().push_back(edge);
            graph[dst]->getIncomings().push_back(edge);
        }
        return true;
    }
    return false;
}

void CalculateDijkstraShortestPath(vector<Node2*>& graph, vector<vector<int>*>& dists, const int srcIdx= 1)
{
    for (auto n : graph) {
        if (n) { n->reset(dists[srcIdx]);}
    }
    // initlized first node and rest node's key
    graph[srcIdx]->markFinished();
    graph[srcIdx]->setDistance(0);
    auto dsts = graph[srcIdx]->getEdges();
    for (auto& e : dsts) {
        graph[e->fDst]->setKey(e);
    }
    // initlize heap 
    vector<Node2*> heap;
    for (int i = 1; i < graph.size(); i++) {
        if (i == srcIdx) continue;
        heap.push_back(graph[i]);
    }
    make_heap(heap.begin(), heap.end(), compareNode);

    // start loop util heap is empty
    while (!heap.empty()) {
        // select minimum edge
        auto w = heap.front();
        // pop out selected node
        pop_heap(heap.begin(), heap.end()); heap.pop_back();
        auto lvw = w->getKey();
        // update node's shortest path value and path
        auto v = graph[lvw->fSrc];
        assert(lvw->fDst == w->id());
        // update result of w
       // auto wPath(v->getPath()); wPath.push_back(w);
        w->setDistance(v->getDistance() + lvw->fWeight);
       // w->getPath() = wPath;
        w->markFinished();
        // RELAX
        for (auto edge : w->getEdges()) {
            assert(edge->fSrc == w->id());
            auto x = graph[edge->fDst];
            // x still inside unfinished set
            if (!x->isFinished() && (w->getDistance() + edge->fWeight) < x->getKeyValue()) {
                // means not finish calculation and new weight is smaller
                x->setKey(edge);
            }
        }
        make_heap(heap.begin(), heap.end(), compareNode);
    }
}

void printResult(vector<vector<int>*>& dists, const int src = 1)
{
    cout << "result: " << endl;
    vector<int>* dist = dists[src];
    vector<int> idx{ 7, 37, 59, 82, 99, 115, 133, 165, 188, 197 };
    for (auto id : idx) {
        cout << (*dist)[id] << ",";
    }
    cout << endl;
}

void resetDist(vector<vector<int>*>& dists, const int src = 1)
{
    vector<int>* dist = dists[src];
    for (int i = 0; i < dist->size(); i++) {
        (*dist)[i] = INT_MAX;
    }
}

bool BellManFordAlgorithm(vector<Node2*>& graph,  vector<vector<int>*>& dists, const int srcIdx)
{
    const int  num = static_cast<int>(graph.size()) - 1;
    vector<long long> dp(num+1, INT_MAX);
    // init A[0,s] = 0;
    dp[srcIdx] = 0;
    vector<int>* dist = dists[srcIdx]; // result
    for (int i = 1; i <=num+1; i++) {
        auto pre = dp;
        for (int v = 1; v <= num; v++) {
            Node2* node = graph[v];
            auto incoming = node->getIncomings();
            long long new_res = pre[v];
            for (auto edge : incoming) {
                int w = edge->fSrc;
                long long viaW = pre[w] + edge->fWeight;
                if (viaW < new_res) {
                    new_res = viaW;
                }
            }
            dp[v] = new_res;
        }
        if (i == num + 1) {
            for (int j = 1; j <= num; j++) {
                if (dp[j] != pre[j]) return false;
            }
        }
    }

    for (int i = 1; i <= num; i++) {
        (*dist)[i] = static_cast<int>(dp[i]);
    }
    return true;
}

void JohnsonAlogrithm(vector<Node2*>& graph, vector<Edge2*>& edges, vector<vector<int>*>& dists)
{
    const int  num = static_cast<int>(graph.size()) - 1;
    //init s
    for (int i = 1; i <= num; i++) {
        Edge2* e = new Edge2(0, i, 0);
        edges.push_back(e);
        graph[0]->getEdges().push_back(e);
        graph[i]->getIncomings().push_back(e);
    }
    // Invoke BellMan-Ford first
    if (!BellManFordAlgorithm(graph, dists, 0)) {
        cout << "Current graph contains negative cycle!" << endl;
        return;
    }
    // Reweight
    cout << "Start re-wieghting edges..." << endl;
    auto& weights = (*dists[0]);
    for (auto e : edges) {
        if (e->fSrc == 0) continue;
        int pv = weights[e->fSrc];
        int pu = weights[e->fDst];
        e->fWeight = e->fWeight + pv - pu;
    }
    cout << "Run Dijkstra on each node as source.. " << endl;
    for (int i = 1; i <= num; i++) {
        CalculateDijkstraShortestPath(graph, dists, i);
    }
    // re-apply weights;
    int minPath = INT_MAX;
    for (int i = 1; i <= num; i++) {
        auto& path = *(dists[i]);
        for (int j = 1; j <= num; j++) {
            if (i == j) {
                assert(path[j] == 0);
                continue;
            }
            path[j] = path[j] - weights[i] + weights[j];
            if (path[j] < minPath) {
                minPath = path[j];
            }
        }
    }
    cout<< "Finally mimimum path is: "<< minPath << endl;
}
void CalculateAllPairShortestPath(const int n, const char* file)
{
    vector<Node2*> graph(n + 1, nullptr);
    vector<Edge2*> pool;
    vector<vector<int>*> dists(n + 1, nullptr);
    for (int i = 0; i < n + 1; i++) {
        dists[i] = new vector<int>(n + 1, INT_MAX);
        graph[i] = new Node2(i);
    }
    if (CreateGraph(graph, pool, n, file)) {
        cout << "Working on graph: " << file << endl;
        JohnsonAlogrithm(graph, pool, dists);
    }
    else {
        cout << "Fail to read graph" << endl;
        return;
    }


    for (auto n : graph) { if (n) delete n; }
    for (auto e : pool) { if (e) delete e; }
    for (auto dist : dists) { if (dist) delete dist; }
}