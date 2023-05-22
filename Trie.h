#ifndef TRIE_H_
#define TRIE_H_

#include <errno.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

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

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	/* Value associated with key (set if end_of_word = 1). */
	void *value;

	/* Value associated if current node marks the end of a word. */
	int end_of_word;

	/* Value that stores the frequency of a word. */
	int freq;

	/* Vector of children. */
	trie_node_t **children;

	/* Number of suffixes for the current node. */
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	/* Root of the trie. */
	trie_node_t *root;

	/* Number of nodes, useful to test correctness. */
	int nodes;
};

/* Function that allocates memory for a node in the trie. */
trie_node_t *trie_create_node(void);

/* Function that allocates memory for a trie. */
trie_t *trie_create(void);

/* Function that inserts in a trie a pair <key, value>. */
void trie_insert(trie_t *trie, char *key, char *value);

/* Function that frees the memory of a node of a trie. */
void trie_node_free(trie_node_t **node);

/* Auxiliar function which returns if the */
/* <key> has suffixes and frees memory if it's the case. */
int __trie_remove(trie_t *trie, trie_node_t *curr_node, char *key);

/* Function that removes a pair <key, value> from the trie. */
void trie_remove(trie_t *trie, char *key);

/* Function that frees the memory of a trie. */
void trie_free(trie_t **trie);

#endif /* TRIE_H_ */
