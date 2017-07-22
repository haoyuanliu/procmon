#include "CycleBuffer.h"
#define DFBUFLEN 1024

CycleBuffer::CycleBuffer() : buffer_(DFBUFLEN){
    readIndex_ = 0;
    writeIndex_ = 0;
    pthread_mutex_init(&mutex_, NULL);
}

CycleBuffer::CycleBuffer(int len) : buffer_(len){
    readIndex_ = 0;
    writeIndex_ = 0;
    pthread_mutex_init(&mutex_, NULL);
}

CycleBuffer::~CycleBuffer() {
    pthread_mutex_destroy(&mutex_);
}

size_t CycleBuffer::getSize() {
    return buffer_.size();
}

size_t CycleBuffer::getRead() {
    pthread_mutex_lock(&mutex_);
    size_t res = readIndex_;
    pthread_mutex_unlock(&mutex_);
    return res;
}

size_t CycleBuffer::getWrite() {
    pthread_mutex_lock(&mutex_);
    size_t res = writeIndex_;
    pthread_mutex_unlock(&mutex_);
    return res;
}

void CycleBuffer::write(double n) {
    pthread_mutex_lock(&mutex_);
    buffer_[writeIndex_] = n;
    if (++writeIndex_ == getSize()) {
        writeIndex_ = 0;
    }
    pthread_mutex_unlock(&mutex_);
}
