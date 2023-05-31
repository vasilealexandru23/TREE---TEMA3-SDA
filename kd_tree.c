#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "kd_tree.h"

kdt_node_t *kdt_node_create(kdt_tree_t *kdt_tree, void *data)
{
	/* Alloc memory for a new node and check if it failed. */
	kdt_node_t *kdt_node = malloc(sizeof(*kdt_node));
	DIE(!kdt_node, "Malloc for kdt_node failed!");

	/* Initialize data. */
	kdt_node->left = NULL;
	kdt_node->right = NULL;
	kdt_node->dim = kdt_tree->dimensions;

	/* Alloc memory for the coordinates of the node(point). */
	/* Check if the allocation failed and if not, initialiaze coordinates. */
	kdt_node->coordinates = malloc(kdt_tree->data_size * kdt_node->dim);
	DIE(!kdt_node->coordinates, "Alloc for coordinates failed!");
	memcpy(kdt_node->coordinates, data, kdt_tree->data_size * kdt_node->dim);

	return kdt_node;
}

kdt_tree_t *kdt_tree_create(size_t data_size)
{
	/* Alloc memory for the kdtree and check if it failed. */
	kdt_tree_t *kdt_tree = malloc(sizeof(*kdt_tree));
	DIE(!kdt_tree, "Alloc for kdt_tree failed!");

	/* Initialize data. */
	kdt_tree->root = NULL;
	kdt_tree->data_size = data_size;

	return kdt_tree;
}

void kdt_tree_insert(kdt_tree_t *kdt_tree, void *data)
{
	/* Create a new node with the given data. */
	kdt_node_t *insert_node = kdt_node_create(kdt_tree, data);

	/* Check if the kdtree is empty. */
	if (!kdt_tree->root) {
		kdt_tree->root = insert_node;
		return;
	}

	/* Iterate over all nodes with "binary search". */
	kdt_node_t *curr_node = kdt_tree->root;
	int curr_level = 0;

	while (1) {
		/* Get the coordinates of the curr_node and the insert_node. */
		int coordinate = curr_level % kdt_tree->dimensions;
		int *insert_node_coord = insert_node->coordinates;
		int *curr_node_coord = curr_node->coordinates;

		/* Compare the coordinate between them and decide where to insert. */
		if (insert_node_coord[coordinate] >= curr_node_coord[coordinate]) {
			/* Check if the right children exists. */
			if (!curr_node->right) {
				curr_node->right = insert_node;
				return;
			}

			/* Continue searching. */
			curr_node = curr_node->right;
		} else {
			/* Check if the left children exists. */
			if (!curr_node->left) {
				curr_node->left = insert_node;
				return;
			}

			/* Continue searching. */
			curr_node = curr_node->left;
		}

		/* Increase the level where we are in kdtree. */
		curr_level++;
	}
}

void kdt_node_free(kdt_node_t *kdt_node)
{
	/* Check if the node exists. */
	if (!kdt_node)
		return;

	/* Erase data from the children. */
	kdt_node_free(kdt_node->left);
	kdt_node_free(kdt_node->right);

	/* Erase all data from the node. */
	free(kdt_node->coordinates);
	free(kdt_node);
}

void kdt_tree_free(kdt_tree_t *kdt_tree)
{
	/* Erase recursively each node. */
	kdt_node_free(kdt_tree->root);
	/* Free memory of the kdtree. */
	free(kdt_tree);
	kdt_tree = NULL;
}
