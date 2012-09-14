#include <stdio.h>
#include <stdlib.h>
#include "tree23.h"

#define DEBUG
	
int main(int argc, char* argv[])
{
	int i = 0;

	tree23* tree = makeTree23();
	printf("Tree allocated...\n");
	initialize23(tree, 1, 32);
	printf("Tree initialized...\n");
	print23(tree);
	
	for (i = 15; i >= 8; i--) {
		delete23(tree, i);
	}
	print23(tree);
	
	for (i = 33; i <= 36; i++) {
		insert23(tree, i);
	}
	print23(tree);

	return 0;
}
