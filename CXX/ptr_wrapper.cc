#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>

// PtrWrapper Observer
template<typename T> class PtrWrapperObs;

// Control Block
struct CBlock {
    CBlock() : shared_cnt{1}, weak_cnt{0} {}
    int shared_cnt;
    int weak_cnt;
};

template<typename T>
class PtrWrapper {
public:
    PtrWrapper() = default;

    explicit PtrWrapper(T* data) : _data{data} {
        if (_data) {
            _cblock = new CBlock();
        }
    }

    PtrWrapper(const PtrWrapper<T>& ptrwrap) : _data(ptrwrap._data), _cblock(ptrwrap._cblock) {
        if (_cblock) {
            ++_cblock->shared_cnt;
        }
    }

    explicit PtrWrapper(const PtrWrapperObs<T>& obs) : _data(obs._data), _cblock(obs._cblock) {
        if (_cblock) {
            ++_cblock->shared_cnt;
        }
    }

    PtrWrapper(PtrWrapper<T>&& ptrwrap) {
        swap(ptrwrap);
        ptrwrap.reset();
    }

    ~PtrWrapper() {
        dec();
    }

    PtrWrapper<T>& operator=(const PtrWrapper<T>& rhs) {
        PtrWrapper<T> tmp(rhs);
        swap(tmp);
        return *this;
    }

    explicit operator bool() const {
        return _data != nullptr;
    }

    T* get() const {
        return _data;
    }

    T* operator->() const {
        return _data;
    }

    T& operator*() const {
        return *_data;
    }

    void reset() {
        dec();
        _data = nullptr;
        _cblock = nullptr;
    }

private:
    void swap(PtrWrapper<T>& rhs) {
        std::swap(_data, rhs._data);
        std::swap(_cblock, rhs._cblock);
    }

    void dec() {
        if (!_cblock) { return; }
        --_cblock->shared_cnt;
        if (_cblock->shared_cnt <= 0) {
            delete _data;
            _data = nullptr;
            if (_cblock->weak_cnt <= 0) {
                delete _cblock;
                _cblock = nullptr;
            }
        }
    }

    friend class PtrWrapperObs<T>;
private:
    T* _data = nullptr;
    CBlock* _cblock = nullptr;
};


template<typename T>
class PtrWrapperObs {
public:
    PtrWrapperObs() = default;

    PtrWrapperObs(const PtrWrapperObs<T>& obs) : _data(obs._data), _cblock(obs._cblock) {
        if (_cblock) {
            ++_cblock->weak_cnt;
        }
    }

    PtrWrapperObs(const PtrWrapper<T>& ptrwrap) : _data(ptrwrap._data), _cblock(ptrwrap._cblock) {
        if (_cblock) {
            ++_cblock->weak_cnt;
        }
    }

    PtrWrapperObs(PtrWrapperObs<T>&& obs) {
        swap(obs);
        obs.reset();
    }

    ~PtrWrapperObs() {
        dec();
    }

    PtrWrapperObs& operator=(const PtrWrapperObs<T>& rhs) {
        PtrWrapperObs<T> tmp(rhs);
        swap(tmp);
        return *this;
    }

    PtrWrapper<T> lock() const {
        return expired() ? PtrWrapper<T>() : PtrWrapper<T>(*this);
    }

    bool expired() const {
        return !_cblock || _cblock->shared_cnt <= 0;
    }

    void reset() {
        dec();
        _data = nullptr;
        _cblock = nullptr;
    }

private:
    void swap(PtrWrapperObs& rhs) {
        std::swap(_data, rhs._data);
        std::swap(_cblock, rhs._cblock);
    }

    void dec() {
        if (!_cblock) { return; }
        --_cblock->weak_cnt;
        if (_cblock->shared_cnt <= 0 && _cblock->weak_cnt <= 0) {
            delete _cblock;
            _cblock = nullptr;
        }
    }

    friend class PtrWrapper<T>;
private:
    T* _data = nullptr;
    CBlock* _cblock = nullptr;
};

int main() {
    {
        PtrWrapper<int> sp1;
        assert(sp1.get() == nullptr);
    }

    {
        PtrWrapper<int> sp1(new int(1));
        assert(*sp1 == 1);
        assert(*sp1.get() == 1);
    }

    {
        PtrWrapper<int> sp1(new int(1));
        PtrWrapper<int> sp2(sp1);
        assert(*sp1 == 1);
        assert(*sp2 == 1);
    }

    {
        PtrWrapper<int> sp1(new int(1));
        sp1.reset();
        assert(sp1.get() == nullptr);
    }

    {
        PtrWrapper<int> sp1(new int(1));
        PtrWrapper<int> sp2(std::move(sp1));
        assert(sp1.get() == nullptr);
        assert(!sp1);
        assert(*sp2 == 1);
    }

    {
        PtrWrapperObs<int> wp1;
        assert(!wp1.lock());
        assert(wp1.expired());
    }

    {
        PtrWrapper<int> sp1(new int(1));
        PtrWrapperObs<int> wp1(sp1);
        PtrWrapperObs<int> wp2(wp1);
        assert(wp1.lock());
        assert(wp2.lock());
    }

    {
        PtrWrapper<int> sp1(new int(1));
        PtrWrapper<int> sp2(new int(2));
        PtrWrapperObs<int> wp1(sp1);
        PtrWrapperObs<int> wp2(sp2);
        PtrWrapperObs<int> wp3;
        wp3 = wp1;
        assert(wp1.lock());
        assert(wp3.lock());
        wp3 = wp2;
        assert(wp2.lock());
        assert(wp3.lock());
    }

    return 0;
}
