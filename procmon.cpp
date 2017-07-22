#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <fcgio.h>
#include <fcgios.h>
#include <fcgi_stdio.h>

#include "util.h"
using namespace std;

void *work_thread(void *arg) {
    int threadid = *(static_cast<int*>(arg));
    int ret = 0;
    int count= 0;
    FCGX_Request httpReq;
    FCGX_InitRequest(&httpReq, 0, 0);
    while(1) {
        ret = FCGX_Accept_r(&httpReq);
        if (ret >= 0) {
            struct timeval time;
            gettimeofday(&time, NULL);
            char *server_name;
            server_name = FCGX_GetParam("SERVER_NAME", httpReq.envp);
            string response("This is the ");
            response.append(Util::transToString(++count));
            response.append(" time of visit of thread " + Util::transToString(threadid) + " !<br>");
            response.append("And tid is " + Util::transToString(gettid()) + " !");
            response.append("Server Name: " + string(server_name));
            string httpRes("Status: 200 OK\r\nContent-type: text/html\r\n");
			httpRes.append("Content-Length: ").append(Util::transToString(response.size()));
            httpRes.append("\r\n\r\n");
            httpRes.append(response);
            printf("%s\r\n", "Hello Nginx!");
            FCGX_PutStr(httpRes.c_str(), httpRes.size(), httpReq.out);
            FCGX_Finish_r(&httpReq);
        } else {
            break;
        }
    }
}

int main() {
    int work_num = 10;
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
