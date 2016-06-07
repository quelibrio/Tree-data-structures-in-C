#include "stdafx.h"
#include <iostream>
#include <map>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#define MAXN 1000
#define RANDOM_DOUBLE rand() / (double)RAND_MAX;
#define MAX_LEVEL 15
using namespace std;

string parseInputTree();

template <class T> 
struct treeNode
{
	T value;
	string canonicalNames;

	treeNode* parent;
	vector<treeNode*> children;

	treeNode(int value){
		this->value = value;
	}
	treeNode* insert(T value);
};

struct comparisonOperator
{
	inline bool operator() (const treeNode<int>* node1, const treeNode<int>* node2)
	{
		for (int i = 0; i < node1->canonicalNames.size() && i < node2->canonicalNames.size(); i++){
			if (node1->canonicalNames[i] < node2->canonicalNames[i])
				return true;
		}
		return false;
	}
};

template<class T>
treeNode<T>* treeNode<T>::insert(T value){
	treeNode<T>* node = new treeNode<int>(value);
	this->children.push_back(node);
	node->parent = this;
	return node;
}

template<class T>
void PrintTree(treeNode<T>* node){
	if (node == NULL)
		return;

	cout << node->value << " ";
	for (int i = 0; i < node->children.size(); i++){
		treeNode<T>* currentChild = node->children[i];
		PrintTree(currentChild);
	}
}
 
int main(){
	///Sample test:
	///(5 {(9 {}) (1 {(4 {}) (12 {}) (42 {})})})
	///(7 {(15 {(7 {}) (10 {}) (8 {})})(3 {})}) 
	///true
	///(5{(9 {})(1{(4 {})(12 {})(42 {})})(2 {(7 {})(8 {})(9 {})})})
	///(7{(2 {(7{})(8 {})(9 {})})(15 {(7 {})(10 {})(8 {})})(3 {})})
	///false
	

	string tree1 = parseInputTree();
	string tree2 = parseInputTree();
	bool result = tree1 == tree2;

	std::cout.setf(std::ios::boolalpha);
	cout << result;
}

string parseInputTree(){
	int c;
	int counter = 0;

	treeNode<int>* currentElement = &treeNode<int>(counter);

	do {
		c = getchar();

		if (c == '(')
		{
			++counter;
			currentElement = currentElement->insert(counter);
		}
		else if (c == ')')
		{
			string result = "";

			sort(currentElement->children.begin(), currentElement->children.end(), comparisonOperator());

			for (int i = 0; i < currentElement->children.size(); i++){
				result.append(currentElement->children[i]->canonicalNames);
			}

			currentElement->canonicalNames = "1" + result + "0";
			currentElement = currentElement->parent;
		}

		//putchar(c);
	} while (c != '\n');

	return currentElement->children[0]->canonicalNames;
}
