#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define GET_US(X) (X.tv_sec * 1000000 + X.tv_usec)
#define ITE 5000
#define MAX 3000000

const char *align[11] = {"at4 ", "at8 ", "at16", "at32", "at64", "at128", "at256", "at512", "at1024", "at2048", "NULL"};
int var_array32[10][1024] __attribute__ ((aligned(32)));
int var_array64[10][1024] __attribute__ ((aligned(64)));
int var_array1024[10][1024] __attribute__ ((aligned(1024)));

int get_time(int [][1024]);
void main_pthread(int *, int *);
void *counting_thread(void *);

int main() {
//    get_time(var_array32);
//    get_time(var_array64);
    get_time(var_array1024);

    return 0;
}

int get_time(int arr[][1024]) {
    struct timeval start, end;
    struct timezone z;
    long int total;
    int a = 1, i = 0, j, k;

    for (k = 0; k < 9; k++) {
        for (total = 0, j = 0; j < ITE; j++) {
            if (gettimeofday(&start, &z)) goto error_exit;
            main_pthread(&arr[k][0], &arr[k][a]);   // make sure it exits only after completing
            if (gettimeofday(&end, &z)) goto error_exit;
            total += GET_US(end) - GET_US(start);
        }
        printf("%s\t(%p and %p)\tdiff = %d\tAvg=%ld\n", align[i++],
               &arr[k][0], &arr[k][a], (uintptr_t) &arr[k][a] - (uintptr_t) &arr[k][0], total / j);
        a = a * 2;
    }

    for (total = 0, j = 0; j < ITE; j++) {
        if (gettimeofday(&start, &z)) goto error_exit;
        main_pthread(NULL, NULL);   // make sure it exits only after completing
        if (gettimeofday(&end, &z)) goto error_exit;
        total += GET_US(end) - GET_US(start);
    }
    printf("%s\t(%d and %d)\t\t\t\t\t\t\t\tdiff = %d\tAvg=%ld\n", align[10], 0, 0, 0, total / j);

    return 0;

    error_exit:
    printf("Cannot read time, too bad\n");
    return -1;
}

void main_pthread(int *c1, int *c2) {
    pthread_t tid1, tid2;
    pthread_attr_t attr;

    pthread_attr_init(&attr); // set default attributes
    pthread_create(&tid1, &attr, counting_thread, c1); // create new thread
    pthread_create(&tid2, &attr, counting_thread, c2); // create new thread

    void *p;
    pthread_join(tid1, &p);
    pthread_join(tid2, &p);

    return;
}

void *counting_thread(void *arg) {
    int local, *j;

    if (arg) { j = arg; }
    else { j = &local; }

    for (*j = 0; *j < MAX; (*j)++);

    pthread_exit(NULL);
}
