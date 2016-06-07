#include "stdafx.h"
#include <iostream>
#include <map>
#include <iomanip>
#define MAXELEMENTS 200000

using namespace std;

struct treeNode{
	int value;
	treeNode* left;
	treeNode* right;
	treeNode(int data);
	bool lookUp(int value);
};

treeNode::treeNode(int data) {
	this->value = data;
	this->left = NULL;
	this->right = NULL;
}

static int lookup(struct treeNode* node, int target) {
	if (node == NULL) 
		return(false);

		if (target == node->value) 
			return(true);
		else {
			if (target < node->value)
				return(lookup(node->left, target));
			else
				return(lookup(node->right, target));
		}
}

treeNode* insert(struct treeNode* node, int data) {
	if (node == NULL) {
		return new treeNode(data);
	}
	else {
		if (data <= node->value) 
			node->left = insert(node->left, data);
		else 
			node->right = insert(node->right, data);

		return(node); // return the (unchanged) node pointer 
	}
}

treeNode* findPredecessorByValue(treeNode* root) {
	static treeNode* pred;
	if (root == NULL) {
		return pred;
	}
	else {
		pred = root;
		return findPredecessorByValue(root->right);
	}
}


treeNode* deleteElem(treeNode* root, int elem) {
    treeNode* save;
    if(root == NULL) {
        printf("Element not in the tree !\n");
        return NULL;
    }

    if(root->value == elem) {
        if(root->right == NULL && root->left == NULL) {                  // no child
            free(root);
            return NULL;
        }
        else if(root->right == NULL || root->left == NULL) {             // one child
            if(root->right == NULL) {
                save = root->left;
                free(root);
                return save;
            }
            else {
                save = root->right;
                free(root);
                return save;
            }
        }
        else {                                                             // two children
			save = findPredecessorByValue(root->left);
            root->value = save->value;
            root->left = deleteElem(root->left, root->value);
            return root;
        }
    }

    else if(root->value < elem) {
        root->right = deleteElem(root->right, elem);
    }
    else if(root->value > elem) {
        root->left = deleteElem(root->left, elem);
    }

    return root;
}

void PrintTree(treeNode* node){
	if (node == NULL)
		return;
	PrintTree(node->left);
	cout << node->value<<" ";
	PrintTree(node->right);
}

int main()
{
	clock_t tStart = clock();

	int randoms[MAXELEMENTS];
	treeNode* root = NULL;

	for (int i = 0; i < MAXELEMENTS; i++){
		randoms[i] = rand();
		root = insert(root, randoms[i]);
	}

	for (int i = 5; i < MAXELEMENTS; i++){
		root = deleteElem(root, randoms[i]);
	}

	//treeNode* left = insert(root, 20);
	//treeNode* right = insert(root, 70);
	//treeNode* rightRight = insert(right, 100);
	//treeNode* leftLeft = insert(left, -10);

	PrintTree(root);

	cout << endl;
	//treeNode* save;
	//save = deleteElem(root, -10);
	//treeNode* save1 = deleteElem(save, 100);
	//treeNode* save2 = deleteElem(save1, 50);
	//treeNode* save3 = deleteElem(save2, 20);
	//treeNode* save4 = deleteElem(save3, 70);

	printf("\nTime taken: %.2fs\n\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	//cout << endl << lookup(root, -10) << " " << lookup(root, 100) <<" "<< lookup(root, -20) << " " << lookup(root, -50);
	return 0;
}
