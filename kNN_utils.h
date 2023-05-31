#ifndef KNN_UTILS_H_
#define KNN_UTILS_H_
#define TRIE_H_
#include "kd_tree.h"

/* Function that returns the square of the euclidean distance */
/* between <source, destination> with the given vector space dimension. */
long euclidean_sq_dist(int *source, int *destination, unsigned int dim);

/* Function that computes the square of the shortest distance */
/* beetween a given point(source), and other nodes in the kdtree. */
void search_best_dist(kdt_node_t *curr_node, int level, int *point,
					  long *best_sq_dist, int **neighbors, int *nr_neigh);

/* Function that swaps two points. */
void swap_points(int **point1, int **point2);

/* Function that sorts the found */
/* points in the plane by their coordinates. */
void sort_points(int **neighbours, int nr_neighbours, int dimension);

/* Function that prints the points included in the given interval(s). */
void search_points(kdt_node_t *curr_node, int level, int *intervals);

#endif /* KNN_UTILS_H_ */
