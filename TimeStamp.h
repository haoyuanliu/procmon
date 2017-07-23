#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <string>
#define kMicroSecondsPerSecond 1000*1000

class TimeStamp {
    public:
        TimeStamp() : microSecondsSinceEpoch_(0) {}
        TimeStamp(int64_t microSeconds) : microSecondsSinceEpoch_(microSeconds) {}
        static TimeStamp now();
        std::string toString();
        std::string toFormattedString(bool showMicroSeconds = true);
        int64_t getSeconds();
        int64_t getMicroSeconds();
        TimeStamp addTime(TimeStamp time, double seconds);
        double timeDiff(TimeStamp high, TimeStamp low);
    private:
        int64_t microSecondsSinceEpoch_;
};
