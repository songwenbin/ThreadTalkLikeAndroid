#include "MessageQueue.h"

struct MessageQueue *NewMessageQueue() {
	struct MessageQueue * queue = (struct MessageQueue *)malloc(sizeof(struct MessageQueue));
	if (queue == NULL) {
		LOG_ERROR("allocated queue error!\n");
		return NULL;
	}

	queue->msgQueueHead = NULL;
	queue->msgQueueTail = NULL;

#ifdef LOCK_FREE_QUEUE	
	lf_queue_init(&queue->queue, 100, 100, 1024*1024);
#else	
	pthread_mutex_init(&queue->queueMutex, NULL);
#endif
	return queue;
}

void DeleteMessageQueue(struct MessageQueue *queue) {
	if (queue == NULL) {
		return;
	}

#ifdef LOCK_FREE_QUEUE	
#else	
	pthread_mutex_destroy(&queue->queueMutex);
#endif
	free(queue);
}

void EnqueueMessage(struct MessageQueue *queue, struct Message *m) {
#ifdef LOCK_FREE_QUEUE	
	lf_queue_push(&queue->queue, m);
#else	
	pthread_mutex_lock(&queue->queueMutex);

	if (queue->msgQueueHead == NULL) {
		queue->msgQueueHead = m;
		queue->msgQueueTail = m;
	} else {
		queue->msgQueueTail->next = m;
		queue->msgQueueTail = m;
	}

	pthread_mutex_unlock(&queue->queueMutex);
#endif
}

struct Message *DequeueMessage(struct MessageQueue *queue) {
	struct Message *m = NULL;
#ifdef LOCK_FREE_QUEUE	
	lf_queue_pop(&queue->queue, m);
#else	
	if (pthread_mutex_trylock(&queue->queueMutex) != 0) {
		return NULL;
	}

	if (queue->msgQueueTail == NULL ) {
		pthread_mutex_unlock(&queue->queueMutex);
		return NULL;
	}

	m = queue->msgQueueHead;
	queue->msgQueueHead = queue->msgQueueHead->next;

	if (m == queue->msgQueueTail) {
		queue->msgQueueHead = queue->msgQueueTail = NULL;
	}

	pthread_mutex_unlock(&queue->queueMutex);
#endif
	return m;
}

void DumpMessage(struct MessageQueue *queue) {
	pthread_mutex_lock(&queue->queueMutex);
	struct Message *next = queue->msgQueueHead;
	while (next != NULL) {
		//LOG_ERROR("message node %x\n", (void *)next);
		next = next->next;
	}
	pthread_mutex_unlock(&queue->queueMutex);
}
