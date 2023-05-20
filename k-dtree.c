#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "k-dtree.h"

bst_node_t *__bst_node_create(bst_tree_t *bst_tree, void *data)
{
	bst_node_t *bst_node;

	bst_node = malloc(sizeof(*bst_node));

	DIE(!bst_node, "Malloc for bst_node failed!");

	bst_node->left = NULL;
	bst_node->right = NULL;
	bst_node->dim = bst_tree->dimensions;

	bst_node->coordinates = malloc(bst_tree->data_size * bst_node->dim);
	DIE(!bst_node->coordinates, "Alloc for coordinates failed!");
	memcpy(bst_node->coordinates, data, bst_tree->data_size * bst_node->dim);

	return bst_node;
}

bst_tree_t *bst_tree_create(size_t data_size)
{
	bst_tree_t *bst_tree;

	bst_tree = malloc(sizeof(*bst_tree));
	DIE(!bst_tree, "Alloc for bst_tree failed!");

	bst_tree->root = NULL;
	bst_tree->data_size = data_size;

	return bst_tree;
}

void bst_tree_insert(bst_tree_t *bst_tree, void *data)
{
	bst_node_t *node = __bst_node_create(bst_tree, data);

	if (!bst_tree->root) {
		bst_tree->root = node;
		return;
	}

	bst_node_t *curr = bst_tree->root;
	int curr_level = 0;
	while (1) {
		int dimension = curr_level % bst_tree->dimensions;
		if (((int *)node->coordinates)[dimension] >=
		    ((int *)curr->coordinates)[dimension]) {
			if (!curr->right) {
				curr->right = node;
				return;
			}
			curr = curr->right;
		} else {
			if (!curr->left) {
				curr->left = node;
				return;
			}
			curr = curr->left;
		}
		curr_level++;
	}
}

void __bst_tree_free(bst_node_t *bst_node)
{
	if (!bst_node)
		return;

	__bst_tree_free(bst_node->left);
	__bst_tree_free(bst_node->right);
	free(bst_node->coordinates);
	free(bst_node);
}

void bst_tree_free(bst_tree_t *bst_tree)
{
	__bst_tree_free(bst_tree->root);
	free(bst_tree);
}
