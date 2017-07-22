#include "StatData.h"

void StatData::parse(const char* startStat) {
    std::istringstream iss(startStat);
    iss >> pid >> command >> state;
    iss >> ppid >> pgrp >> session >> tty_nr >> tgpid >> flags;
    iss >> minflt >> cminflt >> majflt >> cmajflt;
    iss >> utime >> stime >> cutime >> cstime;
    iss >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    iss >> vsize >> rss >> rsslim;
}
