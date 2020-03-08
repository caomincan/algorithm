#pragma once
#ifndef _HEAP_HPP
#define _HEAP_HPP
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
namespace Test
{
	class Heap {
	public:
		Heap() : fSize(0), fIsMaxHeap(true)
		{}

		Heap(const bool isMaxHeap) : fIsMaxHeap(isMaxHeap)
		{}

		Heap(vector<int>& data, const bool isMaxHeap) : 
			fData(data.begin(), data.end()), fSize(data.size()), fIsMaxHeap(isMaxHeap)
		{
			if(fIsMaxHeap) make_heap(fData.begin(), fData.end(),  less<int>());
			else make_heap(fData.begin(), fData.end(), greater<int>());
		}

		bool isEmpty() const { return fData.empty(); }
		int size() const { return static_cast<int>(fSize); }

		int max() 
		{ 
			if (isEmpty()) return -1;
			if (fIsMaxHeap) return fData.front();
			else {
				int maxV = fData[0];
				for (auto x : fData) {
					if (x > maxV) maxV = x;
				}
				return maxV;
			}
		}

		int min() 
		{ 
			if (isEmpty()) return -1;
			if (!fIsMaxHeap) return fData.front();
			else {
				int minV = fData[0];
				for (auto x : fData) {
					if (x < minV) minV = x;
				}
				return minV;
			}
		}

		void popMax()
		{
			if (isEmpty()) return;
			if (fIsMaxHeap){
				pop_heap(fData.begin(), fData.end(), less<int>());
				fData.pop_back();
				fSize = fData.size();
			} else {
				int maxV = max();
				deleteVal(maxV);
			}
			
		}

		void popMin()
		{
			if (isEmpty()) return;
			if (!fIsMaxHeap) {
				pop_heap(fData.begin(), fData.end(), greater<int>());
				fData.pop_back();
				fSize = fData.size();
			} else {
				int minV = min();
				deleteVal(minV);
			}
		}

		void push(int val)
		{
			fData.push_back(val);
			if(fIsMaxHeap) push_heap(fData.begin(), fData.end(), less<int>());
			else push_heap(fData.begin(), fData.end(), greater<int>());
			fSize = fData.size();
		}

		int find(const int val)
		{
			auto it = std::find(fData.begin(), fData.end(), val);
			return it == fData.end() ? -1 : static_cast<int>(it - fData.begin());
		}

		void deleteVal(const int val)
		{
			int idx = find(val);
			if (idx == -1) return;
			if(fIsMaxHeap) pop_heap(fData.begin() + idx, fData.end(), less<int>());
			else pop_heap(fData.begin() + idx, fData.end(), greater<int>());
			fData.pop_back();
			fSize = fData.size();
		}

		// return index
		int parentIdx(int idx) {
			if (idx < 0 || idx >= fSize) return -1;
			return idx %2 == 1 ? idx/ 2 : idx/2-1;
		}

		int leftChildIdx(int idx) {
			int tmp = 2*idx + 1;
			return tmp >= fSize ? -1 : tmp;
		}

		int rightChildIdx(int idx) {
			int tmp = idx * 2 + 2;
			return tmp >= fSize ? -1 : tmp;
		}

		int operator[](int idx)
		{
			if (idx < 0 || idx >= size()) return -1;
			return fData[idx];
		}

		void print()
		{
			int level = 1;
			int base = 2;
			for (int i = 0; i < size(); i++)
			{
				cout << fData[i];
				if (i+1 >= level) {
					cout << endl;
					level += base;
					base *= 2;
				}
				else cout << " ";
			}
			cout << endl;
		}
	private:
		vector<int> fData;
		size_t fSize;
		bool fIsMaxHeap;

		void swap(vector<int>& data, int i, int j)
		{
			int tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
		}
	};
}

#endif
