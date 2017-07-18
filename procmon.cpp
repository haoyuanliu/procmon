#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <fcgio.h>
#include <fcgios.h>

#include "util.h"
using namespace std;

void *work_thread(void *arg) {
    int threadid = *(static_cast<int*>(arg));
    while(1) {
        int ret = 0;
        int count = 0;
        FCGX_Request httpReq;
        FCGX_InitRequest(&httpReq, 0, 0);
        ret = FCGX_Accept_r(&httpReq);
        if (ret >= 0) {
            struct timeval time;
            gettimeofday(&time, NULL);
            string response("This is the ");
            response.append(Util::transToString(count));
            response.append(" time of visit!");
            string httpRes("Status: 200 OK\r\nContent-type: application/octet-stream\r\n");
			httpRes.append("Content-Length: ").append(Util::transToString(response.size()));
            httpRes.append("\r\n\r\n");
            httpRes.append(response);
            FCGX_PutStr(httpRes.c_str(), httpRes.size(), httpReq.out);
        }
    }
}

int main() {
    int work_num = 2;
#if 0
    sigset_t mask;
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sa.sa_mask = mask;
    if (sigaction(SIGPIPE, &sa, 0) == -1) {
        exit(-1);
    }
    if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
        return -1;
    }
#endif
    FCGX_Init();
    pthread_t *work_tid = new pthread_t[work_num];
    for (int i = 0; i < work_num; ++i) {
        pthread_create(&work_tid[i], NULL, work_thread, (void *)&i);
        usleep(10);
    }

    for (int i = 0; i < work_num; ++i) {
        pthread_join(work_tid[i], NULL);
    }

    delete[] work_tid;
    return 0;
}
