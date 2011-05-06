TARGET = thread cond
CFLAGS = -Wall

all:$(TARGET)

thread:thread.c
	gcc -Wall -o $@ $< -lpthread $(CFLAGS) -g

cond:cond.c
	gcc -Wall -o $@ $< -lpthread $(CFLAGS) -g

clean:
	rm -rf $(TARGET)
