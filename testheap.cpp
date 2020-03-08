#include "utils.h"

void test_heap()
{
	cout << "Study of Heap: " << endl;
	vector<int> n{ 1,10,2,22,33,50,20,99 };
	Test::Heap myheap(n, true);
	cout << " Initial heap: " << endl;
	myheap.print();
	cout << "Max value: " << myheap.max() << " and its position:" << myheap.find(myheap.max()) << endl;
	int idx = myheap.find(33);
	cout << "Find 33" << " idx = " << idx << " parent:  ";
	cout << myheap[myheap.parentIdx(idx)] << " left: " << myheap[myheap.leftChildIdx(idx)] << " right: " << myheap[myheap.rightChildIdx(idx)] << endl;
	idx = myheap.find(50);
	cout << "Find 50" << " idx = " << idx << "parent: ";
	cout << myheap[myheap.parentIdx(idx)] << " left: " << myheap[myheap.leftChildIdx(idx)] << " right: " << myheap[myheap.rightChildIdx(idx)] << endl;
	idx = myheap.find(22);
	cout << "Find 22" << " idx = " << idx << "parent: ";
	cout << myheap[myheap.parentIdx(idx)] << " left: " << myheap[myheap.leftChildIdx(idx)] << " right: " << myheap[myheap.rightChildIdx(idx)] << endl;
	idx = myheap.find(2);
	cout << "Find 2" << " idx = " << idx << "parent: ";
	cout << myheap[myheap.parentIdx(idx)] << " left: " << myheap[myheap.leftChildIdx(idx)] << " right: " << myheap[myheap.rightChildIdx(idx)] << endl;

	cout << "Delete 50 : " << endl;
	myheap.deleteVal(50);
	myheap.print();

	cout << "Insert 40: " << endl;
	myheap.push(40);
	myheap.print();

	cout << "Delete 2: " << endl;
	myheap.deleteVal(2);
	myheap.print();

}