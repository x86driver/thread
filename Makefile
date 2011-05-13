TARGET = thread cond lock
CFLAGS = -Wall

all:$(TARGET)

thread:thread.c
	gcc -Wall -o $@ $< -lpthread $(CFLAGS) -g

cond:cond.c
	gcc -Wall -o $@ $< -lpthread $(CFLAGS) -g

lock:lock.c
	gcc -Wall -o $@ $< -lpthread $(CFLAGS) -g

clean:
	rm -rf $(TARGET)
