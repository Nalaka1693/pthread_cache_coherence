#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>

#define GET_US(X) (X.tv_sec * 1000000 + X.tv_usec)
#define ITE 5000
#define MAX 3000000
#define N 64

void main_pthread(int *, int *);

void *counting_thread(void *);

int u[2] __attribute__ ((aligned (N)));

int at1 __attribute__ ((aligned (N)));
int at2 __attribute__ ((aligned (N)));


int main() {
    struct timeval start, end;
    struct timezone z;

    printf("&u = %d\n", (int) u);

    long int total;
    int j, k;

    for (total = 0, j = 0; j < ITE; j++) {  // for aligned version
        if (gettimeofday(&start, &z)) goto error_exit;
        main_pthread(&at1, &at2); // make sure it exits only after completing
        if (gettimeofday(&end, &z)) goto error_exit;
        total += GET_US(end) - GET_US(start);
    }
    printf("Aligned\t\t(%p and %p)\tAvg=%ld\n", &at1, &at2, total / j);
    printf("Aligned\t\t(%d and %d)\tAvg=%ld\n", (int) &at1, (int) &at2, total / j);
    printf("diff = %d\n", (int) &at1 - (int) &at2);

    for (total = 0, j = 0; j < ITE; j++) {  // For un-aligned version
        if (gettimeofday(&start, &z)) goto error_exit;
        main_pthread(&u[0], &u[1]); // make sure it exits only after completing
        if (gettimeofday(&end, &z)) goto error_exit;
        total += GET_US(end) - GET_US(start);
    }
    printf("Un-aligned\t(%p and %p)\tAvg=%ld\n", &u[0], &u[1], total / j);
    printf("Un-aligned\t(%d and %d)\tAvg=%ld\n", (int) &u[0], (int) &u[1], total / j);
    printf("diff = %d\n", (int) &u[0] - (int) &u[1]);

    for (total = 0, j = 0; j < ITE; j++) {  // for local vars
        if (gettimeofday(&start, &z)) goto error_exit;
        main_pthread(NULL, NULL); // make sure it exits only after completing
        if (gettimeofday(&end, &z)) goto error_exit;
        total += GET_US(end) - GET_US(start);
    }
    printf("With local\t\t\t\tAvg=%ld\n", total / j);

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

void *counting_thread(void *c) {
    int i, *j;
    if (c) {
        j = c;
    } else {
        j = &i;
    }

    for (*j = 0; *j < MAX; (*j)++);

    pthread_exit(NULL);
}
