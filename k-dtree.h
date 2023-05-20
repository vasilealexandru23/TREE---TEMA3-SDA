#ifndef K_DTREE_H_
#define K_DTREE_H_
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

typedef struct bst_node_t bst_node_t;
struct bst_node_t {
	/* left child */
	bst_node_t *left;

	/* right child */
	bst_node_t *right;

	/* Dimension of the vector space (R^k). */
	unsigned int dim;

	/* The coordinates of the node(point). */
	void *coordinates;
};

typedef struct bst_tree_t bst_tree_t;
struct bst_tree_t {
	/* root of the tree */
	bst_node_t  *root;

	 /* Type of coordinates for a point. */
	size_t data_size;

	/* Number of coordinates for a point. */
	unsigned int dimensions;
};

bst_node_t *__bst_node_create(bst_tree_t *bst_tree, void *data);

bst_tree_t *bst_tree_create(size_t data_size);

void bst_tree_insert(bst_tree_t *bst_tree, void *data);

void __bst_tree_free(bst_node_t *bst_node);

void bst_tree_free(bst_tree_t *bst_tree);

#endif /* K_DTREE_H_ */
