#ifndef MK_UTILS_H_
#define MK_UTILS_H_
#include "Trie.h"

/* Function that returns the */
/* lexicographically smallest word. */
char *smallest_lexic(trie_node_t *curr_node);

/* Function that gets the shortest word. */
void shortest_word(trie_node_t *curr_node, char **ans);

/* Function that gets the most frequent word. */
void most_freq_word(trie_node_t *curr_node, char **ans, int *best_freq);

#endif /* MK_UTILS_H_ */
