#include "utils.h"
#include <assert.h>
#include <queue>

struct TreeNode {
	TreeNode(long w) : 
		weight(w), left(nullptr), right(nullptr), encode("")
	{}

	TreeNode(long w, TreeNode* l, TreeNode* r) :
		weight(w), left(l), right(r), encode("")
	{}
	long weight;
	TreeNode* left;
	TreeNode* right;
	string encode;

	bool isLeaf() const { return !left && !right; }
};

bool compare(const TreeNode* n1, const TreeNode* n2)
{
	return n1->weight > n2->weight;
}

template<class Comp>
void push(vector<TreeNode*>& heap, TreeNode* val, Comp comp)
{
	heap.push_back(val);
	push_heap(heap.begin(), heap.end(), comp);
}

template<class Comp>
void pop(vector<TreeNode*>& heap, Comp comp)
{
	pop_heap(heap.begin(), heap.end(), comp);
	heap.pop_back();
}

void release(vector<TreeNode*>& pool)
{
	for (auto n: pool)
	{
		delete n;
	}	
}

vector<TreeNode*> GenerateEncode(TreeNode* root)
{
	vector<TreeNode*> res;
	if (!root) return res;
	// BFS
	queue<TreeNode*> queue;
	queue.push(root);
	while (!queue.empty())
	{
		int count = static_cast<int>(queue.size());
		while (count > 0)
		{
			TreeNode* n = queue.front();
			queue.pop();
			if (n->left) {
				n->left->encode = (n->encode + "0");
				queue.push(n->left);
			}
			if (n->right) {
				n->right->encode = (n->encode + "1");
				queue.push(n->right);
			}
			if (n->isLeaf()) res.push_back(n);
			count--;
		}
	}

	return res;
}

void InitProblem(vector<TreeNode*>& pool, vector<TreeNode*>& heap)
{
	ifstream data("huffman.txt");
	if (data.is_open()) {
		string line;
		getline(data, line);
		const int n = stoi(line);
		pool.reserve(n);
		heap.reserve(n);
		while (getline(data, line)) {
			long w = stol(line);
			TreeNode* n = new TreeNode(w);
			pool.push_back(n);
			push(heap, n, compare);
		}
	}
	else {
		cout << "Fail to open file: huffman.txt!" << endl;
	}
}

TreeNode* process(vector<TreeNode*>& pool, vector<TreeNode*>& heap)
{
	TreeNode* root = nullptr;
	while (heap.size() > 1)
	{
		TreeNode* left = heap.front(); pop(heap, compare);
		TreeNode* right = heap.front(); pop(heap, compare);
		TreeNode* internal = new TreeNode((left->weight + right->weight), left, right);
		pool.push_back(internal);
		push(heap, internal, compare);
	}
	root = heap.front(); pop(heap, compare);
	assert(heap.empty());
	return root;
}

void runHuffmanCompression()
{
	vector<TreeNode*> pool;
	vector<TreeNode*> heap;

	InitProblem(pool, heap);

	TreeNode* root = process(pool, heap);

	auto res = GenerateEncode(root);

	int max_length = INT_MIN;
	int min_length = INT_MAX;
	for (auto n : res) {
		int length = static_cast<int>(n->encode.size());
		if ( length < min_length) {
			min_length = length;
		}
		if ( length > max_length) { 
			max_length = length;
		}
	}

	cout << "Maximum length of a codeword: " << max_length << endl;
	cout << "Minimum length of a codeword: " << min_length << endl;
	release(pool);
}