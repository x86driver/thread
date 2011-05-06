#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_gps_data;
pthread_cond_t cond_gps_data;
char buf[256];
int data_ready;

void *read_thread(void *arg)
{
    while (1) {
        pthread_mutex_lock(&mutex_gps_data);
//        if (data_ready == 0) {
            printf("Enter something: ");
            scanf("%s", buf);
            printf("In read thread: %s\n", buf);
        if (data_ready == 0) {
            data_ready = 1;
            pthread_cond_signal(&cond_gps_data);
        }
        pthread_mutex_unlock(&mutex_gps_data);
        if (buf[0] == '0')
            return NULL;
    }
    return NULL;
}

void *use_thread(void *arg)
{
    while (1) {
        pthread_mutex_lock(&mutex_gps_data);
        printf("In use thread, wait for data...\n");
        while (data_ready == 0)
            pthread_cond_wait(&cond_gps_data, &mutex_gps_data);
        printf("In use thread, I got: %s\n", buf);
        data_ready = 0;
        pthread_mutex_unlock(&mutex_gps_data);
        if (buf[0] == '0')
            return NULL;
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;

    data_ready = 0;
    pthread_mutex_init(&mutex_gps_data, NULL);
    pthread_cond_init(&cond_gps_data, NULL);

    pthread_create(&tid1, NULL, use_thread, NULL);
    pthread_create(&tid2, NULL, read_thread, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
