#pragma once
#ifndef _UTIL_HPP
#define _UTIL_HPP
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;
void swap(vector<int>& nums, int i, int j);

void count_inversion();
void quick_sort_problems();
void solve_scc_problem();
void findMinimumCut();
void CalculateShortestPath();
void CalculateMedianFromStream();
int findNumberOfTwoSum();
void test_heap();
void CalculateCompletionTimes();
void findMST();
void findMaxSpacingCluster4();
void processClustering(const char* file);
#endif