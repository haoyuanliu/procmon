#include "StatData.h"

StatData::StatData() {
    pthread_mutex_init(&mutex_);
}

StatData::~StatData() {
    pthread_mutex_destroy(&mutex_);
}

void StatData::parse(const char* startStat) {
    std::istringstream iss(startStat);
    pthread_mutex_lock(&mutex_);
    iss >> pid >> command >> state;
    iss >> ppid >> pgrp >> session >> tty_nr >> tgpid >> flags;
    iss >> minflt >> cminflt >> majflt >> cmajflt;
    iss >> utime >> stime >> cutime >> cstime;
    iss >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    iss >> vsize >> rss >> rsslim;
    pthread_mutex_unlock(&mutex_);
}
