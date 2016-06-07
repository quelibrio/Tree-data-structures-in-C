/* Implementation of the Skip List Data structure. The following
 * implementation will work only with the primitive data types. The
 * reason is that we use the data field of the node as both key and value.
 * Standart operations insert, remove, search, print are included.
 */

#include "stdafx.h"
#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <iostream>
#include <limits>
#include <ctime>
#include <vector>
#define maxLevel 32
using std::vector;

template <typename T> class SkipList {
public:
  SkipList();                                       ///< Default Constructor
  SkipList(const SkipList<T> &other);               ///< Copy constructor
  ~SkipList();                                      ///< Destructor
  SkipList<T> &operator=(const SkipList<T> &other); ///< Assignment operator
  void insert(const T &data);       ///< Standart insert an element method
  void remove(const T &data);       ///< Standart remove an element method
  void print() const;               ///< Print all elements method
  T* search(const T &data) const; ///< Search for an element by data
  bool isEmpty() const;
private:
  struct SkipNode {
    T data;             ///< data and key of the SkipNode
    SkipNode** forward; ///< A dynamic array with links
    /// Parameterized constructor
	SkipNode(T data, int level) : data(data) {
	  forward = new SkipNode *[level];
	  for (int i = 0; i < level; i++) {
		  forward[i] = nullptr;
      }
    }
	~SkipNode(){
		delete[] forward;
	}
  } * head, *NIL; ///< pointers to the first and last node

  /// Data members of the SkipList class
  float probability;
  int currLevel;
  /// helper methods for fundamental operations
  int genRandomLevel() const;
  void copySkipList(const SkipList<T> &other);
  void deleteSkipList();
};

template <typename T>
SkipList<T>::SkipList()
    : head(nullptr), NIL(nullptr), probability(0.5), currLevel(0) {
  /// One of the reasons the Skip List works
  /// only with primitive types is the two lines below
  T headData = std::numeric_limits<T>::min();
  T NILData = std::numeric_limits<T>::max();
  head = new SkipNode(headData, maxLevel);
  NIL = new SkipNode(NILData, maxLevel);
  for (unsigned i = 0; i < maxLevel; i++) {
    head->forward[i] = NIL;
  }
  srand(time(NULL)); ///< seeding the pseudo random number generator
}
template <typename T> 
SkipList<T>::SkipList(const SkipList<T> &other) : SkipList() {
  copySkipList(other);
}
template <typename T> 
SkipList<T>::~SkipList() { 
	deleteSkipList();
}
template <typename T>
SkipList<T> &SkipList<T>::operator=(const SkipList<T> &other) {
  if (this != &other) {
    deleteSkipList();
    copySkipList(other);
  }
  return *this;
}
template <typename T> 
void SkipList<T>::insert(const T &data) {
  SkipNode *newSkipNode = head;
  SkipNode **update = new SkipNode*[maxLevel];
  for (int i = currLevel - 1; i >= 0; i--) {
	  while (newSkipNode->forward[i]->data < data) {
      newSkipNode = newSkipNode->forward[i];
    }
    update[i] = newSkipNode;
  }
  newSkipNode = newSkipNode->forward[0];
  if (newSkipNode->data != data) {
    int newLevel = genRandomLevel();
    if (newLevel > currLevel) {
      for (int i = currLevel; i < newLevel; i++) {
        update[i] = head;
      }
      currLevel = newLevel;
    }
    newSkipNode = new SkipNode(data, newLevel);
    for (int i = 0; i < newLevel; i++) {
      newSkipNode->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = newSkipNode;
    }
  }
  delete[] update;
}

template <typename T> 
void SkipList<T>::remove(const T &data) {
  SkipNode *temp = head;
  SkipNode **update = new SkipNode*[maxLevel];
  for (int i = currLevel - 1; i >= 0; i--) {
    while (temp->forward[i] != nullptr && temp->forward[i]->data < data) {
      temp = temp->forward[i];
    }
    update[i] = temp;
  }
  temp = temp->forward[0];
  if (temp->data == data) {
    int i = 0;
    while (i < currLevel && update[i]->forward[i] == temp) {
      update[i]->forward[i] = temp->forward[i];
      i++;
    }
    delete temp;
    while (currLevel > 0 && head->forward[currLevel] == NIL) {
      currLevel--;
    }
  }
  delete[] update;
}
template <typename T> 
void SkipList<T>::print() const {
  SkipNode *temp = head;
  temp = temp->forward[0];
  while (temp != NIL) {
    std::cout << temp->data << ", ";
    temp = temp->forward[0];
  }
  std::cout << std::endl;
}
template <typename T> 
T* SkipList<T>::search(const T &data) const {
  SkipNode *temp = head;
  for (int i = currLevel - 1; i >= 0; i--) {
    while (temp->forward[i]->data < data) {
      temp = temp->forward[i];
    }
  }
  temp = temp->forward[0];
  return temp->data == data ? &temp->data : nullptr;
}
template <typename T> 
int SkipList<T>::genRandomLevel() const {
  int level = 1;
  while (((double)std::rand() / RAND_MAX)< probability && level < maxLevel) {
    level++;
  }
  return level;
}
template <typename T>
void SkipList<T>::copySkipList(const SkipList<T> &other) {
	SkipNode *temp = other.head;
	temp = temp->forward[0];
	while (temp != other.NIL){
		insert(temp->data);
		temp = temp->forward[0];
	}
}
template <typename T> 
void SkipList<T>::deleteSkipList() {
	SkipNode *temp = head, *nextTemp;
	while (temp){
		nextTemp = temp->forward[0];
		delete temp;
		temp = nextTemp;
	}
	head = nullptr;
	NIL = nullptr;
}
template <typename T>
bool SkipList<T>::isEmpty() const {
	return head->forward[0] == NIL;
}
#endif