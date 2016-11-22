#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "tree.h"
#include "queue.h"

static struct tree_node *create_tree_node(void *value)
{
	struct tree_node *node = malloc(sizeof(struct tree_node));

	if (node == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failure.\n");
	}

	pthread_mutex_init(&(node->lock), NULL);
	node->child_count = 0;
	node->children = NULL;
	node->value = value;
}

static void _lock(struct tree_node *node)
{
	pthread_mutex_lock(&(node->lock));
}

static void _unlock(struct tree_node *node)
{
	pthread_mutex_unlock(&(node->lock));
}

void free_tree_node(struct tree_node *node, void (*free_value)(void *))
{
	if (free_value != NULL)
		free_value(node->value);
	free(node->children);
	free(node);
}

void recursive_free_tree(struct tree_node *root, void (*free_value)(void *), struct tree_node *except)
{
	if (root == except)
		return;

	if (free_value != NULL)
		free_value(root);	

	for (int i=0; i<root->child_count; i++) {
		recursive_free_tree(root->children[i], free_value, except);	
	}

	if (root->children != NULL)
		free(root->children);

	free(root);
}

struct tree_node *init_tree(void *value)
{
	struct tree_node *root = create_tree_node(value);
	root->depth = 0;
	return root;
}

void breadth_traverse(struct tree_node *root, int max_depth, void (*eval)(struct tree_node  *))
{
	struct queue *to_see = init_queue();
	struct tree_node *cur;

	int start_depth = root->depth;

	enqueue(to_see, root);

	while ((cur = dequeue(to_see)) != NULL) {
		
		if (max_depth == -1 || cur->depth - start_depth < max_depth) {
			eval(cur);
			int child_count = 0;

			struct tree_node **children = get_tree_children(cur, &child_count);

			for (int i=0; i<child_count; i++) {
				enqueue(to_see, children[i]);
			}

			free(children);
		}
	}
	
	free_queue(to_see, NULL);
}

struct tree_node *add_tree_child(struct tree_node *parent, void *child_value)
{
	_lock(parent);

	struct tree_node *child = create_tree_node(child_value);
	child->depth = parent->depth + 1;

	parent->child_count++;
	if (parent->children == NULL) {
		parent->children = malloc(sizeof(struct tree_node *) * parent->child_count);
		
	} else {
		parent->children = realloc(parent->children, sizeof(struct tree_node *) * parent->child_count);
	}

	if (parent->children == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failure.\n");
		exit(1);
	}

	parent->children[parent->child_count - 1] = child;

	_unlock(parent);

	return child;
}

struct tree_node **get_tree_children(struct tree_node *parent, int *count)
{
	_lock(parent);

	size_t size_of_array = sizeof(struct tree_node *) * parent->child_count;
	struct tree_node **cloned_children = malloc(size_of_array);

	if (cloned_children == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failure.\n");
		exit(1);
	}

	memcpy(cloned_children, parent->children, size_of_array);
	*count = parent->child_count;

	_unlock(parent);

	return cloned_children;
}
