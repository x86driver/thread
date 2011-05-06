#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define PENDING_EXIT 2
#define START 1
#define STOP 0

#define WAIT_SECOND 5

int thread_status = 0;
pthread_t thread;

void *lib_thread_func(void *arg)
{
    int token = 0;
    while (thread_status == START) {
        printf("send %d\n", token);
        usleep(500000);
        printf("recv %d\n", token);
        ++token;
        usleep(500000);
    }
    return NULL;
}

void start()
{
    // create a thread, and send/recv some data
    printf("Start ...\n");

    while (thread_status == PENDING_EXIT)
        sleep(1);
    thread_status = START;
    pthread_create(&thread, NULL, lib_thread_func, NULL);
}

void stop()
{
    // stop the thread, or join
    printf("Stop ...\n");
    thread_status = PENDING_EXIT;
//    if (thread != 0)
//        pthread_join(thread, NULL);
}

void *thread1(void *arg)
{
    start();
    sleep(1);
    start();
    while (1);
/*
    int sec;
    while (1) {
        start();
        sec = rand() % WAIT_SECOND;
        sleep(sec);
    }
*/
    return NULL;
}

void *thread2(void *arg)
{
    sleep(1);
    stop();
    while (1);
/*
    int sec;
    while (1) {
        stop();
        sec = rand() % WAIT_SECOND;
        sleep(sec);
    }
*/
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
