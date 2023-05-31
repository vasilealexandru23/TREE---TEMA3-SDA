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

	/* Alloc memory for the node's children. */
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
	/* Build the path for our key in the trie. */
	while (strlen(key) != 0) {
		/* Get the first letter of our current word. */
		int first_letter = key[0] - 'a';

		/* Create the node if it doesn't exists. */
		if (!curr_node->children[first_letter]) {
			curr_node->children[first_letter] = trie_create_node();
			curr_node->n_children++;
			trie->nodes++;
		}

		/* Go to the next node. */
		curr_node = curr_node->children[first_letter];
		key = key + 1;
	}

	/* Check if we have already inserted the word. */
	if (!curr_node->freq) {
		/* Check if current node's value is not allocated. */
		if (!curr_node->value) {
			curr_node->value = malloc(strlen(value) + 1);
			DIE(!curr_node->value, "Alloc for curr_node->value failed!");
		}

		/* Copy the value in the current node. */
		memcpy(curr_node->value, value, strlen(value) + 1);

		/* Current node marks the end of our inserted word. */
		curr_node->end_of_word = 1;
	}

	/* Increase the word's frequency. */
	curr_node->freq++;
}

void trie_node_free(trie_node_t **node)
{
	/* Check if the node exists. */
	if ((*node) == NULL)
		return;

	/* Erase all of the node's children. */
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		if ((*node)->children[i])
			trie_node_free(&(*node)->children[i]);

	/* Erase data. */
	free((*node)->children);
	(*node)->children = NULL;
	free((*node)->value);
	(*node)->value = NULL;
	free((*node));
	*node = NULL;
}

int __trie_remove(trie_t *trie, trie_node_t *curr_node, char *key)
{
	/* Check if we iterated over all chars in our key. */
	if (strlen(key) == 0) {
		if (curr_node->end_of_word == 1) {
			curr_node->end_of_word = 0;
			curr_node->freq = 0;
			return (curr_node->n_children == 0);
		}
		return 0;
	}

	/* Get the next node. */
	int first_letter = key[0] - 'a';
	trie_node_t *next_node = curr_node->children[first_letter];

	/* Check to remove the next node. */
	if (next_node && __trie_remove(trie, next_node, key + 1) == 1) {
		/* Erase the corresponding child and update data. */
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
	/* Use helper function to do it recursively. */
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
