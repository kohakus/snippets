#ifndef __WAITGROUP_H__
#define __WAITGROUP_H__

#include <mutex>
#include <atomic>
#include <thread>

class WaitGroup {
public:
    WaitGroup() : _cnt{0} {}

    void add(int count) {
        _cnt += count;
    }

    void done() {
        int f = _cnt -= 1;
        if (f <= 0) {
            _cv.notify_all();
        }
    }

    void wait() {
        std::unique_lock<std::mutex> ulk(_lk);
        _cv.wait(ulk, [this]{ return this->_cnt.load() <= 0; });
    }
private:
    std::atomic<int> _cnt;
    std::mutex _lk;
    std::condition_variable _cv;
};

#endif // __WAITGROUP_H__
