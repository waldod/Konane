#include "queue.h"
#include "linklist.h"

#include <stdio.h>
#include <stdlib.h>

struct queue *init_queue()
{
	struct queue *q = malloc(sizeof(struct queue));

	if (q == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failure.\n");
		exit(1);
	}

	q->head = NULL;
	return q;
}

void enqueue(struct queue *queue, void *value)
{
	insert_node_end(&queue->head, value);
}

void *dequeue(struct queue *queue)
{
	return remove_at_pos(&queue->head, 0);
}

void free_queue(struct queue *queue, void (*free_value)(void *))
{
	recursive_free_node(queue->head, free_value);
	free(queue);
}
