#ifndef MK_UTILS_H_
#define MK_UTILS_H_
#include "Trie.h"

char *smallest_lexic(trie_node_t *curr_node);

void shortest_word(trie_node_t *curr_node, char **ans);

void most_freq_word(trie_node_t *curr_node, char **ans, int *best_freq);

#endif /* MK_UTILS_H_ */
