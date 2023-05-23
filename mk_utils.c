#include <stdlib.h>
#include <string.h>
#include "mk_utils.h"

char *smallest_lexic(trie_node_t *curr_node)
{
	/* Check if we found a word in the dictionary. */
	if (curr_node->end_of_word)
		return curr_node->value;

	/* Go to first child. */
	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i)
		if (curr_node->children[i])
			return smallest_lexic(curr_node->children[i]);

	return NULL;
}

void shortest_word(trie_node_t *curr_node, char **ans)
{
	/* Check if we found a word in the dictionary. */
	if (curr_node->end_of_word) {
		if (!(*ans) || strlen((*ans)) > strlen((char *)curr_node->value))
			(*ans) = curr_node->value;
		return;
	}

	/* Continue searching. */
	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i)
		if (curr_node->children[i])
			shortest_word(curr_node->children[i], ans);
}

void most_freq_word(trie_node_t *curr_node, char **ans, int *best_freq)
{
	/* Check if we found a word in the dictionary. */
	if (curr_node->end_of_word) {
		if (!(*ans) || curr_node->freq > (*best_freq)) {
			(*ans) = curr_node->value;
			(*best_freq) = curr_node->freq;
		} else if (curr_node->freq == (*best_freq) &&
			   strcmp(curr_node->value, (*ans)) < 0) {
			(*ans) = curr_node->value;
		}
	}

	/* Continue searching. */
	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i)
		if (curr_node->children[i])
			most_freq_word(curr_node->children[i], ans, best_freq);
}
