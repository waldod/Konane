/**
 * Reference:
 * CMPT 360 - Assignment 1 - Task 1
 * Alexander Martin (1782136)
 * Sept 27, 2016
 */

#ifndef _LINKLIST_H_
#define _LINKLIST_H_

struct node {
	struct node *next;
	void *value;
};

/**
 * insert_node_start() - creates and inserts a node for a linked list at the start
 * @head - head of link list
 * @value - Void pointer for value of a node
 */
void insert_node_start(struct node **head, void *value);

/**
 * insert_node_end() - creates and inserts a node at the end of the list
 * @head - head of link list
 * @value - Void pointer for value of a node
 */
void insert_node_end(struct node **head, void *value);

/**
 * create_node() - creates a node for a linked list
 * @value - Void pointer for value of a node
 *
 * Return: struct node * holding value
 */
struct node *create_node(void *value);

/**
 * free_node() - frees memory taken for a node non-recursively
 * @node - Node to free
 * @free_value - Function that can deallocate a value of node
 *               (pass NULL if not necessary)
 */
void free_node(struct node *node, void (*free_value)(void *));

/**
 * recursive_free_node() - frees memory taken for a node recursively
 * @start - Node to start freeing from
 * @free_value - Function that can deallocate value of node
 *               (pass NULL if not necessary)
 */
void recursive_free_node(struct node *start, void (*free_value)(void *));

/**
 * remove_node() - remove node with a particular value
 * @head - double pointer to head (since it can change)
 * @remove - value to remove
 * @free_value - function to free value if necessary
 */
void remove_node(struct node **head, void *remove, void (*free_value)(void *));

/**
 * remove_at_pos() - remove and return a linklist node at a position
 * @head - double pointer to head (since it can change)
 * @pos - position to remove from
 *
 * Return: value at that at that position (or NULL if not found)
 */
void *remove_at_pos(struct node **head, int pos);

#endif /* _LINKLIST_H_ */
