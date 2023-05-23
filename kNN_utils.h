#ifndef KNN_UTILS_H_
#define KNN_UTILS_H_
#define TRIE_H_
#include "k-dtree.h"

long euclidean_sq_dist(int *source, int *destination, unsigned int dim);

void search_best_dist(kdt_node_t *curr_node, int level, int *point,
					  long *best_sq_dist, int **neighbors, int *nr_neigh);

void swap(int **neigh1, int **neigh2);

void sort_points(int **neighbours, int nr_neighbours, int dimension);

void search_points(kdt_node_t *curr_node, int level, int *intervals);

#endif /* KNN_UTILS_H_ */
