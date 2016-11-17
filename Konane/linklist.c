/**
 * Reference:
 * CMPT 360 - Assignment 1 - Task 1
 * Alexander Martin (1782136)
 * Sept 27, 2016
 */

#include <stdlib.h>
#include <stdio.h>

#include "linklist.h"

/**
 * create_node() - creates a node for a linked list
 * @value - Void pointer for value of a node
 *
 * Return: struct node * holding value or NULL on failure.
 */
struct node *create_node(void *value)
{
	struct node *new = malloc(sizeof(struct node));

	if (!new) {
        fprintf(stderr, "ERROR: Memory allocation failure.");
        exit(1);
    }

	new->next = NULL;
	new->value = value;
	return new;
}

void insert_node_start(struct node **head, void *value)
{
	struct node *new = create_node(value);
	struct node *cur = *head;

	new->next = cur == NULL ? NULL : cur;
	*head = new;
}

void insert_node_end(struct node **head, void *value)
{
    struct node *new = create_node(value);

    if (*head == NULL) {
        *head = new; 
    } else {
        struct node *cur;

        for (cur = *head; cur->next != NULL; cur = cur->next);
        cur->next = new;
    }
}

void remove_node(struct node **head, void *remove, void (*free_value)(void *))
{
    if ((*head)->value == remove) {
        struct node *to_remove = *head;
        *head = (*head)->next; 
        free_node(to_remove, free_value);

    } else {
        struct node *cur;

        for (cur = *head; cur->next != NULL && cur->next->value != remove; cur = cur->next);

        // if no match
        if (cur->next == NULL)
            return;

        struct node *to_remove = cur->next;
        cur->next = cur->next->next;    
        free_node(to_remove, free_value);
    }

}

void free_node(struct node *node, void (*free_value)(void *))
{
	if (free_value != NULL)
		free_value(node->value);
	node->value = NULL;
	free(node);
	node = NULL;
}

void recursive_free_node(struct node *start, void (*free_value)(void *))
{
	struct node *current = start;

	while (current != NULL) {
		struct node *next = current->next;

		free_node(current, free_value);
		current = next;
	}
}

void *remove_at_pos(struct node **head, int pos)
{
	struct node *ret;

	if (pos == 0) {
		ret = *head;
		if (*head != NULL)
			*head = (*head)->next;	
	} else if (pos > 0) {
		struct node *prev;
		struct node *cur = *head;
		for (int i = 0; i < pos; i++) {
			if (cur == NULL) {
				ret = NULL;
				break;
			} else {
				prev = cur;
				cur = cur->next;
			}
		}

		ret = cur;
		prev->next = cur->next;
	} else {
		ret = NULL;
	}

	if (ret == NULL)
		return NULL;

	void *ret_value = ret->value;
	free_node(ret, NULL);
	return ret_value;
}
