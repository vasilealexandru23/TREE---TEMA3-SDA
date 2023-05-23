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
	int *point = malloc(tree->dimensions * tree->data_size);

	for (unsigned int i = 0; i < tree->dimensions; ++i)
		scanf("%d", &point[i]);

	long best_sq_dist = MAX_DIST;
	int **neighbours = malloc(MAX_NEIGHS * sizeof(int *));
	int nr_neigh = 0;
	search_best_dist(tree->root, 0, point, &best_sq_dist, neighbours,
					 &nr_neigh);
	sort_points(neighbours, nr_neigh, tree->dimensions);
	for (int i = 0; i < nr_neigh; ++i) {
		for (int j = 0; j < tree->dimensions; ++j)
			printf("%d ", neighbours[i][j]);
		printf("\n");
	}

	free(neighbours);
	free(point);
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
