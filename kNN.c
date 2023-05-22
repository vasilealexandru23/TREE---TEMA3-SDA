#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "k-dtree.h"

#define OPERATION_LEN 100
#define MAX_FILE_NAME 100
#define MAX_DIST 100000000

void load_file(char *file_name, kdt_tree_t *tree)
{
	/* Open the file and check if failed. */
	FILE *fin;
	fin = fopen(file_name, "rt");
	DIE(!fin, "Failed to load the file\n");

	/* Read the number of vectors(points) and their dimension. */
	int points, dim;
	fscanf(fin, "%d %d", &points, &dim);
	int *new_point = malloc(dim * sizeof(int));
	tree->dimensions = dim;

	/* Read and insert every point in the tree. */
	while (points--) {
		for (unsigned int i = 0; i < tree->dimensions; ++i)
			fscanf(fin, "%d", &new_point[i]);
		kdt_tree_insert(tree, new_point);
	}

	/* Free the memory for aux variables and close the file. */
	free(new_point);
	fclose(fin);
}

long euclidean_sq_dist(int *source, int *destination, unsigned int dim)
{
	long dist = 0;
	for (unsigned int i = 0; i < dim; ++i)
		dist += 1LL * (source[i] - destination[i]) *
			(source[i] - destination[i]);
	return dist;
}

void search_best_dist(kdt_node_t *curr_node, int level, int *point,
					  long *best_sq_dist)
{
	if (!curr_node)
		return;

	int *curr_point = curr_node->coordinates;

	/* Compute distance beetween <source, destination> point. */
	long dist = euclidean_sq_dist(point, curr_point, curr_node->dim);

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

void print_nearest_points(kdt_node_t *curr_node, int level, int *point,
						  long *best_sq_dist, int minimal_dist)
{
	/* 97032 */
	if (!curr_node)
		return;

	int *curr_point = curr_node->coordinates;

	/* Compute distance beetween <source, destination> point. */
	long dist = euclidean_sq_dist(point, curr_point, curr_node->dim);

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

void nearest_neighbour(kdt_tree_t *tree)
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

void search_points(kdt_node_t *curr_node, int level, int *intervals)
{
	if (!curr_node)
		return;

	int *curr_point = curr_node->coordinates;

	int works = 1;
	for (unsigned int i = 0; i < curr_node->dim; ++i)
		if (!(curr_point[i] >= intervals[2 * i] &&
			  curr_point[i] <= intervals[2 * i + 1]))
			works++;

	int index = level % curr_node->dim;

	if (curr_point[index] >= intervals[2 * index + 1]) {
		search_points(curr_node->left, level + 1, intervals);
	} else if (curr_point[index] <= intervals[2 * index]) {
		search_points(curr_node->right, level + 1, intervals);
	} else if (curr_point[index] >= intervals[2 * index] &&
		   curr_point[index] <= intervals[2 * index + 1]) {
		search_points(curr_node->right, level + 1, intervals);
		search_points(curr_node->left, level + 1, intervals);
	}

	if (works == 1) {
		for (unsigned int i = 0; i < curr_node->dim; ++i)
			printf("%d ", curr_point[i]);
		printf("\n");
	}
}

void range_search(kdt_tree_t *tree)
{
	int *intervals = malloc(2 * tree->dimensions * tree->data_size);
	for (unsigned int i = 0; i < 2 * tree->dimensions; ++i)
		scanf("%d", &intervals[i]);
	search_points(tree->root, 0, intervals);
	free(intervals);
}

int main(void)
{
	char operation[OPERATION_LEN];
	kdt_tree_t *tree = kdt_tree_create(sizeof(int));
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
			kdt_tree_free(tree);
			break;
		}
	}

	return 0;
}
