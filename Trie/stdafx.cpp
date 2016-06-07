//TASK 2
#include "stdafx.h"
#include <iostream>
#include <vector>
#include<string>
using namespace std;

class Node
{
public:
	Node(int i)
	{
		index = i;
		mContent = ' ';
		mMarker = false;
	}

	~Node() {}

	int inx(){ return index; }
	char content() { return mContent; }
	void setContent(char c) { mContent = c; }
	bool wordMarker() { return mMarker; }
	void setWordMarker() { mMarker = true; }
	Node* findChild(char c);
	void appendChild(Node* child) { mChildren.push_back(child); }
	vector<Node*> children() { return mChildren; }

private:
	char mContent;
	bool mMarker;
	int index;
	vector<Node*> mChildren;
};

class Trie {
public:
	Trie();
	~Trie();
	void addWord(string s, int i);
	int searchWord(string s);
	void deleteWord(string s);
private:
	Node* root;
};

Node* Node::findChild(char c)
{
	for (int i = 0; i < mChildren.size(); i++)
	{
		Node* tmp = mChildren.at(i);
		if (tmp->content() == c)
		{
			return tmp;
		}
	}

	return NULL;
}

Trie::Trie()
{
	root = new Node(-1);
}

Trie::~Trie()
{
	// Free memory
}

void Trie::addWord(string s, int ind)
{
	Node* current = root;

	if (s.length() == 0)
	{
		current->setWordMarker(); // an empty word
		return;
	}

	for (int i = 0; i < s.length(); i++)
	{
		Node* child = current->findChild(s[i]);
		if (child != NULL)
		{
			current = child;
		}
		else
		{
			Node* tmp = new Node(ind);
			tmp->setContent(s[i]);
			current->appendChild(tmp);
			current = tmp;
		}
		if (i == s.length() - 1)
			current->setWordMarker();
	}
}


int Trie::searchWord(string s)
{
	Node* current = root;

	while (current != NULL)
	{
		for (int i = 0; i < s.length(); i++)
		{
			Node* tmp = current->findChild(s[i]);
			if (tmp == NULL)
				return -1;
			current = tmp;
		}

		if (current->wordMarker())
			return current->inx();
		else
			return -1;
	}

	return -1;
}

// Test program
int main()
{
	Trie* trie = new Trie();

	int n, stringIndex1 = 0, stringIndex2 = 0, bestResult = 0;
	string s;

	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> s;
		//getline(cin, s);
		if (i > 0)
		{
			for (int j = 1; j < s.length(); j++)
			{
				string ss = s.substr(0, j);
				int addedIn = trie->searchWord(ss);
				if (addedIn != -1)
				{
					if (bestResult < j)
					{
						bestResult = j;
						stringIndex1 = addedIn;
						stringIndex2 = i;
					}
				}
			}
		}
		trie->addWord(s, i);
	}

	//Having strings starting from 1
	cout << stringIndex1 + 1 << " " << stringIndex2 + 1 << endl;

	delete trie;
	return 0;
}
