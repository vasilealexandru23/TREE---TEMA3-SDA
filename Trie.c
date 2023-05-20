#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Trie.h"

trie_node_t *trie_create_node(void)
{
	/* Alloc memory for the new node. */
	trie_node_t *new_node = malloc(sizeof(trie_node_t));
	DIE(!new_node, "Malloc for new_node failed!");

	/* Initialize data in the node. */
	new_node->value = NULL;
	new_node->freq = 0;
	new_node->end_of_word = 0;
	new_node->n_children = 0;

	/* Alloc memory for the children nodes. */
	new_node->children = calloc(ALPHABET_SIZE, sizeof(trie_node_t *));
	DIE(!new_node->children, "Calloc for the children failed!");

	return new_node;
}

trie_t *trie_create(void)
{
	/* Alloc memory for the trie. */
	trie_t *trie = malloc(sizeof(trie_t));
	DIE(!trie, "Malloc for the trie failed!");

	/* Initialize data. */
	trie->root = trie_create_node();
	trie->nodes = 0;

	return trie;
}

void trie_insert(trie_t *trie, char *key, char *value)
{
	trie_node_t *curr_node = trie->root;
	while (strlen(key) != 0) {
		int first_letter = key[0] - 'a';
		if (!curr_node->children[first_letter]) {
			curr_node->children[first_letter] = trie_create_node();
			curr_node->n_children++;
			trie->nodes++;
		}
		curr_node = curr_node->children[first_letter];
		key = key + 1;
	}
	if (!curr_node->freq) {
		curr_node->value = malloc(strlen(value) + 1);
		memcpy(curr_node->value, value, strlen(value) + 1);
		curr_node->end_of_word = 1;
	}
	curr_node->freq++;
}

void trie_node_free(trie_node_t **node)
{
	if ((*node) == NULL)
		return;
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if ((*node)->children[i])
			trie_node_free(&(*node)->children[i]);
	free((*node)->children);
	if ((*node)->value)
		free((*node)->value);
	free((*node));
	*node = NULL;
}

int __trie_remove(trie_t *trie, trie_node_t *curr_node, char *key)
{
	if (strlen(key) == 0) {
		if (curr_node->end_of_word == 1) {
			curr_node->end_of_word = 0;
			return (curr_node->n_children == 0);
		}
		return 0;
	}

	int first_letter = key[0] - 'a';
	trie_node_t *next_node = curr_node->children[first_letter];
	if (next_node && __trie_remove(trie, next_node, key + 1) == 1) {
		trie_node_free(&curr_node->children[first_letter]);
		curr_node->n_children--;
		trie->nodes--;
		if (curr_node->n_children == 0 && curr_node->end_of_word == 0)
			return 1;
	}
	return 0;
}

void trie_remove(trie_t *trie, char *key)
{
	__trie_remove(trie, trie->root, key);
}

void trie_free(trie_t **trie)
{
	/* Free memory for every */
	/* node, starting with root. */
	trie_node_free(&((*trie)->root));
	free((*trie));
	*trie = NULL;
}
