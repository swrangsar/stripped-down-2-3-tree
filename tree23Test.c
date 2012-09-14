#include <stdio.h>
#include <stdlib.h>
#include "tree23.h"

void initialize23(tree23* tree, int initial, int last)
{
	int i = 0;
	for (i = initial; i <= last; i++) {
		tree23Insert(tree, i);
	}
}

	
int main(int argc, char* argv[])
{
	tree23* tree = makeTree23();
	initialize23(tree, 1, 32);

	return 0;
}
