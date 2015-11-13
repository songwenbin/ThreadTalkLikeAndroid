#ifndef _MESSAGE_QUEUE_HEADER_
#define _MESSAGE_QUEUE_HEADER_

#include "pthread.h"
#include "Common.h"
#include "lfqueue.h"

#include "Message.h"

struct MessageQueue {
	struct Message *msgQueueHead;
	struct Message *msgQueueTail;
	pthread_mutex_t queueMutex;
	lf_queue queue;
};

struct MessageQueue *NewMessageQueue();
void DeleteMessageQueue(struct MessageQueue *queue);
void EnqueueMessage(struct MessageQueue *queue, struct Message *m);
struct Message *DequeueMessage(struct MessageQueue *queue);

#endif
