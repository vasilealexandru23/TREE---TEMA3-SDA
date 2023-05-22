#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trie.h"

#define OPERATION_LEN 100
#define MAX_WORD 100
#define MAX_FILE_NAME 100

void insert(char *word, trie_t *words_history)
{
	/* Alloc memory for a copy of the current word. */
	char *copy_word = malloc(strlen(word) + 1);
	DIE(!copy_word, "Alloc for copy of word failed!");
	strcpy(copy_word, word);

	/* Add in the user's history the word. */
	trie_insert(words_history, word, copy_word);

	/* Free memory allocated for the copy. */
	free(copy_word);
}

void remove_word(char *word, trie_t *words_history)
{
	/* Remove the word from the user's history. */
	trie_remove(words_history, word);
}

void load_file(char *file_name, trie_t *words_history)
{
	/* Open the file and check if it failed. */
	FILE *fin = fopen(file_name, "rt");
	DIE(!fin, "Failed to load the file\n");

	/* Read every word from file and insert in the user's history. */
	char word[MAX_WORD];
	while (fscanf(fin, "%s", word) != EOF)
		insert(word, words_history);

	/* Close the file. */
	fclose(fin);
}

void autocorrect(trie_node_t *curr_node, char *word, int k, int *words_found)
{
	if (!strlen(word)) {
		if (curr_node && curr_node->end_of_word) {
			printf("%s\n", (char *)curr_node->value);
			(*words_found)++;
		}
		return;
	}

	unsigned int first_letter = word[0] - 'a';

	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i) {
		if (curr_node->children[i]) {
			if (i == first_letter) {
				autocorrect(curr_node->children[i], word + 1, k,
							words_found);
			} else if (k) {
				autocorrect(curr_node->children[i], word + 1,
							k - 1, words_found);
			}
		}
	}
}

char *smallest_lexic(trie_node_t *curr_node)
{
	/* Check if we found a word in the dictionary. */
	if (curr_node->end_of_word)
		return curr_node->value;

	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i)
		if (curr_node->children[i])
			return smallest_lexic(curr_node->children[i]);

	return NULL;
}

void shortest_word(trie_node_t *curr_node, char **ans)
{
	if (curr_node->end_of_word) {
		if (!(*ans) || strlen((*ans)) > strlen((char *)curr_node->value))
			(*ans) = curr_node->value;
		return;
	}

	/* RUN dfs. */
	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i)
		if (curr_node->children[i])
			shortest_word(curr_node->children[i], ans);
}

void most_freq_word(trie_node_t *curr_node, char **ans, int *best_freq)
{
	if (curr_node->end_of_word) {
		if (!(*ans) || curr_node->freq > (*best_freq)) {
			(*ans) = curr_node->value;
			(*best_freq) = curr_node->freq;
		} else if (curr_node->freq == (*best_freq) &&
			   strcmp(curr_node->value, (*ans)) < 0) {
			(*ans) = curr_node->value;
		}
	}

	/* RUN dfs. */
	for (unsigned int i = 0; i < ALPHABET_SIZE; ++i)
		if (curr_node->children[i])
			most_freq_word(curr_node->children[i], ans, best_freq);
}

void autocomplete(trie_t *trie, char *prefix, int crit)
{
	trie_node_t *curr_node = trie->root;

	/* Get the node that stores the given prefix. */
	while (strlen(prefix) != 0) {
		int first_letter = prefix[0] - 'a';
		if (!curr_node->children[first_letter]) {
			printf("No words found\n");
			if (!crit) {
				printf("No words found\n");
				printf("No words found\n");
			}
			return;
		}
		curr_node = curr_node->children[first_letter];
		prefix = prefix + 1;
	}

	if (crit == 1 || crit == 0) {
		char *ans = smallest_lexic(curr_node);
		if (ans)
			printf("%s\n", ans);
		else
			printf("No words found\n");
	}
	if (crit == 2 || crit == 0) {
		char *ans = NULL;
		shortest_word(curr_node, &ans);
		if (ans)
			printf("%s\n", ans);
		else
			printf("No words found\n");
	}
	if (crit == 3 || crit == 0) {
		char *ans = NULL;
		int best_freq = -1;
		most_freq_word(curr_node, &ans, &best_freq);
		if (ans)
			printf("%s\n", ans);
		else
			printf("No words found\n");
	}
}

int main(void)
{
	char operation[OPERATION_LEN];
	trie_t *words_history = trie_create();

	while (1) {
		scanf("%s", operation);
		if (!strcmp(operation, "LOAD")) {
			char file_name[MAX_FILE_NAME];
			scanf("%s", file_name);
			load_file(file_name, words_history);
		} else if (!strcmp(operation, "INSERT")) {
			char word[MAX_WORD];
			scanf("%s", word);
			insert(word, words_history);
		} else if (!strcmp(operation, "REMOVE")) {
			char word[MAX_WORD];
			scanf("%s", word);
			remove_word(word, words_history);
		} else if (!strcmp(operation, "AUTOCORRECT")) {
			char word[MAX_WORD];
			int k;
			scanf("%s %d", word, &k);
			int words_found = 0;
			autocorrect(words_history->root, word, k, &words_found);
			if (!words_found)
				printf("No words found\n");
		} else if (!strcmp(operation, "AUTOCOMPLETE")) {
			char word[MAX_WORD];
			int crit;
			scanf("%s %d", word, &crit);
			autocomplete(words_history, word, crit);
		} else if (!strcmp(operation, "EXIT")) {
			trie_free(&words_history);
			break;
		}
	}

	return 0;
}
