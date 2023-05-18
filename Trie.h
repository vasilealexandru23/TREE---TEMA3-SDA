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
    /* Value associated with key (set if end_of_word = 1) */
    void* value;

    /* 1 if current node marks the end of a word, 0 otherwise */
    int end_of_word;

    /* Keep the frequency of a word. */
    int freq;

    trie_node_t** children;
    int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
    trie_node_t* root;

    /* Callback to free value associated with key, should be called when freeing */
    void (*free_value_cb)(void*);

    /* Optional - number of nodes, useful to test correctness */
    int nNodes;
};


trie_node_t* trie_create_node();

trie_t* trie_create();

void trie_insert(trie_t* trie, char* key, char* value);

void trie_node_free(trie_node_t **node);

int __trie_remove(trie_t *trie, trie_node_t *curr_node, char *key);

void trie_remove(trie_t* trie, char* key);

void trie_free(trie_t** pTrie);

#endif /* TRIE_H_ */