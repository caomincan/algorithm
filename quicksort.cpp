#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "utils.h"

using namespace std;

int pivotLastElement(int st, int ed) {
	return ed;
}

void swap(vector<int>& nums, int i, int j) {
	if (i == j) return;
	int tmp = nums[i];
	nums[i] = nums[j];
	nums[j] = tmp;
}

int find_pivot_position(vector<int>& nums, int left, int right, int k) {
	int i = left;
	for (int j = i; j <= right; j++) {
		if (nums[j] >= k) continue;
		else swap(nums, i++, j);
	}
	return i - 1;
}

void quick_sort_first(vector<int>& nums, int left, int right, int& count) {
	if (left >= right) return;
	count += right - left;
	int pivot = nums[left];
	int k = find_pivot_position(nums, left+1, right, pivot);
	swap(nums, k, left);
	// continue partition
	quick_sort_first(nums, left, k - 1, count);
	quick_sort_first(nums, k + 1, right, count);
}

void quick_sort_last(vector<int>& nums, int left, int right, int& count) {
	if (left >= right) return;
	count += right - left;
	int pivot = nums[right];
	swap(nums, left, right);
	int k = find_pivot_position(nums, left+1, right, pivot);
	swap(nums, k, left);
	// continue partition
	quick_sort_last(nums, left, k - 1, count);
	quick_sort_last(nums, k + 1, right, count);
}

void quick_sort_median(vector<int>& nums, int left, int right, int& count) {
	if (left >= right) return;
	count += right - left;

	int mid = left + (right - left) / 2;	
	vector<int> candidates{ nums[left], nums[right], nums[mid] };
	sort(candidates.begin(), candidates.end());
	int pivot = candidates[1];
	int pIdx;
	if (pivot == nums[left]) pIdx = left;
	else if (pivot == nums[right]) pIdx = right;
	else pIdx = mid;
	swap(nums, left, pIdx);
	int k = find_pivot_position(nums, left + 1, right, pivot);
	swap(nums, k, left);
	// continue partition
	quick_sort_median(nums, left, k - 1, count);
	quick_sort_median(nums, k + 1, right, count);

}

bool isSorted(vector<int>& nums) {
	for (int i = 0; i < nums.size() - 1; i++) {
		if (nums[i] > nums[i + 1]) return false;
	}
	return true;
}

void quick_sort_problems() {
	ifstream data("QuickSort.txt");
	vector<int> nums1;
	if (data.is_open()) {
		string line;
		while (getline(data, line)) {
			nums1.push_back(stoi(line));
		}
	}

	cout << " count : " << nums1.size() << endl;
	vector<int> nums2(nums1); vector<int> nums3(nums1);
	int count = 0;
	// first
	quick_sort_first(nums1, 0, nums1.size() - 1, count);
	cout << " pivot is 1st comparision: " << count << endl;
	cout << " is sorted? " << (isSorted(nums1) ? "YES" : "NO") << endl;

	// second
	int count2 = 0;
	quick_sort_last(nums2, 0, nums2.size() - 1, count2);
	cout << " pivot is last comparision: " << count2 << endl;
	cout << " is sorted? " << (isSorted(nums2) ? "YES" : "NO") << endl;

	// median
	int count3 = 0;
	quick_sort_median(nums3, 0, nums3.size() - 1, count3);
	cout << " pivot is median comparision: " << count3 << endl;
	cout << " is sorted? " << (isSorted(nums3) ? "YES" : "NO") << endl;

}