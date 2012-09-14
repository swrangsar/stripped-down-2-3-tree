#ifndef _tree23_h
#define _tree23_h

typedef struct node23 {
	int small;
	int large;
	struct node23* left;
	struct node23* middle;
	struct node23* right;
	struct node23* parent;
} node23;

typedef struct tree23 {
	node23* root;
} tree23;

node23* makeNode23()
tree23* makeTree23()
int isLeaf(node23* node);
node23* target23(node23* node, int key);
node23* search23(node23* node, int key);

/* functions related to inserting into the 2-3 tree */
int* rearrange(int a, int b, int c);
node23* insert23(tree23* tree, node23* node, int key);
tree23* pushup(tree23* tree, node23* node, int key)								;
void tree23Insert(tree23* tree, int key);

/*functions related to deleting */
node23* successor23(node23* node, int key);
void swapWithSuccessorRecursive(node23* node, int key);
int countNode23(node23* node);
node23* delete23(tree23* tree, node23* node, int key);
tree23* fixNode23(tree23* tree, node23* node);
void destroy23(node23* node);

/* functions for printing the 2-3 tree */
void print23(tree23* tree);
void depthFirstPrint23(node23* node);
void print23Node(node23* node);

#endif
