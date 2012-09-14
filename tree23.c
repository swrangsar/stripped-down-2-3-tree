#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "tree23.h"
#define DEBUG


tree23* makeTree23()
{
	tree23* new = malloc(sizeof(tree23));
	if (!new) {
		perror("Out of memory for 2-3 tree...\n");
		exit(1);
	}
	new->root = NULL;
	return new;
}

node23* makeNode23()
{
	node23* new = malloc(sizeof(node23));
	if (!new) {
		perror("Out of memory for 2-3 node...\n");
		exit(1);
	}
	new->small = new->large = 0;
	new->left = new->middle = new->right = new->parent = NULL;
	return new;
}

void initialize23(tree23* tree, int initial, int last)
{
	int i = 0;
	for (i = initial; i <= last; i++) {
		insert23(tree, i);
	}
}

int isLeaf(node23* node)
{
	if (!node->left && !node->middle && !node->right) {
		return 1;
	} else {
		return 0;
	}
}

node23* target23(node23* node, int key)
{
	node23* new = NULL;
	if (isLeaf(node)) return node;
	if (key == node->small || key == node->large) return NULL;
	if (keyCount23(node) == 1) {
		if (key < node->small && node->left) new = target23(node->left, key);
		if (key > node->small && node->middle) new = target23(node->middle, key);
	} else {
		if (key < node->small && node->left) new = target23(node->left, key);
		if (key > node->small && key < node->large && node->middle) new = target23(node->middle, key);
		if (key > node->large && node->right) new = target23(node->right, key);
	}
	return new;
}

node23* search23(node23* node, int key)
{
	node23* new = NULL;
	if (key == node->small || key == node->large) {
		return node;
	} else if (isLeaf(node)) {
		return NULL;
	}
	if (!node->large) {
		if (key < node->small && node->left) new = target23(node->left, key);
		if (key > node->small && node->middle) new = target23(node->middle, key);
	} else {
		if (key < node->small && node->left) new = target23(node->left, key);
		if (key > node->small && key < node->large && node->middle) new = target23(node->middle, key);
		if (key > node->large && node->right) new = target23(node->right, key);
	}
	return new;
}


int* rearrange(int a, int b, int c)
{
	int* new = malloc(sizeof(int)*3);
	new[0] = a; new[1] = b; new[2] = c;
	int i, j;
	for (j =0; j<3; j++) {
		for (i = 0; i < 2; i++) {
		int temp;
		if (new[i] > new[i+1]) {
			temp = new[i];
			new[i] = new[i+1];
			new[i+1] = temp;
		}
	}
	}
	#ifdef DEBUG
		printf("a: %d b: %d c %d\n", new[0], new[1], new[2]);
	#endif
	return new;
}

void insertIntoNode23(tree23* tree, node23* node, int key)
{
	if (isLeaf(node)) {
		if (!keyCount23(node)) {
			node->small = key;
		} else if (keyCount23(node) == 1) {
			node->large = key;
		} else {
			int* new = rearrange(node->small, node->large, key);
			node->small = new[0]; node->large = new[2];
			splitLeaf23(tree, node, new[1]);				
		}
		return;
	} else {
		node23* tempLeaf = target23(node, key);
		if (tempLeaf) insertIntoNode23(tree, tempLeaf, key);
		return;
	}
}

