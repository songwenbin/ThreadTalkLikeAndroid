#ifndef _MESSAGE_HEADER_
#define _MESSAGE_HEADER_

#include "Common.h"

struct Message {
	int  what;
	int  arg1;
	int  arg2;
	void *obj;	
	void *(*callback);
	void *target;
	struct Message *next;
};

struct Message *NewMessage();
void DelMessage(struct Message *m);

#endif
