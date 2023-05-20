#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "k-dtree.h"

#define OPERATION_LEN 100
#define MAX_FILE_NAME 100
#define MAX_DIST 100000000

void load_file(char *file_name, bst_tree_t *tree)
{
	/* Open the file and check if failed. */
	FILE *fin;
	fin = fopen(file_name, "rt");
	DIE(!fin, "Failed to load the file\n");

	int n, k;
	fscanf(fin, "%d %d", &n, &k);
	int *new_point = malloc(k * sizeof(int));
	tree->dimensions = k;
	while (n--) {
		for (unsigned int i = 0; i < tree->dimensions; ++i)
			fscanf(fin, "%d", &new_point[i]);
		bst_tree_insert(tree, new_point);
	}

	free(new_point);
	fclose(fin);
}

void search_best_dist(bst_node_t *curr_node, int level, int *point,
					  long *best_sq_dist)
{
	if (!curr_node)
		return;

	int *curr_point = curr_node->coordinates;

	/* Compute distance beetween <source, destination> point. */
	long dist = 0;
	for (unsigned int i = 0; i < curr_node->dim; ++i)
		dist += 1LL * (point[i] - curr_point[i]) * (point[i] - curr_point[i]);

	if (dist < *best_sq_dist)
		*best_sq_dist = dist;

	int next_node = -1; /* 0->left, 1->right. */
	unsigned int index = level % curr_node->dim;
	if (point[index] < curr_point[index]) {
		search_best_dist(curr_node->left, level + 1, point, best_sq_dist);
		next_node = 0;
	} else {
		search_best_dist(curr_node->right, level + 1, point, best_sq_dist);
		next_node = 1;
	}

	long diff = (curr_point[index] - point[index]) *
			(curr_point[index] - point[index]);
	if (diff < *best_sq_dist) {
		if (next_node == 0)
			search_best_dist(curr_node->right, level + 1, point, best_sq_dist);
		else
			search_best_dist(curr_node->left, level + 1, point, best_sq_dist);
	}
}

void print_nearest_points(bst_node_t *curr_node, int level, int *point,
						  long *best_sq_dist, int minimal_dist)
{
	if (!curr_node)
		return;

	int *curr_point = curr_node->coordinates;

	/* Compute distance beetween <source, destination> point. */
	long dist = 0;
	for (unsigned int i = 0; i < curr_node->dim; ++i)
		dist += 1LL * (point[i] - curr_point[i]) * (point[i] - curr_point[i]);

	if (dist < *best_sq_dist)
		*best_sq_dist = dist;

	if (dist == minimal_dist) {
		for (unsigned int i = 0; i < curr_node->dim; ++i)
			printf("%d ", curr_point[i]);

		printf("\n");
	}

	int next_node = -1; /* 0->left, 1->right. */
	unsigned int index = level % curr_node->dim;
	if (point[index] < curr_point[index]) {
		print_nearest_points(curr_node->left, level + 1, point,
							 best_sq_dist, minimal_dist);
		next_node = 0;
	} else {
		print_nearest_points(curr_node->right, level + 1, point,
							 best_sq_dist, minimal_dist);
		next_node = 1;
	}

	long diff = (curr_point[index] - point[index]) *
			(curr_point[index] - point[index]);

	if (diff < *best_sq_dist) {
		if (next_node == 0)
			print_nearest_points(curr_node->right, level + 1, point,
								 best_sq_dist, minimal_dist);
		else
			print_nearest_points(curr_node->left, level + 1, point,
								 best_sq_dist, minimal_dist);
	}
}

void nearest_neighbour(bst_tree_t *tree)
{
	int *point = malloc(tree->dimensions * tree->data_size);

	for (unsigned int i = 0; i < tree->dimensions; ++i)
		scanf("%d", &point[i]);

	long best_sq_dist = MAX_DIST;
	search_best_dist(tree->root, 0, point, &best_sq_dist);
	long curr_best_dist = MAX_DIST;
	print_nearest_points(tree->root, 0, point, &curr_best_dist,
						 best_sq_dist);

	free(point);
}

void search_points(bst_node_t *curr_node, int *intervals)
{
	if (!curr_node)
		return;

	int *curr_point = curr_node->coordinates;

	int works = 1;
	for (unsigned int i = 0; i < curr_node->dim; ++i)
		if (!(curr_point[i] >= intervals[2 * i] &&
			  curr_point[i] <= intervals[2 * i + 1]))
			works++;

	search_points(curr_node->left, intervals);
	search_points(curr_node->right, intervals);
	if (works == 1) {
		for (unsigned int i = 0; i < curr_node->dim; ++i)
			printf("%d ", curr_point[i]);
		printf("\n");
	}
}

void range_search(bst_tree_t *tree)
{
	int *intervals = malloc(2 * tree->dimensions * tree->data_size);
	for (unsigned int i = 0; i < 2 * tree->dimensions; ++i)
		scanf("%d", &intervals[i]);
	search_points(tree->root, intervals);
	free(intervals);
}

int main(void)
{
	char operation[OPERATION_LEN];
	bst_tree_t *tree = bst_tree_create(sizeof(int));
	while (1) {
		scanf("%s", operation);
		if (!strcmp(operation, "LOAD")) {
			char file_name[MAX_FILE_NAME];
			scanf("%s", file_name);
			load_file(file_name, tree);
		} else if (!strcmp(operation, "NN")) {
			nearest_neighbour(tree);
		} else if (!strcmp(operation, "RS")) {
			range_search(tree);
		} else if (!strcmp(operation, "EXIT")) {
			bst_tree_free(tree);
			break;
		}
	}

	return 0;
}