void splitLeaf23(tree23* tree, node23* node, int key)
{
	if (isLeaf(node)) {
		/* this part turned out to be the same for both leafs and non-leafs. no i did a mistake
		 * they cant be the same we'll write a new function below
		 */
		if (!node->parent) {
			node23* newChild = makeNode23();
			node23* newRoot = makeNode23();
			newChild->small = node->large; node->large = 0; newChild->parent = node->parent = newRoot;
			newRoot->small = key; newRoot->left = node; newRoot->middle = newChild;
			tree->root = newRoot;
			return;
		}
		if (node->parent) {
			node23* newChild = makeNode23();
			newChild->small = node->large; node->large = 0;
			if (keyCount23(node->parent) == 1) {
				if (key < node->parent->small) {
					node->parent->right = node->parent->middle;
					node->parent->large = node->parent->small;
					node->parent->middle = newChild;
					newChild->parent = node->parent;
					node->parent->small = key;
					return;
				}
				if (key > node->parent->small) {
					node->parent->right = newChild;
					newChild->parent = node->parent;
					node->parent->large = key;
					return;
				}
			}
			if (keyCount23(node->parent) == 2) {
				node23* newNode = makeNode23();
				if (key < node->parent->small) {
					newNode->left = node->parent->middle;newNode->middle = node->parent->right;
					newNode->small = node->parent->large;
					node->parent->middle = node->parent->right = NULL;
					node->parent->large = 0;
					node->parent->middle = newChild; 
					newChild->parent = node->parent;
					int newKey = node->parent->small;
					node->parent->small = key;
					splitNode23(tree, node->parent, newNode, newKey);
					return;
				}
				if (key > node->parent->small && key < node->parent->large) {
					newNode->left = newChild; newChild->parent = newNode; newNode->middle = node->parent->right;
					newNode->small = node->parent->large;
					node->parent->right = NULL;					
					node->parent->large = 0;
					splitNode23(tree, node->parent, newNode, key);
					return;
				}
				if (key > node->parent->large) {
					newNode->left = node->parent->right; newNode->middle = newChild;
					newChild->parent = newNode;
					newNode->small = key; node->parent->right = NULL;
					int newKey = node->parent->large; node->parent->large  = 0;
					splitNode23(tree, node->parent, newNode, newKey);
					return;
				}					
			}
		}
	}		
}

void splitNode23(tree23* tree, node23* node, node23* newChild, int key)
{
	if (isLeaf(node)) {
		printf("This function is meant for an internal node and not a leaf node.\n");
		exit(1);
	}
	if (!node->parent) {
		node23* newRoot = makeNode23();
		newRoot->small = key;
		newRoot->left = node; newRoot->middle = newChild;
		node->parent = newChild->parent = newRoot;
		tree->root = newRoot;
		return;
	}
	if (keyCount23(node->parent) == 1) {
		if (key < node->parent->small) {
			node->parent->right = node->parent->middle;
			node->parent->large = node->parent->small;
			node->parent->middle = newChild;
			newChild->parent = node->parent;
			node->parent->small = key;
			return;
		}
		if (key > node->parent->small) {
			node->parent->right = newChild;
			newChild->parent = node->parent;
			node->parent->large = key;
			return;
		}
	}
	if (keyCount23(node->parent) == 2) {
		node23* newNode = makeNode23();
		if (key < node->parent->small) {
			newNode->left = node->parent->middle; newNode->small = node->parent->large; newNode->middle = node->parent->right;
			node->parent->middle = newChild; newChild->parent = node->parent; node->parent->large = 0; node->parent->right = NULL;
			int newKey = node->parent->small; node->parent->small = key;
			splitNode23(tree, node->parent, newNode, newKey);
			return;
		}
		if (key > node->parent->small && key < node->parent->large) {
			newNode->left = newChild; newChild->parent = newNode; newNode->small = node->parent->large; newNode->middle = node->parent->right;
			node->parent->large = 0; node->parent->right = NULL;
			int newKey = key; 
			splitNode23(tree, node->parent, newNode, newKey);
			return;
		}
		if (key > node->parent->large) {
			newNode->left = node->parent->right; newNode->small = key; newNode->middle = newChild;
			newChild->parent = newNode;
			node->parent->right = NULL;
			int newKey = node->parent->large; node->parent->large = 0;
			splitNode23(tree, node->parent, newNode, newKey);
			return;
		}
	}
}
								
void insert23(tree23* tree, int key)
{
	if (!tree) {
		perror("The tree pointer points to NULL!\n");
		exit(1);
	}
	#ifdef DEBUG
		//printf("inserting into 2-3 tree...\n");
		//if (tree->root) printf("tree has root...\n");
	#endif
	if (tree->root) insertIntoNode23(tree, tree->root, key);
	if (!tree->root) {
		node23* new = makeNode23();
		if (!new) printf("Out of memory...in insert23()\n");
		tree->root = new;
		insert23(tree, key);
		return;
	}
}

node23* successor(node23* node, int key)
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
	return NULL;
}

