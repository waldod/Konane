#ifndef _TREE_H_
#define _TREE_H_

#include <pthread.h>

/**
 * TREE LIBRARY
 *
 * This library is thread safe as long as only the functions
 * provided are used for handling the tree. 
 *
 * It is safe to access child_count and value directly (assuming
 * that whatever value contains is thread safe).
 *
 * DO NOT ACCESS CHILDREN DIRECTLY!  The children array is dynamically
 * allocated and thus will be freed and reallocated.  Use get_tree_children
 * instead.
 */

struct tree_node {
	struct tree_node **children;
	int child_count;
	int depth;
	pthread_mutex_t lock;
	void *value;
};

/**
 * free_tree_node() - frees a tree node
 * @node - node to free
 * @free_value - function to free value of node (NULL if unnecessary) 
 */
void free_tree_node(struct tree_node *node, void (*free_value)(void *));

/**
 * recursive_free_tree() - frees tree starting at root proceeding through all children
 * @root - root of tree
 * @free_value - function to free value of node (NULL if unnecessary)
 * @except - Node node to free or traverse
 */
void recursive_free_tree(struct tree_node *root, void (*free_value)(void *), struct tree_node *except);

/**
 * breadth_traverse() - do a breadth-traverse of the tree
 * @root - root to start
 * @max_depth - max depth to go down to (-1 if no limit)
 * @eval - function to run once we reach a node
 */
void breadth_traverse(struct tree_node *root, int max_depth, void (*eval)(struct tree_node *));

/**
 * init_tree() - initializes the tree ADT
 * @value - value to insert into root of tree
 *
 * Returns: A tree node containing the value on success
 */
struct tree_node *init_tree(void *value);

/**
 * add_tree_child() - adds a child to a tree node 
 * @parent - parent of the new child
 * @value - value to insert into new child 
 *
 * Note: There is no way to remove a child from a tree.
 *
 * Returns: the new tree node
 */
struct tree_node *add_tree_child(struct tree_node *parent, void *child_value);


/**
 * get_tree_children() - returns a new copy of the node's children array (thread safe)
 * @parent - parent to get children from
 * @count - pointer to write child count to
 *
 * Note: Make sure to free the children array after using
 *
 * Note: Array is cloned, the children it points to are not.
 *
 * Returns: dynamically allocated array of children.
 */
struct tree_node **get_tree_children(struct tree_node *parent, int *count);

#endif /* _TREE_H_ */
