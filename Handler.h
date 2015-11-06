#ifndef _HANDLER_HEADER_
#define _HANDLER_HEADER_

#include <sys/eventfd.h>

#include "MessageQueue.h"
#include "MessagePool.h"
#include "Message.h"
#include "Looper.h"

struct Handler {
	int eventFd;
	int (*HandleMessage)(struct Message *m);
	struct MessageQueue *mQueue;
	struct MessagePool *pool;
	struct Looper *loop;
};

struct Handler *NewHandler();
void DelHandler(struct Handler *handle);
struct Message *ObtainMessage(struct Handler *handler);
struct Message *ObtainMessageWithWhat(struct Handler *handler, int what);
void SendMessage(struct Handler* handler, struct Message *m);
void HandleMessageQueue(struct Handler *handler);

#endif
