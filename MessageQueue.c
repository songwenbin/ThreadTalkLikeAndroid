#include "MessageQueue.h"

struct MessageQueue *NewMessageQueue() {
	struct MessageQueue * queue = (struct MessageQueue *)malloc(sizeof(struct MessageQueue));
	if (queue == NULL) {
		fprintf(stderr, "allocated queue error!\n");
		return NULL;
	}

	queue->msgQueueHead = NULL;
	queue->msgQueueTail = NULL;
	
	pthread_mutex_init(&queue->queueMutex, NULL);

	return queue;
}

void DeleteMessageQueue(struct MessageQueue *queue) {
	if (queue == NULL) {
		return;
	}

	pthread_mutex_destroy(&queue->queueMutex);
	free(queue);
}

void EnqueueMessage(struct MessageQueue *queue, struct Message *m) {
	pthread_mutex_lock(&queue->queueMutex);

	if (queue->msgQueueHead == NULL) {
		queue->msgQueueHead = m;
		queue->msgQueueTail = m;
	} else {
		queue->msgQueueTail->next = m;
		queue->msgQueueTail = m;
	}

	pthread_mutex_unlock(&queue->queueMutex);
}

struct Message *DequeueMessage(struct MessageQueue *queue) {
	pthread_mutex_lock(&queue->queueMutex);

	if (queue->msgQueueTail == NULL ) {
		pthread_mutex_unlock(&queue->queueMutex);
		return NULL;
	}

	struct Message *m = queue->msgQueueHead;
	queue->msgQueueHead = queue->msgQueueHead->next;

	if (m == queue->msgQueueTail) {
		queue->msgQueueHead = queue->msgQueueTail = NULL;
	}

	pthread_mutex_unlock(&queue->queueMutex);
	return m;
}

void DumpMessage(struct MessageQueue *queue) {
	pthread_mutex_lock(&queue->queueMutex);
	struct Message *next = queue->msgQueueHead;
	while (next != NULL) {
		//printf("message node %x\n", (void *)next);
		next = next->next;
	}
	pthread_mutex_unlock(&queue->queueMutex);
}
