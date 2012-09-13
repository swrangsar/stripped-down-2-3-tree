#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "printTree23.h"

typedef struct tree23Node {
	int min;
	int small;
	int large;
	struct tree23Node* left;
	struct tree23Node* middle;
	struct tree23Node* right;
	struct tree23Node* parent;
} tree23Node;

typedef struct tree23 {
	tree23Node* root;
} tree23;

int isLeaf(tree23Node* node)
{
	if (!tree23Node->left && !tree23Node->middle && !tree23Node->right) {
		return 1;
	} else {
		return 0;
	}
}

tree23Node* position(tree23Node* node, int key)
{
	if (isLeaf(node)) return node;
	if (key == node->small || key == node->large) return NULL;
	if (!node->large) {
		if (key < node->small && node->left) position(node->left);
		if (key > node->small && node->middle) position(node->middle);
	} else {
		if (key < node->small && node->left) position(node->left);
		if (key > node->small && key < node->large && node->middle) position(node->middle);
		if (key > node->large && node->right) position(node->right);
	}
	return NULL;
}

tree23Node* search(tree23Node* node, int key)
{
	if (key == node->small || key == node->large) {
		return node;
	} else if (isLeaf(node)) {
		return NULL;
	}
	if (!node->large) {
		if (key < node->small && node->left) position(node->left);
		if (key > node->small && node->middle) position(node->middle);
	} else {
		if (key < node->small && node->left) position(node->left);
		if (key > node->small && key < node->large && node->middle) position(node->middle);
		if (key > node->large && node->right) position(node->right);
	}
}


int* rearrange(int a, int b, int c)
{
	int* new = malloc(sizeof(int)*3);
	new[0] = a; new[1] = b; new[2] = c;
	int i = 0;
	for (; i < 3; i++) {
		int temp;
		if (new[i] > new[i+1]) {
			temp = new[i];
			new[i] = new[i+1];
			new[i+1] = temp;
		}
	}
	return new;
}

tree23Node* insert(tree23* tree, tree23Node* node, int key)
{
	if (isLeaf(node)) {
		if (!node->small) {
			node->small = key;
		} else if (!node->large) {
			node->large = key;
		} else {
			int* new = rearrange(node->small, node->large, key);
			node->small = new[0]; node->large = new[2];
			pushup(tree, node, new[1]);
		}
		return node;
	} else {
		tree23Node* tempLeaf = position(node, key);
		if (tempLeaf) insert(tree, tempLeaf, key);
		return tempLeaf;
	}
}

tree23Node* init23Node(tree23Node* node)
{
	node->min = node->small = node->large = 0;
	node->left = node->middle = node->right = node->parent = NULL;
	return node;
}
	
tree23* pushup(tree23* tree, tree23Node* node, int key)
{
	if (isLeaf(node) || !isLeaf(node)) {
		/* this part turned out to be the same for both leafs and non-leafs */
		if (!node->parent) {
			tree23Node* new = init23Node(malloc(sizeof(tree23Node)));
			tree23Node* new2 = init23Node(malloc(sizeof(tree23Node)));
			new->small = node->large; node->large = 0; new->parent = node->parent = new2;
			new2->small = key; new2->left = node; new2->right = new;
			tree->root = new2;
			return tree;
		} else {
			tree23Node* new = init23Node(malloc(sizeof(tree23Node)));
			new->small = node->large; node->large = 0;
			if (node->parent->large) {
				if (key < node->parent->small) {
					tree23Node* new2 = init23Node(malloc(sizeof(tree23Node)));
					new2->left = node->parent->middle;new2->middle = node->parent->right;
					new2->small = node->parent->large; node->parent->middle=node->parent->right = NULL;
					node->parent->middle = new; node->parent->large = 0;
					int newKey = node->parent->small; node->parent->small = key;
					pushup(tree, node->parent, newKey);
				}
				if (key > node->parent->small && key < node->parent->large) {
					tree23Node* new2 = init23Node(malloc(sizeof(tree23Node)));
					new2->left = new; new2->middle = node->parent->right;
					new2->small = node->parent->large; node->parent->right = new;
					node->parent->large = 0;
					pushup(tree, node->parent, key);
				}
				if (key > node->parent->large) {
					tree23Node* new2 = init23Node(malloc(sizeof(tree23Node)));
					new2->left = node->parent->right; new2->middle = new;
					new2->small = key; node->parent->right = NULL;
					int newKey = node->parent->large; node->parent->large  = 0;
					pushup(tree, node->parent, newKey);
				}					
			} else {
				if (key < node->parent->small) {
					node->parent->right = node->parent->middle;
					node->parent->large = node->parent->small;
					node->parent->middle = new;
					node->parent->small = key;
				}
				if (key > node->parent->small) {
					node->parent->right = new;
					node->parent->large = key;
				}
				return tree;
			}
		}
	}
}
								
