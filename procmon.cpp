#include <iostream>
#include <string>
#include <pthread.h>
#include <fcgi_stdio.h>

#include "util.h"
using namespace std;

void *work_thread(void *arg) {
    int threadid = *(static_cast<int*>(arg));
    while(1) {
        int ret = 0;
        int count = 0;
        FCGX_Request httpReq;
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
    int work_num = 5;
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
