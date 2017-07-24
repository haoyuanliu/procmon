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
#include <string.h>
#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <fcgio.h>
#include <fcgios.h>
#include <fcgi_stdio.h>

#include "util.h"
#include "CycleBuffer.h"
#include "httpResponse.h"
#include "TimeStamp.h"
using namespace std;

//全局变量，保存buffer信息
#define LISTENPID 4687
#define BUFFLEN 600
int g_clockTicks = static_cast<int>(::sysconf(_SC_CLK_TCK));
int g_pageSize = static_cast<int>(::sysconf(_SC_PAGE_SIZE));
CycleBuffer *cycle_buffer;

struct ItemRequest {
    string str_uri;
    string str_uri_all;
    string server_name;
};

struct CpuTime {
    int userTime_;
    int sysTime_;
    double getCpuTime(double period, double ticksPerSecond) const {
        return (userTime_ + sysTime_) / (ticksPerSecond * period);
    }
};

void getHttpRequest(FCGX_Request &fcgi_request, ItemRequest &item_request) {
    char *arg_para;
    arg_para = FCGX_GetParam("SERVER_NAME", fcgi_request.envp);
    if ((arg_para != NULL) && strlen(arg_para) > 0) {
        item_request.server_name.assign(arg_para);
    }

    arg_para = FCGX_GetParam("REQUEST_URI", fcgi_request.envp);
    if ((arg_para != NULL) && strlen(arg_para) > 0) {
        item_request.str_uri_all.assign(arg_para);
        char *tmppos = strchr(arg_para, '?');
        if (tmppos) *tmppos = 0;
        item_request.str_uri.assign(arg_para);
    }
}

void *produce_thread(void *arg) {
    int fd = *(static_cast<int*>(arg));
    char buf[1024];
    int ret;
    int count = 0;
    StatData stat_data;
    StatData last_data;
    while(1) {
        if ((ret = pread(fd, buf, sizeof buf, 0)) == -1) {
            cout << "Read /proc/" << getpid() << "/stat error!" << endl;
        }
        stat_data.parse(buf);
        CpuTime time;
        time.userTime_ = std::max(0, static_cast<int>(stat_data.utime - last_data.utime));
        time.sysTime_ = std::max(0, static_cast<int>(stat_data.stime - last_data.stime));
        cycle_buffer->write(static_cast<double>(time.getCpuTime(1, g_clockTicks)));
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

            ItemRequest item_request;
            getHttpRequest(httpReq, item_request);

            if ("/procmon" == item_request.str_uri) {

            } else if ("/procmon/cpu.png") {

            }

            TimeStamp now = TimeStamp::now();
            httpResponse httpRes;
            httpRes.setStatus(httpResponse::k200Ok);
            httpRes.setStatusStr("OK");
            httpRes.setCloseConnection(false);
            httpRes.setContentType("text/html");
            httpRes.addHeader("test_key", "test_value");
            httpRes.appendBody("<html><head><title>%s on %s</title>\n", "procmon", "kunto");
            httpRes.appendBody("<h1>%s on %s</h1>\n", "procmon", "kunto");
            httpRes.appendBody("<p>Page generated at %s (UTC)", now.toFormattedString().c_str());
            httpRes.appendBody("<p>Request URI ALL is: %s", item_request.str_uri_all.c_str());
            httpRes.appendBody("<p>Request URI is: %s", item_request.str_uri.c_str());
            httpRes.appendBody("</head></html>");
            string response = httpRes.toString();
            FCGX_PutStr(response.c_str(), response.size(), httpReq.out);
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
    httpResponse test;
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
    cycle_buffer = new CycleBuffer(BUFFLEN);
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
    delete cycle_buffer;
    return 0;
}
