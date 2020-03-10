#pragma once
#ifndef _HEAP_HPP
#define _HEAP_HPP
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
namespace Test
{
    template<class T>
	class Heap {
	public:
		Heap() : fSize(0), fIsMaxHeap(true)
		{}

		Heap(const bool isMaxHeap) : fSize(0),fIsMaxHeap(isMaxHeap)
		{}

		Heap(vector<T>& data, const bool isMaxHeap) : 
			fData(data.begin(), data.end()), fSize(data.size()), fIsMaxHeap(isMaxHeap)
		{
			if(fIsMaxHeap) make_heap(fData.begin(), fData.end(),  less<T>());
			else make_heap(fData.begin(), fData.end(), greater<T>());
		}

		bool isEmpty() const { return fData.empty(); }
		int size() const { return static_cast<int>(fSize); }

		T max() 
		{ 
			if (isEmpty()) return T();
			if (fIsMaxHeap) return fData.front();
			else {
				T maxV = fData[0];
				for (auto x : fData) {
					if (x > maxV) maxV = x;
				}
				return maxV;
			}
		}

		T min() 
		{ 
			if (isEmpty()) return T();
			if (!fIsMaxHeap) return fData.front();
			else {
				T minV = fData[0];
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
				pop_heap(fData.begin(), fData.end(), less<T>());
				fData.pop_back();
				fSize = fData.size();
			} else {
				T maxV = max();
				deleteVal(maxV);
			}
			
		}

		void popMin()
		{
			if (isEmpty()) return;
			if (!fIsMaxHeap) {
				pop_heap(fData.begin(), fData.end(), greater<T>());
				fData.pop_back();
				fSize = fData.size();
			} else {
				T minV = min();
				deleteVal(minV);
			}
		}
        void push(T&& val)
		{
			fData.push_back(val);
			if(fIsMaxHeap) push_heap(fData.begin(), fData.end(), less<T>());
			else push_heap(fData.begin(), fData.end(), greater<T>());
			fSize = fData.size();
		}

		void push(const T& val)
		{
			fData.push_back(val);
			if(fIsMaxHeap) push_heap(fData.begin(), fData.end(), less<T>());
			else push_heap(fData.begin(), fData.end(), greater<T>());
			fSize = fData.size();
		}

		int find(const T& val)
		{
			auto it = std::find(fData.begin(), fData.end(), val);
			return it == fData.end() ? -1 : static_cast<int>(it - fData.begin());
		}

		void deleteVal(const T& val)
		{
			int idx = find(val);
			if (idx == -1) return;
			if(fIsMaxHeap) pop_heap(fData.begin() + idx, fData.end(), less<T>());
			else pop_heap(fData.begin() + idx, fData.end(), greater<T>());
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

		T operator[](int idx)
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
		vector<T> fData;
		size_t fSize;
		bool fIsMaxHeap;

		void swap(vector<T>& data, T i, T j)
		{
			T tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
		}
	};
}

#endif
