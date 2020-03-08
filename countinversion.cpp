#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "utils.h"

using namespace std;


long long countSplit(vector<int>& nums, int left, int right) {
    if (left >= right) return 0;
    if (right - left == 1) {
        if (nums[left] > nums[right]) {
            swap(nums, left, right);
            return 1;
        }
        else return 0;
    }
    //
    long long sum = 0;
    vector<int> tmp(nums.begin() + left, nums.begin() + right + 1);
    int mid = (static_cast<int>(tmp.size()) - 1) / 2;
    int p1 = 0; int p2 = mid + 1;
    while (left <= right) {
        if (p1 > mid) {
            // run out of p1
            nums[left++] = tmp[p2++];
            continue;
        }

        if (p2 >= tmp.size()) {
            // run out of p2
            nums[left++] = tmp[p1++];
            continue;
        }
        // case
        if (tmp[p1] <= tmp[p2]) nums[left++] = tmp[p1++];
        else {
            nums[left++] = tmp[p2++];
            sum += (mid - p1 + 1);
        }
    }
    return sum;
}

long long countInversion(vector<int>& nums, int left, int right) {
    // cout << "from " << left << " to " << right << endl;
    if (left >= right) return 0;
    int mid = left + (right - left) / 2;
    long long leftC = countInversion(nums, left, mid);
    long long rightC = countInversion(nums, mid + 1, right);
    long long split = countSplit(nums, left, right);
    // cout << "left: " << leftC << " right: " << rightC << " split: " << split << endl;
    return leftC + rightC + split;
}

void count_inversion()
{
    ifstream myfile("IntegerArray.txt");
    string line;
    if (myfile.is_open()) {
        vector<int> nums;
        while (getline(myfile, line)) {
            nums.push_back(stoi(line));
        }
        cout << "size of input : " << nums.size() << endl;
        long long res = countInversion(nums, 0, static_cast<int>(nums.size()) - 1);
        cout << " results : " << res << endl;
    }
}