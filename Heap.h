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
		Heap() : fSize(0)
		{}

		Heap(vector<int>& data) : fData(data.begin(), data.end()), fSize(data.size())
		{
			make_heap(fData.begin(), fData.end());
		}

		int max() { return fData.front(); }
		void popMax()
		{
			pop_heap(fData.begin(), fData.end());
			fData.pop_back();
			fSize = fData.size();
		}

		void push(int val)
		{
			fData.push_back(val);
			push_heap(fData.begin(), fData.end());
			fSize = fData.size();
		}

		bool isEmpty() const { return fData.empty(); }
		int size() const { return fSize; }

		int find(const int val)
		{
			auto it = std::find(fData.begin(), fData.end(), val);
			return it == fData.end() ? -1 : it - fData.begin();
		}

		void deleteVal(const int val)
		{
			int idx = find(val);
			if (idx == -1) return;
			pop_heap(fData.begin() + idx, fData.end());
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
		int fSize;

		void swap(vector<int>& data, int i, int j)
		{
			int tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
		}
	};
}

#endif
