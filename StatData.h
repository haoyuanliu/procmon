#include <sstream>
#include <string>

class StatData {
    public:
        StatData();
        ~StatData();
        void parse(const char* startStat);
        std::string getState(const char stat);
        int pid;
        std::string command;
        char state;
        int ppid;
        int pgrp;
        int session;
        int tty_nr;
        int tgpid;
        int flags;

        long minflt;
        long cminflt;
        long majflt;
        long cmajflt;

        long utime;
        long stime;
        long cutime;
        long cstime;

        long priority;
        long nice;
        long num_threads;
        long itrealvalue;
        long starttime;

        long vsize;
        long rss;
        long rsslim;
    private:
        pthread_mutex_t mutex_;
};
