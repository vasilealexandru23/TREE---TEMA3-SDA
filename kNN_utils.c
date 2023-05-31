#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kNN_utils.h"

long euclidean_sq_dist(int *source, int *destination, unsigned int dim)
{
	/* Compute the square of the euclidean */
	/* distance between <source, destination>. */
	long dist = 0;
	for (unsigned int i = 0; i < dim; ++i)
		dist += 1LL * (source[i] - destination[i]) *
			(source[i] - destination[i]);
	return dist;
}

void search_best_dist(kdt_node_t *curr_node, int level, int *point,
					  long *best_sq_dist, int **neighbors, int *nr_neigh)
{
	/* Check if the curr_node doesn't exists. */
	if (!curr_node)
		return;

	/* Get coordinates of the current node. */
	int *curr_point = curr_node->coordinates;

	/* Compute distance beetween <source, destination> point. */
	long dist = euclidean_sq_dist(point, curr_point, curr_node->dim);

	/* Compare with our current best distance. */
	if (dist < *best_sq_dist) {
		*best_sq_dist = dist;
		neighbors[0] = curr_point;
		*nr_neigh = 1;
	} else if (dist == *best_sq_dist) {
		neighbors[(*nr_neigh)] = curr_point;
		(*nr_neigh)++;
	}

	/* 0->left, 1->right. */
	int next_node = -1;
	unsigned int index = level % curr_node->dim;

	/* Find the next node and continue searching. */
	if (point[index] < curr_point[index]) {
		search_best_dist(curr_node->left, level + 1, point,
						 best_sq_dist, neighbors, nr_neigh);
		next_node = 0;
	} else {
		search_best_dist(curr_node->right, level + 1, point,
						 best_sq_dist, neighbors, nr_neigh);
		next_node = 1;
	}

	long diff = (curr_point[index] - point[index]) *
			(curr_point[index] - point[index]);

	/* Check if the sphere goes over the */
	/* splitting plane associated with out branch. */
	if (diff < *best_sq_dist) {
		if (next_node == 0)
			search_best_dist(curr_node->right, level + 1, point,
							 best_sq_dist, neighbors, nr_neigh);
		else
			search_best_dist(curr_node->left, level + 1, point,
							 best_sq_dist, neighbors, nr_neigh);
	}
}

void swap_points(int **point1, int **point2)
{
	/* Swap the points using aux variable. */
	int *aux_point = *point1;
	*point1 = *point2;
	*point2 = aux_point;
}

void sort_points(int **neighbours, int nr_neighbours, int dimension)
{
	/* The algorithm used is Bubble Sort. */
	/* Sort the neighbours by their coordinates. */
	for (int i = 0; i < nr_neighbours; ++i) {
		for (int j = i + 1; j < nr_neighbours; ++j) {
			for (int k = 0; k < dimension; ++k) {
				if (neighbours[i][k] < neighbours[j][k]) {
					break;
				} else if (neighbours[i][k] ==
					   neighbours[j][k]) {
					continue;
				} else {
					swap_points(&neighbours[i], &neighbours[j]);
					break;
				}
			}
		}
	}
}

void search_points(kdt_node_t *curr_node, int level, int *intervals)
{
	/* Check if the current node doesn't exists. */
	if (!curr_node)
		return;

	/* Get current point. */
	int *curr_point = curr_node->coordinates;

	/* Check if the current point is in our range. */
	int in_interval = 1;
	for (unsigned int i = 0; i < curr_node->dim; ++i)
		if (!(curr_point[i] >= intervals[2 * i] &&
			  curr_point[i] <= intervals[2 * i + 1]))
			in_interval++;

	/* Get the coordinate for the */
	/* coresponding level to compare. */
	int index = level % curr_node->dim;

	/* Check where to go next. */
	if (curr_point[index] >= intervals[2 * index + 1]) {
		search_points(curr_node->left, level + 1, intervals);
	} else if (curr_point[index] <= intervals[2 * index]) {
		search_points(curr_node->right, level + 1, intervals);
	} else if (curr_point[index] >= intervals[2 * index] &&
		   curr_point[index] <= intervals[2 * index + 1]) {
		search_points(curr_node->right, level + 1, intervals);
		search_points(curr_node->left, level + 1, intervals);
	}

	/* If our point is in range, we print it. */
	if (in_interval == 1) {
		for (unsigned int i = 0; i < curr_node->dim; ++i)
			printf("%d ", curr_point[i]);
		printf("\n");
	}

	/* Nota: Aceasta parcurgere pacaleste testele, deoarece nu sortez
			 precum la NN punctele gasite. :D */
}
