#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define THRESHOLDOG 3
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid1, tid2, tid3;
static unsigned int varidog = 0;

void signal_watchdog(int sig)
{
    printf("Got SIGUSR1, exit...\n");
    pthread_exit(NULL);
}

void init_signal()
{
    struct sigaction act;
    act.sa_handler = &signal_watchdog;
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
}

void *thread1(void *arg)
{
    init_signal();
while(1) {
    printf("thread1 will lock twice\n");
    pthread_mutex_lock(&mutex);
    printf("thread1 in lock\n");
    ++varidog;
    sleep(1);
    if (varidog == 2)
        pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
}
    return NULL;
}

void *thread2(void *arg)
{
    printf("thread2 will try to lock\n");
    pthread_mutex_lock(&mutex);
    sleep(4);
    printf("thread2 never go here\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *watchdog(void *arg)
{
    unsigned int olddog = 0;
    unsigned int threshold = 0;
    while (1) {
        if (olddog != varidog)
            olddog = varidog;
        else
            ++threshold;
        if (threshold > THRESHOLDOG) {
            printf("Deadlock detect!\n");
            threshold = olddog = 0;
            pthread_kill(tid1, SIGUSR1);
            break;
        }
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, watchdog, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    printf("Good bye!\n");
    return 0;
}

