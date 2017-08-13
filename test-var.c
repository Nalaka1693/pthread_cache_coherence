#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>

#define GET_US(X) (X.tv_sec * 1000000 + X.tv_usec)
#define ITE 5000
#define MAX 3000000

int at_1;
int at_2;
int at4_1 __attribute__ ((aligned(4)));
int at4_2 __attribute__ ((aligned(4)));
int at8_1 __attribute__ ((aligned(8)));
int at8_2 __attribute__ ((aligned(8)));
int at16_1 __attribute__ ((aligned(16)));
int at16_2 __attribute__ ((aligned(16)));
int at32_1 __attribute__ ((aligned(32)));
int at32_2 __attribute__ ((aligned(32)));
int at64_1 __attribute__ ((aligned(64)));
int at64_2 __attribute__ ((aligned(64)));
int at128_1 __attribute__ ((aligned(128)));
int at128_2 __attribute__ ((aligned(128)));
int at256_1 __attribute__ ((aligned(256)));
int at256_2 __attribute__ ((aligned(256)));

int u32[2] __attribute__ ((aligned(32)));
int u64[2] __attribute__ ((aligned(64)));

const char *align[11] = {"at", "at4", "at8", "at16", "at32", "at64", "at128", "at256", "u32", "u64", "NULL"};
int *var_array[] = {&at_1, &at_2,
                    &at4_1, &at4_2,
                    &at8_1, &at8_2,
                    &at16_1, &at16_2,
                    &at32_1, &at32_2,
                    &at64_1, &at64_2,
                    &at128_1, &at128_2,
                    &at256_1, &at256_2,
                    &u32[0], &u32[1],
                    &u64[0], &u64[1],
                    NULL, NULL};

void main_pthread(int *, int *);
void *counting_thread(void *);

int main() {
    struct timeval start, end;
    struct timezone z;
    long int total;
    int i = 0, j, k;

    for (k = 0; k < 22; k+=2) {
        for (total = 0, j = 0; j < ITE; j++) {
            if (gettimeofday(&start, &z)) goto error_exit;
            main_pthread(var_array[k], var_array[k+1]);   // make sure it exits only after completing
            if (gettimeofday(&end, &z)) goto error_exit;
            total += GET_US(end) - GET_US(start);
        }
        printf("%s\t(%d and %d)\tdiff = %d\tAvg=%ld\n", align[i++],
               (int) var_array[k], (int) var_array[k+1], (int) var_array[k+1] - (int) var_array[k], total / j);
    }

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
