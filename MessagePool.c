#include "MessagePool.h"

struct MessagePool *CreateMessagePool() {
	struct MessagePool *pool = (struct MessagePool *)malloc(sizeof(struct MessagePool));
	if (pool == NULL) {
		printf("allocated pool error\n");
		return NULL;
	}

	pthread_mutex_init(&pool->poolMutex, NULL);

	pool->count = 0;
	int i = 0;
	for (i = 0; i < MESSAGE_INIT_NUM; i ++) {
		struct Message *m = NewMessage();
		PutMessage(pool, m);
	}


	return pool;
}

void DeleteMessagePool(struct MessagePool *pool) {
	if (pool != NULL) {
		pthread_mutex_destroy(&pool->poolMutex);
		free(pool);
	}
}

struct Message *GetMessage(struct MessagePool *pool) {
	pthread_mutex_lock(&pool->poolMutex);
	if (pool->count == 0) {
		return NULL;
	}
	struct Message *m = pool->head;
	pool->head = pool->head->next;
	if (m == pool->tail) {
		pool->head = pool->tail = NULL;
	}
	pool->count--;
	pthread_mutex_unlock(&pool->poolMutex);

	return m;
}

void PutMessage(struct MessagePool *pool, struct Message *m) {
	pthread_mutex_lock(&pool->poolMutex);
	if (pool->count == 0) {
		pool->head = m;
		pool->tail = m;
	} else {
		pool->tail->next = m;
		pool->tail = m;
	}
	pool->count++;
	pthread_mutex_unlock(&pool->poolMutex);
}
