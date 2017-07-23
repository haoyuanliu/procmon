#include "TimeStamp.h"

std::string TimeStamp::toString() {
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microSeconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    //snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microSeconds);
    snprintf(buf, sizeof(buf)-1, "%lld %lld", seconds, microSeconds);
    return std::string(buf);
}

TimeStamp TimeStamp::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return TimeStamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string TimeStamp::toFormattedString(bool showMicroSeconds) {
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    if (showMicroSeconds) {
        int microSeconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microSeconds);
    } else {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }

    return std::string(buf);
}

int64_t TimeStamp::getSeconds() {
    return microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
}

int64_t TimeStamp::getMicroSeconds() {
    return microSecondsSinceEpoch_;
}

TimeStamp TimeStamp::addTime(TimeStamp time, double seconds) {
    int64_t delta = static_cast<int64_t>(seconds * kMicroSecondsPerSecond);
    return TimeStamp(time.getMicroSeconds() + delta);
}

double TimeStamp::timeDiff(TimeStamp high, TimeStamp low) {
    int64_t diff = high.getMicroSeconds() - low.getMicroSeconds();
    return static_cast<double>(diff / kMicroSecondsPerSecond);
}
