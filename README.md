# A Inter-Thread Commincation Library

## Usage

### main thread code
```
int HandleMessageImpl(struct Message *m) {
	switch(m->what) {
		case 0x01:
		break;
	}
}

void MainThread() {
	struct Handler *handler = NewHandler();
	handle->handleMessage = HandleMessageImpl;

	LooperStart(handler->loop);
}
```

### sub thread code
```
void SubThread(struct Handler *handler) {
	struct Message *m = ObtainMessage(handler);
	m->what = 0x01;
	SendMessage(handler, m);
}
```
## UniTest
```
# cd ./unitest/
# ./test.sh
```
