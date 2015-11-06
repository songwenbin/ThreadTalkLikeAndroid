#ifndef _MESSAGE_QUEUE_HEADER_
#define _MESSAGE_QUEUE_HEADER_

#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include "Message.h"

#define MSG_QUEUE_SIZE 1024

struct MessageQueue {
	struct Message *msgQueueHead;
	struct Message *msgQueueTail;
	pthread_mutex_t queueMutex;
};

struct MessageQueue *NewMessageQueue();
void DeleteMessageQueue(struct MessageQueue *queue);
void EnqueueMessage(struct MessageQueue *queue, struct Message *m);
struct Message *DequeueMessage(struct MessageQueue *queue);

#endif
