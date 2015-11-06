#include "Message.h"

struct Message *NewMessage() {
	struct Message *m = (struct Message *)malloc(sizeof(struct Message));
	if (m == NULL) {
		fprintf(stderr, "allocated message error!\n");
	}

	m->what = 0;
	m->args1 = 0;
	m->args2 = 0;
	m->obj = NULL;
	m->callback = NULL;
	m->next = NULL;

	return m;
}

void DeleteMessage(struct Message *m) {
	if (m == NULL) {
		return;
	}

	free(m);
}
