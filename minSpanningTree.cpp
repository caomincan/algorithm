#include "utils.h"
#include <assert.h>

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
};

bool compareEdge(const Edge* e1, const Edge* e2) { return e1->weight < e2->weight; }

class Node {
public:
    Node(int label) :
        fLabel(label),
        fKey(-1,-1, INT_MAX)
    {}

    Node() : fLabel(-1), fKey(-1,-1, INT_MAX) {}
    ~Node()
    {}

    Node(const Node&) = default;
    Node& operator=(const Node&) = default;

    Edge& getKey() { return fKey; }
    void  setKey(const Edge& e) { fKey = e; }
    int getLabel() const { return fLabel; }
    void setLabel(int label) { fLabel = label; }
    void insertEdge(Edge e) { fEdges.push_back(e); }
    vector<Edge>& getEdges() { return fEdges; }

    friend bool operator<(const Node& n1, const Node& n2) { return n1.getKeyValue() < n1.getKeyValue(); }
    friend bool operator>(const Node& n1, const Node& n2) { return n2.getKeyValue() > n2.getKeyValue(); }
    bool operator==(const Node& other) const { return getLabel() == other.getLabel(); }

    void initKey() {
        sort(fEdges.begin(), fEdges.end(), less<Edge>());
        assert(fEdges.size() >= 1);
        setKey(fEdges[0]);
    }

protected:
    int getKeyValue() const
    {
        return fKey.src!=-1 ? fKey.weight : INT_MAX;
    }

private:
    int fLabel;
    Edge fKey;
    vector<Edge> fEdges;
    
};


bool ReadGraph(vector<Node>& graph,  const char* datafile)
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
        graph.push_back(Node(0));
        for (int i = 1; i <= n; i++)
        {
            graph.push_back(Node(i));
        }
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
   
            graph[src].insertEdge(Edge(src,dst,w));
            graph[dst].insertEdge(Edge(dst,src,w));
        }
        for (auto it = graph.begin() + 1; it != graph.end(); ++it) it->initKey();
        return true;
    }
    return false;
}

void findMST()
{
    vector<Node> graph;
    //vector<shared_ptr<Node>> tree;
    if (ReadGraph(graph,  "edges.txt")) {
        // init heap
        Test::Heap<Node> selection(false);
        for (int i = 1; i < graph.size(); i++) {
            selection.push(Node(graph.at(i)));
        }
          
    }
    cout << "The total cost of MST: ";
}