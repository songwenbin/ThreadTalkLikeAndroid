#ifndef __MESSAGE_POOL_HEAD_
#define __MESSAGE_POOL_HEAD_

#include "Message.h"

#define MESSAGE_INIT_NUM 1024*1024

struct MessagePool {
	struct Message *head;
	struct Message *tail;
	int count;
	pthread_mutex_t poolMutex;
};

struct MessagePool *CreateMessagePool();

struct Message *GetMessage(struct MessagePool *pool);

void PutMessage(struct MessagePool *pool, struct Message *m);

#endif
