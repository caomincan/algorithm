#include "utils.h"
#include <iostream>
#include "Heap.h"

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
    // Find stream number of median by heap
	cout << "Find stream number of median by heap: " << endl;
	CalculateMedianFromStream();
	//test_heap();


	return 1;
}