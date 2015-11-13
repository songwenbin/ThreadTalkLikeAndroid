
int HandleMessageImpl(struct Message *m) {
	switch(m->what) {
		case 0x01:
		break;
	}
}

void MainThread() {
	struct Handler *handler = NewHandler();
	handle->handleMessage = HandleMessageImpl;
}


void SubThread(struct Handler *handler) {
	struct Message *m = ObtainMessage(handler);
	m->what = 0x01;
	SendMessage(handler, m);
}
