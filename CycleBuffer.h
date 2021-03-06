#include <vector>
#include <pthread.h>

class CycleBuffer {
    public:
        CycleBuffer();
        CycleBuffer(int len);
        ~CycleBuffer();

        size_t getSize();
        size_t getReadIndex();
        size_t getWriteIndex();
        size_t getCount();
        double getValue(size_t index);
        std::vector<double> getBuffer();

        void write(double n);
    private:
        std::vector<double> buffer_;
        size_t count_;
        size_t writeIndex_;
        pthread_mutex_t mutex_;
};
