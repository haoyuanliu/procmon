#include <vector>
#include <pthread.h>

class CycleBuffer {
    public:
        CycleBuffer();
        CycleBuffer(int len);
        ~CycleBuffer();
        size_t getSize();
        void write(double n);
    private:
        std::vector<double> buffer_;
        size_t readIndex_;
        size_t writeIndex_;
        pthread_mutex_t mutex_;
};
