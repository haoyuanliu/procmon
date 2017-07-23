#include "CycleBuffer.h"
#define DFBUFLEN 1024

CycleBuffer::CycleBuffer() : buffer_(DFBUFLEN){
    count_ = 0;
    writeIndex_ = 0;
    pthread_mutex_init(&mutex_, NULL);
}

CycleBuffer::CycleBuffer(int len) : buffer_(len){
    count_ = 0;
    writeIndex_ = 0;
    pthread_mutex_init(&mutex_, NULL);
}

CycleBuffer::~CycleBuffer() {
    pthread_mutex_destroy(&mutex_);
}

size_t CycleBuffer::getSize() {
    return buffer_.size();
}

size_t CycleBuffer::getCount() {
    return count_;
}

size_t CycleBuffer::getReadIndex() {
    pthread_mutex_lock(&mutex_);
    size_t res;
    if (count_ < getSize()) {
        res = 0;
    } else {
        res = writeIndex_ + 1;
    }
    pthread_mutex_unlock(&mutex_);
    return res;
}

size_t CycleBuffer::getWriteIndex() {
    pthread_mutex_lock(&mutex_);
    size_t res = writeIndex_;
    pthread_mutex_unlock(&mutex_);
    return res;
}

double CycleBuffer::getValue(size_t index) {
    pthread_mutex_lock(&mutex_);
    double res = buffer_[index];
    pthread_mutex_unlock(&mutex_);
    return res;
}

void CycleBuffer::write(double n) {
    pthread_mutex_lock(&mutex_);
    buffer_[writeIndex_] = n;
    count_++;
    if (++writeIndex_ == getSize()) {
        writeIndex_ = 0;
    }
    pthread_mutex_unlock(&mutex_);
}
