#include "Message.h"

struct Message *NewMessage() {
	struct Message *m = (struct Message *)malloc(sizeof(struct Message));
	if (m == NULL) {
		LOG_ERROR("allocated message error!\n");
	}

	m->what = 0;
	m->arg1 = 0;
	m->arg2 = 0;
	m->obj = NULL;
	m->callback = NULL;
	m->next = NULL;

	return m;
}

void DelMessage(struct Message *m) {
	if (m == NULL) {
		return;
	}

	free(m);
}
