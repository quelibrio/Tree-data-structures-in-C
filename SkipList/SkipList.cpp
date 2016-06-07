// SkipList.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <map>
#include <cstdlib>
#include <cmath>
#include <cstring>
#define MAXN 1000
#define RANDOM_DOUBLE rand() / (double)RAND_MAX;
#define MAX_LEVEL 15

using namespace std;

struct skipNode{
	int value;
	skipNode * * forward;
	
	skipNode(int level, int &value)
	{
		forward = new skipNode*[level + 1];
		memset(forward, 0, sizeof(skipNode*) * (level + 1));
		this -> value = value;
	}

	~skipNode(){
		delete [] forward;
	}
};

struct skipList{
	skipNode *header;
	float probability;
	int value;
	int level;

	skipList(float probability){
		this->header = new skipNode(MAX_LEVEL, this->value);
		this->probability = probability;
		this->level = 0;
	}

	void display();
	void insertElement(int &x);
	void deleteElement(int &x);
	bool contains(int &x);
	int randomLevel();

	~skipList(){
		delete header;
	}
};

void skipList::insertElement(int &x){
	skipNode *node = header;
	skipNode *update[MAX_LEVEL + 1];

	memset(update, 0, sizeof(skipNode*) * (MAX_LEVEL + 1));

	for (int i = this->level; i >= 0; i--){
		while (node->forward[i] != NULL && node->forward[i] -> value < x)
		{
			node = node->forward[i];
		}
		update[i] = node;
	}

	node = node->forward[0];

	if (node == NULL || node->value != x){
		int currentLevel = randomLevel();
		if (currentLevel > this->level)
		{
			for (int i = this->level + 1; i < currentLevel; i++){
				update[i] = header;
			}

			this->level = currentLevel;			
		}

		skipNode *newNode = new skipNode(currentLevel, x);

		for (int i = 0; i < this->level; i++){
			newNode->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = newNode;
		}
	}
}

void skipList::deleteElement(int &x){
	skipNode *node = this->header;
	skipNode *update[MAX_LEVEL + 1];
	memset(update, 0, sizeof(skipNode*) * (MAX_LEVEL + 1));

	for (int i = this->level; i >= 0; i--){
		while (node->forward[i] != NULL && node->forward[i]->value < x)
		{
			node = node->forward[i];
		}
		update[i] = node;
	}
	node = node->forward[0];

	if (node->value == x){
		for (int i = 0; i <= this->level; i++){
			if (update[i]->forward[i] != node){
				break;
			}
			update[i]->forward[i] = node->forward[i];
		}

		//delete node;

		while (this->level > 0 && this->header->forward[this->level] == NULL)
		{
			this->level--;
		}
	}
}


void skipList::display(){
	const skipNode *node = header->forward[0];
	while (node != NULL)
	{
		cout << node->value;
		node = node->forward[0];
		if (node != NULL){
			cout << '-';
		}
		cout << endl;
	}
}

int skipList::randomLevel() {
	int level = 1;

	double randomDouble = RANDOM_DOUBLE;

	while (randomDouble < this->probability && level < MAX_LEVEL) {
		level++;
		randomDouble = RANDOM_DOUBLE;
	}

	return level;
}

bool skipList::contains(int &x){
	skipNode *currentNode = this->header;
	for (int i = this->level; i >= 0; i--){
		while (currentNode->forward[i] != NULL && currentNode->forward[i]->value < x){
			currentNode = currentNode->forward[i];
		}
	}
	currentNode = currentNode->forward[0];
	return currentNode != NULL && currentNode->value == x;
}

int main()
{
	skipList skipListInts(0.5);
	/*int numbers[10] = { 3, 5, 1, 2, 8, 11, 20, -1, -5, 9 };*/
	int numbers[MAXN];
	for (int i = 0; i < MAXN; i++){
		numbers[i] = rand();
		skipListInts.insertElement(numbers[i]);
	}

	//skipListInts.display();

	/*for (int i = 0; i < 10; i++){
		cout<< (bool)skipListInts.contains(numbers[i])<< " ";
	}
	
	cout << endl;*/

	for (int i = 5; i < MAXN; i++){
		skipListInts.deleteElement(numbers[i]);
	}

	int count = 0;
	for (int i = 0; i < MAXN; i++){
		if (skipListInts.contains(numbers[i])){
			cout << numbers[i] << " ";
			count++;
		}
	}

	return 0;
}