void tree23Insert(tree23* tree, int key)
{
	if (!tree) {
		perror("The tree pointer points to NULL!\n");
		exit(1);
	}
	if (tree->root) insert(tree, tree->root, key);
	if (!tree->root) {
		tree23Node* new = init23Node(malloc(sizeof(tree23)));
		tree->root = new;
		tree23Insert(tree, key);
	}
}

tree23Node* successor(tree23Node* node, int key)
{
	if (key != node->small && key != node->large) {
		return NULL;
	}
	if (key == node->small) {
		return node->middle;
	}
	if (key == node->large) {
		return node->right;
	}
}

void swapWithSuccessorRecursive(tree23Node* node, int key)
{
	if (successor(node, key)) {
		tree23Node* temp = successor(node, key);
		if (key == node->small) {
			node->small = temp->small;
			temp->small = key;
		}
		if (key == node->large) {
			node->large = temp->small;
			temp->small = key;
		}
		swapWithSuccessorRecursive(temp, key);
	}
}

int node23Count(tree23Node* node)
{
	int n = 0;
	if (node->small) n++;
	if (node->large) n++;
	return n;
}					

tree23Node* delete(tree23* tree, tree23Node* node, int key)
{
	if (!isLeaf(node)) {
		tree23Node* match = search(node, key);
		if (match)) {
			swapWithRecursive(match, key)
			delete(tree, match, key);
			return node;
		} else {
			return NULL;
		}
	} else {
		/* if the node is  a leaf implement delete for the leaf */
		if (key == node->large) {
			node->large = 0;
			return node;
		}
		if (key == node->small && node->large) {
			node->small = node->large;
			node->large = 0;
			return node;
		}
		if (key == node->small && !node->large) {
			/* if parent is NULL */
			if (!node->parent) {
				tree->root = NULL;
				return NULL;
			}
			/* assuming the parent is not NULL */
			if (node23Count(node->parent) == 1) {
				if (key < node->parent->small) {
					node->small = node->parent->small;
					node->parent->small = node->parent->middle->small;
					if (node23Count(node->parent->middle) == 2) {
						node->parent->middle->small = node->parent->middle->large;
						node->parent->middle->large = 0;
						return node;
					}
					node->parent->small = 0;
					fixNode23(node->parent);
					return node;
				} else {
					node->small = node->parent->small;
					node->parent->small = 0;
					if (node23Count(node->parent->left) == 2) {
						node->parent->small = node->parent->left->large;
						node->parent->left->large = 0;
					}
					fixNode23(node->parent);
					return node;
				}
			} else if (node23count(node->parent) == 2) {
				if (key < node->parent->small) {
					if (node23Count(node->parent->middle) == 2) {
						node->small = node->parent->small;
						node->parent->small = node->parent->middle->small;
						node->parent->middle->small = node->parent->middle->large;
						node->parent->middle->large = 0;
						return node;
					} else if (node23Count(node->parent->right) == 2) {
						node->small = node->parent->small;
						node->parent->small = node->parent->middle->small;		
						node->parent->middle->small = node->parent->large;
						node->parent->large = node->parent->right->small;
						node->parent->right->small = node->parent->right->large;
						node->parent->right->large = 0;
						return node;
					} else {
						node->small = node->parent->small;
						node->large = node->parent->middle->small;
						node->parent->small = node->parent->large;
						node->parent->middle->small = node->parent->right->small;
						node->parent->large = 0;
						free(node->parent->right);
						return node;
					}
				} else if (key < node->parent->small && key > node->parent->large) {
					if (node23Count(node->parent->left) == 2) {
						node->small = node->parent->small;
						node->parent->small = node->parent->left->large;
						node->parent->left->large = 0;
						return node;
					} else if (node23Count(node->parent->right) == 2) {
						node->small = node->parent->large;
						node->parent->large = node->parent->right->small;
						node->parent->right->small = node->parent->right->large;
						node->parent->right->large = 0;
						return node;
					} else {
						node->small = node->parent->large;
						node->large = node->parent->right->small;
						node->parent->large = 0;
						free(node->parent->right);
						return node;
					}
				} else {
					if (node23Count(node->parent->middle)==2) {
						node->small = node->parent->large;
						node->parent->large = node->parent->middle->large;
						node->parent->middle->large = 0;
						return node;
					} else if (node23Count(node->parent->left) == 2) {
						node->small = node->parent->large;
						node->parent->large = node->parent->middle->small;
						node->parent->middle->small = node->parent->small;
						node->parent->small = node->parent->left->large;
						node->parent->left->large = 0;
						return node;
					} else {
						node->parent->middle->large = node->parent->large;
						node->parent->large = 0;
						free(node->parent->right);
						return node;
					}
				}
			}
		}
	}
}
			
	
