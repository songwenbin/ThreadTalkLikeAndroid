#include "Handler.h"
#include "Looper.h"
#include <pthread.h>

__thread struct Looper *gLoop = NULL;

struct Handler *NewHandler() {
	struct Handler *handler = (struct Handler*)malloc(sizeof(struct Handler));
	if (handler == NULL) {
		fprintf(stderr, "allocated handler error\n");
		return NULL;
	}

	handler->eventFd = eventfd(0, EFD_NONBLOCK);
	handler->mQueue  = NewMessageQueue();
	handler->pool = CreateMessagePool();
	handler->HandleMessage = NULL;

	if (gLoop == NULL) {
		handler->loop = gLoop = CreateLooper();
	} else {
		handler->loop = gLoop;
	}

	//printf("Create loop address %x\n", handler->loop);

	RegisterHandler(handler->loop, handler);

	return handler;
}

void DelHandler(struct Handler *handler) {
	//printf("Del loop address %x\n", handler->loop);
	DeleteLooper(handler->loop);
	handler->loop = NULL;
	gLoop = NULL;
	DeleteMessageQueue(handler->mQueue);
	close(handler->eventFd);
	free(handler);
}

void SendMessage(struct Handler* handler, struct Message *m) {
	uint64_t u = 1;
	EnqueueMessage(handler->mQueue, m);
	//printf("xxx %d\n", m->what);
	write(handler->eventFd, &u, sizeof(uint64_t));
}

struct Message *ObtainMessage(struct Handler *handler) {
	struct Message *m;
	if (handler->pool->count == 0) {
	       	m = NewMessage();
		m->target = handler;
	} else {
		m = GetMessage(handler->pool);
		m->target = handler;
	}

	/*
	m = GetMessage(handler->pool);
	if (m == NULL) {
		m = NewMessage();
	}
	m->target = handler;
	*/
	return m;
}

struct Message *ObtainMessageWithWhat(struct Handler *handler, int what) {
	struct Message *m = NewMessage();
	m->what = what;
	return m;
}

void HandleMessageQueue(struct Handler *handler) {
	while (1) {
		struct Message *m = DequeueMessage(handler->mQueue);
		if (m == NULL) {
			return;
		}

		if (m->target == handler && handler->HandleMessage != NULL) {
			handler->HandleMessage(m);
			PutMessage(handler->pool, m);
			//DeleteMessage(m);
		}
	}
}
