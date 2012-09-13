#include <stdio.h>
#include <stdlib.h>
#include "print23.h"

void print23(tree23* tree)
{
	if (tree && tree->root) {
		depthFirstSearch23(tree->root);
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
	int count = 0, power2 = 2, level = 0;
	while (nodeQueue->tail) {
		twoThreeNode* temp = dequeue(nodeQueue);
		printNode23(temp);
		if (temp->left) enqueue(nodeQueue, temp->left);
		if (temp->middle) enqueue(nodeQueue, temp->middle);
		if (temp->right) enqueue(nodeQueue, temp->right);
		if (count == power2) {
			printf("\n*** Level %d ***\n", level++);
			count = 0; power2 *= 2;
		}
		count++;
	}
}

void print23Node(node23* node)
{
	int a, b;
	a = b = -1;
	if (node->small) a = node->small;
	if (node->large) b = node->large;
	printf("small: %d large: %d\n", a, b);
}
