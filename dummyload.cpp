#include <iostream>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include "TimeStamp.h"

using namespace std;
int g_cycles = 0;
int g_percent = 0;
bool g_busy = false;
pthread_mutex_t g_mutex;
pthread_cond_t g_cond;

void busy(int cycles) {
    double res = 0;
    for (int i = 0; i < cycles; ++i) {
        res += sqrt(i) * sqrt(i+1);
    }
}

void *loadFunc(void *) {
    while(1) {
        pthread_mutex_lock(&g_mutex);
        while(!g_busy) {
            pthread_cond_wait(&g_cond, &g_mutex);
        }
        pthread_mutex_unlock(&g_mutex);
        busy(g_cycles);
    }
}

void load(int percent) {
    percent = std::max(0, percent);
    percent = std::min(100, percent);

    int err = 2 * percent - 100;
    int count = 0;

    for (int i = 0; i < 100; ++i) {
        bool busy = false;
        if (err > 0) {
            busy = true;
            err += 2 * (percent - 100);
            count++;
        } else {
            err += 2 * percent;
        }
        pthread_mutex_lock(&g_mutex);
        g_busy = busy;
        pthread_cond_broadcast(&g_cond);
        pthread_mutex_unlock(&g_mutex);
        usleep(10 * 1000);
    }
}

void fixed(){
    while(true) {
        load(g_percent);
    }
}

void cosine() {
    while(true){
        for (int i = 0; i < 100; ++i) {
            int percent = static_cast<int>((1.0 + cos(i * 3.1415926 / 50)) / 2 * g_percent + 0.5);
            load(percent);
        }
    }
}

void sawtooth() {
    while(true) {
        for (int i = 0; i < 100; ++i) {
            int percent = static_cast<int>(i / 100.0 * g_percent);
            load(percent);
        }
    }
}


double getSeconds(int cycles) {
    TimeStamp start = TimeStamp::now();
    busy(cycles);
    return timeDiff(TimeStamp::now(), start);
}

void findCycles() {
    g_cycles = 1000;
    while(getSeconds(g_cycles) < 0.001)
        g_cycles = g_cycles + g_cycles / 4;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [fct] [percent] [num_threads]\n", argv[0]);
        return 0;
    }
    cout << getpid() << endl;
    findCycles();
    g_percent = argc > 2 ? atoi(argv[2]) : 50;
    int numThreads = argc > 3 ? atoi(argv[3]) : 1;
    pthread_t *load_tid = new pthread_t[numThreads];
    for(int i = 0; i < numThreads; ++i) {
        pthread_create(&load_tid[i], NULL, loadFunc, NULL);
    }
    switch (argv[1][0]) {
        case 'f':
            fixed();
            break;
        case 'c':
            cosine();
            break;
        case 'z':
            sawtooth();
            break;
        default:
            break;
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(load_tid[i], NULL);
    }
    delete [] load_tid;
    return 0;
}
