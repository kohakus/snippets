#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>

class CrossPrint {
public:
    CrossPrint(int num) : _topv(num), _cross{false} {}

    void run() {
        printf("CrossPrint run() ...\n");
        std::thread ta([this]{ this->taskA(); });
        std::thread tb([this]{ this->taskB(); });
        ta.join();
        tb.join();
    }
private:
    void taskA() {
        for (int i = 1; i <= _topv; i++) {
            std::unique_lock<std::mutex> ulk(_lk);
            _cv.wait(ulk, [&]{ return this->_cross; });
            printf("taskA echo :%d\n", i);
            _cross = !_cross;
            _cv.notify_one();
        }
    }

    void taskB() {
        for (int i = 1; i <= _topv; i++) {
            std::unique_lock<std::mutex> ulk(_lk);
            _cv.wait(ulk, [&]{ return !this->_cross; });
            printf("taskB echo :%d\n", i);
            _cross = !_cross;
            _cv.notify_one();
        }
    }
private:
    std::mutex _lk;
    std::condition_variable _cv;
    bool _cross;
    int _topv;
};

class RoundPrint {
public:
    RoundPrint(int num) : _topv{num}, _cur_num{0}, _round{false} {}

    void run() {
        printf("RoundPrint run() ...\n");
        _cur_num = 0;
        std::thread ta([this]{ this->taskA(); });
        std::thread tb([this]{ this->taskB(); });
        ta.join();
        tb.join();
    }
private:
    void taskA() {
        while(true) {
            std::unique_lock<std::mutex> ulk(_lk);
            _cv.wait(ulk, [&]{ return this->_round; });

            if (_cur_num < _topv) {
                printf("taskA echo :%d\n", ++_cur_num);
            }
            _round = !_round;
            _cv.notify_one();
            if (_cur_num >= _topv) {
                break;
            }
        }
    }

    void taskB() {
        while(true) {
            std::unique_lock<std::mutex> ulk(_lk);
            _cv.wait(ulk, [&]{ return !this->_round; });
            if (_cur_num < _topv) {
                printf("taskB echo :%d\n", ++_cur_num);
            }
            _round = !_round;
            _cv.notify_one();
            if (_cur_num >= _topv) {
                break;
            }
        }
    }
private:
    std::mutex _lk;
    std::condition_variable _cv;
    bool _round;
    int _topv;
    int _cur_num;
};

int main() {
    CrossPrint cp(10);
    cp.run();
    printf("\n");

    RoundPrint rp(10);
    rp.run();

    return 0;
}
