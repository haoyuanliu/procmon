#include "CycleBuffer.h"

CycleBuffer::CycleBuffer() {
    pthread_mutex_init(&mutex_, NULL);
}

CycleBuffer::CycleBuffer(int len) : buffer_(len){
    pthread_mutex_init(&mutex_, NULL);
}

CycleBuffer::~CycleBuffer() {
    pthread_mutex_destroy(&mutex_);
}

size_t CycleBuffer::getSize() {
    return buffer_.size();
}

void CycleBuffer::write(double n) {
    pthread_mutex_lock(&mutex_);
    buffer_[writeIndex_] = n;
    if (++writeIndex_ == getSize()) {
        writeIndex_ = 0;
    }
    pthread_mutex_unlock(&mutex_);
}
