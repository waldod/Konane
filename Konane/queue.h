#ifndef _QUEUE_H_
#define _QUEUE_H_

struct queue {
	struct node *head;
};

/**
 * init_queue() - initializes a queue
 * 
 * Returns an empty queue
 */
struct queue *init_queue();

/**
 * enqueue() - enqueues a new value to the queue
 * @queue - queue to add to
 * @value - value to add
 */
void enqueue(struct queue *queue, void *value);

/**
 * dequeue() - dequeues a queue
 * @queue - queue to dequeue
 *
 * Returns: value of node dequeued
 */
void *dequeue(struct queue *queue);

/**
 * free_queue() - frees a queue
 * @queue - queue to free
 * @free_value - function to free value of each queue node
 *
 */
void free_queue(struct queue *queue, void (*free_value)(void *));

#endif /* _QUEUE_H_ */
