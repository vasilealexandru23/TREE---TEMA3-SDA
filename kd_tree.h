#ifndef KD_TREE_H_
#define KD_TREE_H_
#include <stdlib.h>
#include <errno.h>

/* useful macro for handling error codes */
#define DIE(assertion, call_description)				\
	do {												\
		if (assertion) {								\
			fprintf(stderr, "(%s, %d): ",				\
					__FILE__, __LINE__);				\
			perror(call_description);					\
			exit(errno);								\
		}												\
	} while (0)

typedef struct kdt_node_t kdt_node_t;
struct kdt_node_t {
	/* left child */
	kdt_node_t *left;

	/* right child */
	kdt_node_t *right;

	/* Dimension of the vector space (R^n). */
	unsigned int dim;

	/* The coordinates of the node(point). */
	void *coordinates;
};

typedef struct kdt_tree_t kdt_tree_t;
struct kdt_tree_t {
	/* root of the tree */
	kdt_node_t  *root;

	 /* Type of coordinates for a point. */
	size_t data_size;

	/* Number of coordinates for a point. */
	unsigned int dimensions;
};

/* Function that creates a node for the kdtree. */
kdt_node_t *kdt_node_create(kdt_tree_t *kdt_tree, void *data);

/* Function that creates the kdtree. */
kdt_tree_t *kdt_tree_create(size_t data_size);

/* Function that inserts data in the kdtree. */
void kdt_tree_insert(kdt_tree_t *kdt_tree, void *data);

/* Function that frees the memory of a node. */
void kdt_node_free(kdt_node_t *kdt_node);

/* Function that frees the memory of a kdtree. */
void kdt_tree_free(kdt_tree_t *kdt_tree);

#endif /* KD_TREE_H_ */
