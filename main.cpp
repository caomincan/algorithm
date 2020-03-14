#include "utils.h"

using namespace std;
int main(int argn, char* argv[])
{
	cout << "Start problems: " << endl;
	cout << "Count Inversion from Array: " << endl;
	count_inversion();
	cout << "Count number of comparision from quick sort: " << endl;
	quick_sort_problems();
	cout << "5 largest SCC: 434821 968 459 313 211" << endl;
	//solve_scc_problem();
	// 434821 968 459 313 211
	// correct answer 17
	cout << "Find minimum cuts : 17" << endl;
	// findMinimumCut(); // supper slow
	// correct answer: 2599,2610,2947,2052,2367,2399,2029,2442,2505,3068
	cout << "Dijkstra's shorted path: " << endl;
	CalculateShortestPath();
    // Find stream number of median by heap  1213
	cout << "Find stream number of median by heap: " << endl;
	CalculateMedianFromStream();
	//test_heap();
	// Find variant number of sum : 427
	cout << "The number of target values tt: " << endl;
	//cout << " The number of variant t is: " << findNumberOfTwoSum() << endl;
	// correct answer: w-l 69119377652  w/l 67311454237
    cout << "Find the weighted compilation time: " << endl;
    CalculateCompletionTimes();
    // Find Minimum Spanning Tree: -3612829
    cout << "Find minimum spanning tree from graph: -3612829" << endl;
    //findMST();
	// Find Maximum spacing of k-cluster: 106 
	cout << "Find maximum k-cluster spacing: 106" << endl;
	findMaxSpacingCluster4();
	// Find largest value of k so that Hamming Distance is at least 3
	// correct answer 6118
	cout << "Find largest value of k : 6118" << endl;	
	//processClustering("clustering_big.txt");
	// Find maximum and minimum length of encode by Huffman alogrithm
	// max 19 min 9
	runHuffmanCompression();
	// of the vertices 1, 2, 3, 4, 17, 117, 517, and 997,
	// which ones belong to the maximum-weight independent set?
	// correct answer 10100110
	cout << "Max-weight Independent Set: " << endl;
	findMaximumIndependentSet();
    getchar();
	return 1;
}