#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kNN_utils.h"

#define MAX_NEIGHS 100
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
	DIE(!new_point, "Alloc for point failed!");
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

void nearest_neighbour(kdt_tree_t *tree)
{
	/* Alloc memory for the point and check if it failed. */
	int *point = malloc(tree->dimensions * tree->data_size);
	DIE(!point, "Alloc for point failed!");

	/* Read the point's coordinates. */
	for (unsigned int i = 0; i < tree->dimensions; ++i)
		scanf("%d", &point[i]);

	/* Initialize data for dfs. */
	long best_sq_dist = MAX_DIST;
	int **neighbours = malloc(MAX_NEIGHS * sizeof(int *));
	DIE(!neighbours, "Alloc for neighbours failed!");
	int nr_neigh = 0;

	/* Search the nearest neighbours. */
	search_best_dist(tree->root, 0, point, &best_sq_dist, neighbours,
					 &nr_neigh);

	/* Sort the points by their coordinates. */
	sort_points(neighbours, nr_neigh, tree->dimensions);

	/* Print all the nearest neighbours. */
	for (int i = 0; i < nr_neigh; ++i) {
		for (unsigned int j = 0; j < tree->dimensions; ++j)
			printf("%d ", neighbours[i][j]);
		printf("\n");
	}

	/* Free memory. */
	free(neighbours);
	free(point);
}

void range_search(kdt_tree_t *tree)
{
	/* Alloc memory for the intervals where to search points. */
	int *intervals = malloc(2 * tree->dimensions * tree->data_size);

	/* Check if allocation failed. */
	DIE(!intervals, "Alloc for intervals failed!");

	/* Read the intervals. */
	for (unsigned int i = 0; i < 2 * tree->dimensions; ++i)
		scanf("%d", &intervals[i]);

	/* Search the points in our intervals / range. */
	search_points(tree->root, 0, intervals);

	/* Free memory. */
	free(intervals);
}

int main(void)
{
	/* Create the representative structure for points. */
	kdt_tree_t *tree = kdt_tree_create(sizeof(int));

	/* Read operations from user until EXIT. */
	char operation[OPERATION_LEN];
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
