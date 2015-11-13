#include "Handler.h"
#include "Looper.h"
#include "Common.h"
#include <pthread.h>

__thread struct Looper *gLoop = NULL;

struct Handler *NewHandler() {
	struct Handler *handler = (struct Handler*)malloc(sizeof(struct Handler));
	if (handler == NULL) {
		LOG_ERROR("allocated handler error\n");
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

	RegisterHandler(handler->loop, handler);

	return handler;
}

void DelHandler(struct Handler *handler) {
	if (gLoop != NULL) {
		DeleteLooper(handler->loop);
		handler->loop = NULL;
		gLoop = NULL;
	}
	DeleteMessageQueue(handler->mQueue);
	DeleteMessagePool(handler->pool);
	close(handler->eventFd);
	free(handler);
}

void SendMessage(struct Handler* handler, struct Message *m) {
	uint64_t u = 1;
	EnqueueMessage(handler->mQueue, m);
	write(handler->eventFd, &u, sizeof(uint64_t));
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
		}
	}
}

struct Message *ObtainMessage(struct Handler *handler) {
	struct Message *m;
	
	m = GetMessage(handler->pool);
	if (m == NULL) {
		m = NewMessage();
	}
	m->target = handler;
	return m;
}

struct Message *ObtainMessageWithWhat(struct Handler *handler, int what) {
	struct Message *m = ObtainMessage(handler);
	m->what = what;
	return m;
}

struct Message *ObtainMessageWithArgs(struct Handler *handler, int arg1, int arg2) {
	struct Message *m = ObtainMessage(handler);
	m->arg1 = arg1;
	m->arg2 = arg2;
	return m;
}
