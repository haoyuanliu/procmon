#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
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
#include "CycleBuffer.h"
#include "StatData.h"
using namespace std;

//全局变量，保存buffer信息
#define LISTENPID 4687
#define BUFFLEN 600
CycleBuffer *cputime;

void *produce_thread(void *arg) {
    int fd = *(static_cast<int*>(arg));
    char buf[1024];
    int ret;
    int count = 0;
    StatData stat_data;
    while(1) {
        if ((ret = pread(fd, buf, sizeof buf, 0)) == -1) {
            cout << "Read /proc/" << getpid() << "/stat error!" << endl;
        }
        stat_data.parse(buf);
        cputime->write(static_cast<double>(stat_data.pid));
        sleep(1);
    }
}

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
            response.append(" Buffer pointer: " + Util::transToString(cputime->getWriteIndex()));
            response.append("<br>utime: " + Util::transToString(cputime->getValue(cputime->getWriteIndex()-1)));
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

bool pidExist(int pid) {
    char filename[256];
    snprintf(filename, sizeof filename, "/proc/%d/stat", pid);
    return ::access(filename, R_OK) == 0;
}

int main(int argc, char *argv[]) {
#if 0
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " pid port name" << endl;
        return 0;
    }


    int pid = atoi(argv[1]);
    if (!pidExist(pid)) {
        cout << pid << " is not exist!" << endl;
        return 0;
    }
#endif
    cputime = new CycleBuffer(BUFFLEN);
    int pid = LISTENPID;
    if (!pidExist(pid)) {
        cout << pid << " is not exist!" << endl;
        return 0;
    }
    char filename[256];
    snprintf(filename, sizeof filename, "/proc/%d/stat", pid);
    int fd = open(filename, O_RDONLY);

    pthread_t produce_tid;
    pthread_create(&produce_tid, NULL, produce_thread, static_cast<void*>(&fd));

    int work_num = 2;
    FCGX_Init();
    pthread_t *work_tid = new pthread_t[work_num];
    for (int i = 0; i < work_num; ++i) {
        pthread_create(&work_tid[i], NULL, work_thread, static_cast<void*>(&i));
        usleep(10);
    }

    pthread_join(produce_tid, NULL);
    for (int i = 0; i < work_num; ++i) {
        pthread_join(work_tid[i], NULL);
    }

    close(fd);
    delete[] work_tid;
    delete cputime;
    return 0;
}
