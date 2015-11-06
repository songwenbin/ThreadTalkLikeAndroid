#ifndef _MESSAGE_HEADER_
#define _MESSAGE_HEADER_

#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

struct Message {
	int  what;
	int  args1;
	int  args2;
	void *obj;	
	void *(*callback);
	void *target;
	struct Message *next;
};

struct Message *NewMessage();
void DeleteMessage(struct Message *m);

#endif
