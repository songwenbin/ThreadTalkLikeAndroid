#ifndef _LOOPER_HEADER_
#define _LOOPER_HEADER_

#include "Handler.h"

#define HANDLER_NUM 1024
#define STOP_LOOP 0x11

struct Looper {
	int efd;

	int handleNum;
	struct Handler *handlers[HANDLER_NUM];

	int activeNum;
	int activeHandlers[HANDLER_NUM*HANDLER_NUM];

	int stopFd;
};

struct Looper *CreateLooper();
void DeleteLooper(struct Looper *loop);
void LoopStart(struct Looper *loop);
void LoopStop(struct Looper *loop);
void RegisterHandler(struct Looper *loop, struct Handler *handler);

int DispatchMessage(struct Looper *loop);
int Looper(struct Looper *loop);

#endif
