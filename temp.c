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
					node->parent->small = key;
					return;
				}
				if (key > node->parent->small) {
					node->parent->right = newChild;
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
					int newKey = node->parent->small;
					node->parent->small = key;
					splitNode23(tree, node->parent, newNode, newKey);
					return;
				}
				if (key > node->parent->small && key < node->parent->large) {
					newNode->left = newChild; newNode->middle = node->parent->right;
					newNode->small = node->parent->large;
					node->parent->right = NULL;					
					node->parent->large = 0;
					splitNode23(tree, node->parent, newNode, key);
					return;
				}
				if (key > node->parent->large) {
					newNode->left = node->parent->right; newNode->middle = newChild;
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
			node->parent->small = key;
			return;
		}
		if (key > node->parent->small) {
			node->parent->right = newChild;
			node->parent->large = key;
			return;
		}
	}
	if (keyCount23(node->parent) == 2) {
		node23* newNode = makeNode23();
		if (key < node->parent->small) {
			newNode->left = node->parent->middle; newNode->small = node->parent->large; newNode->middle = node->parent->right;
			node->parent->middle = newChild; node->parent->large = 0; node->parent->right = NULL;
			int newKey = node->parent->small; node->parent->small = key;
			splitNode23(tree, node->parent, newNode, newKey);
			return;
		}
		if (key > node->parent->small && key < node->parent->large) {
			newNode->left = newChild; newNode->small = node->parent->large; newNode->middle = node->parent->right;
			node->parent->large = 0; node->parent->right = NULL;
			int newKey = key; 
			splitNode23(tree, node->parent, newNode, newKey);
			return;
		}
		if (key > node->parent->large) {
			newNode->left = node->parent->right; newNode->small = key; newNode->middle = newChild;
			node->parent->right = NULL;
			int newKey = node->parent->large; node->parent->large = 0;
			splitNode23(tree, node->parent, newNode, newKey);
			return;
		}
	}
}