void swapWithSuccessorRecursive(node23* node, int key)
{
	if (successor(node, key)) {
		node23* temp = successor(node, key);
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

int keyCount23(node23* node)
{
	int n = 0;
	if (node->small) n++;
	if (node->large) n++;
	return n;
}					

node23* deleteNode23(tree23* tree, node23* node, int key)
{
	if (!isLeaf(node)) {
		node23* match = search23(node, key);
		if (match) {
			swapWithSuccessorRecursive(match, key);
			deleteNode23(tree, match, key);
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
			if (keyCount23(node->parent) == 1) {
				if (key < node->parent->small) {
					node->small = node->parent->small;
					node->parent->small = node->parent->middle->small;
					if (keyCount23(node->parent->middle) == 2) {
						node->parent->middle->small = node->parent->middle->large;
						node->parent->middle->large = 0;
						return node;
					}
					node->parent->small = 0;
					node->large = node->parent->middle->small;
					destroy23(node->parent->middle);
					fixNode23(tree, node->parent);
					return node;
				} else {
					node->small = node->parent->small;
					node->parent->small = 0;
					if (keyCount23(node->parent->left) == 2) {
						node->parent->small = node->parent->left->large;
						node->parent->left->large = 0;
						return node;
					}
					node->parent->left->large = node->small;
					destroy23(node->parent->middle);
					fixNode23(tree, node->parent);
					return node;
				}
			} else if (keyCount23(node->parent) == 2) {
				if (key < node->parent->small) {
					if (keyCount23(node->parent->middle) == 2) {
						node->small = node->parent->small;
						node->parent->small = node->parent->middle->small;
						node->parent->middle->small = node->parent->middle->large;
						node->parent->middle->large = 0;
						return node;
					} else if (keyCount23(node->parent->right) == 2) {
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
						destroy23(node->parent->right);
						return node;
					}
				} else if (key < node->parent->small && key > node->parent->large) {
					if (keyCount23(node->parent->left) == 2) {
						node->small = node->parent->small;
						node->parent->small = node->parent->left->large;
						node->parent->left->large = 0;
						return node;
					} else if (keyCount23(node->parent->right) == 2) {
						node->small = node->parent->large;
						node->parent->large = node->parent->right->small;
						node->parent->right->small = node->parent->right->large;
						node->parent->right->large = 0;
						return node;
					} else {
						node->small = node->parent->large;
						node->large = node->parent->right->small;
						node->parent->large = 0;
						destroy23(node->parent->right);
						return node;
					}
				} else {
					if (keyCount23(node->parent->middle)==2) {
						node->small = node->parent->large;
						node->parent->large = node->parent->middle->large;
						node->parent->middle->large = 0;
						return node;
					} else if (keyCount23(node->parent->left) == 2) {
						node->small = node->parent->large;
						node->parent->large = node->parent->middle->small;
						node->parent->middle->small = node->parent->small;
						node->parent->small = node->parent->left->large;
						node->parent->left->large = 0;
						return node;
					} else {
						node->parent->middle->large = node->parent->large;
						node->parent->large = 0;
						destroy23(node->parent->right);
						return node;
					}
				}
			}
		}
	}
	return NULL;
}

void delete23(tree23* tree, int key)
{
	if (tree && tree->root) deleteNode23(tree, tree->root, key);
}

tree23* fixNode23(tree23* tree, node23* node)
{
	if (!node->parent) {
		tree->root = node->left; 
		destroy23(node);
		return tree;
	}
	if (keyCount23(node->parent) == 1) {
		if (node == node->parent->left) {
			if (keyCount23(node->parent->middle) == 2) {
				node->small = node->parent->small;
				node->middle = node->parent->middle->left;
				node->parent->small = node->parent->middle->small;
				node->parent->middle->left = node->parent->middle->middle;
				node->parent->middle->small = node->parent->middle->large;
				node->parent->middle->middle = node->parent->middle->right;
				node->parent->middle->large = 0;
				node->parent->middle->right = NULL;
				return tree;
			}
			if (keyCount23(node->parent->middle) == 1) {
				node->parent->middle->right = node->parent->middle->middle;
				node->parent->middle->large = node->parent->middle->small;
				node->parent->middle->middle = node->parent->middle->left;
				node->parent->middle->small = node->parent->small;
				node->parent->middle->left = node->left;
				node->parent->small = 0;
				node->parent->left = node->parent->middle;
				node->parent->middle = NULL;
				fixNode23(tree, node->parent);
				return tree;
			}
		}
		if (node == node->parent->middle) {
			if (keyCount23(node->parent->left) == 2) {
				node->middle = node->left;
				node->small = node->parent->small;
				node->left = node->parent->left->right;
				node->parent->small = node->parent->left->large;
				node->parent->left->right = NULL;
				node->parent->left->large = 0;
				return tree;
			}
			if (keyCount23(node->parent->left) == 1) {
				node->parent->left->large = node->parent->small;
				node->parent->left->right = node->left;
				node->parent->small = 0;
				node->parent->middle = NULL;
				fixNode23(tree, node->parent);
				return tree;
			}
		}
	}
	if (keyCount23(node->parent) == 2) {
		if (node == node->parent->left) {
			if (keyCount23(node->parent->middle) == 2) {
				node->small = node->parent->small;
				node->middle = node->parent->middle->left;
				node->parent->small = node->parent->middle->small;
				node->parent->middle->left = node->parent->middle->middle;
				node->parent->middle->small = node->parent->middle->large;
				node->parent->middle->middle = node->parent->middle->right;
				node->parent->middle->large = 0;
				node->parent->middle->right = NULL;
				return tree;
			}
			if (keyCount23(node->parent->right) == 2) {
				node->small = node->parent->small;
				node->middle = node->parent->middle->left;
				node->parent->small = node->parent->middle->small;
				node->parent->middle->left = node->parent->middle->middle;
				node->parent->middle->small = node->parent->large;
				node->parent->middle->middle = node->parent->right->left;
				node->parent->large = node->parent->right->small;
				node->parent->right->left = node->parent->right->middle;
				node->parent->right->small = node->parent->right->large;
				node->parent->right->middle = node->parent->right->right;
				node->parent->right->large = 0;
				node->parent->right->right = NULL;
				return tree;
			} else {
				node->small = node->parent->small;
				node->middle = node->parent->middle->left;
				node->large = node->parent->middle->small;
				node->right = node->parent->middle->middle;
				node->parent->small = node->parent->large;
				node->parent->middle = node->parent->right;
				node->parent->large = 0;
				node->parent->right = NULL;
				return tree;
			}
		}
		if (node == node->parent->middle) {
			if (keyCount23(node->parent->left) == 2) {
				node->small = node->parent->small;
				node->middle = node->left;
				node->left = node->parent->left->right;
				node->parent->small = node->parent->left->large;
				node->parent->left->right = NULL;
				node->parent->left->large = 0;
				return tree;
			}
			if (keyCount23(node->parent->right) >= 1) {
				node->parent->left->large = node->parent->small;
				node->parent->left->right = node->left;
				node->parent->small = node->parent->large;
				node = node->parent->right;
				node->parent->large = 0;
				node->parent->right = NULL;
				return tree;
			}
		}
		if (node == node->parent->right) {
			if (keyCount23(node->parent->middle) == 2) {
				node->middle = node->left;
				node->small = node->parent->large;
				node->left = node->parent->middle->right;
				node->parent->large = node->parent->middle->large;
				node->parent->middle->right = NULL;
				node->parent->middle->large = 0;
				return tree;
			} else {
				node->parent->middle->large = node->parent->large;
				node->parent->middle->right = node->left;
				node->parent->large = 0;
				node->parent->right = NULL;
				return tree;
			}
		}
	}
	return NULL;
}

/* cleanup memory after deleting a 2-3 node */
void destroy23(node23* node)
{
	printf("deallocating...\n");
	exit(1);
	if (node) {
		if (node->left) free(node->left);
		if (node->middle) free(node->middle);
		if (node->right) free(node->right);
		if (node->parent) free(node->parent);
		free(node);
	}
}

/* printing the tree kinda functions */	
void print23(tree23* tree)
{
	if (tree && tree->root) {
		depthFirstPrint23(tree->root);
	}
}

void depthFirstPrint23(node23* node)
{
	queue* nodeQueue = createQueue();
	if (!node) {
		perror("The node to be printed doesn't exist.\n");
		exit(1);
	}
	enqueue(nodeQueue, node);
	while (nodeQueue->tail) {
		node23* temp = dequeue(nodeQueue);
		printNode23(temp);
		if (temp->left) enqueue(nodeQueue, temp->left);
		if (temp->middle) enqueue(nodeQueue, temp->middle);
		if (temp->right) enqueue(nodeQueue, temp->right);
	}
}

void printNode23(node23* node)
{
	int a, b;
	a = b = 0;
	if (node->small) a = node->small;
	if (node->large) b = node->large;
	printf("small: %d large: %d\n", a, b);
}
