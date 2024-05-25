#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>

template<typename T>
class DynamicArray {
public:
    DynamicArray() : _data{nullptr}, _len{0}, _cap{0} {}

    explicit DynamicArray(size_t capacity) : _data{new T[capacity]}, _len{0}, _cap{capacity} {}

    DynamicArray(const DynamicArray<T>& darr) {
        _data = new T[darr._cap];
        std::memcpy(_data, darr._data, darr._len*sizeof(T));
        _len = darr._len;
        _cap = darr._cap;
    }

    DynamicArray& operator=(const DynamicArray<T>& rhs) {
        DynamicArray<T> tmp(rhs);
        inner_swap(tmp);
        return *this;
    }

    ~DynamicArray() {
        delete[] _data;
        _len = 0;
        _cap = 0;
    }

    T& operator[](size_t idx) {
        if (idx >= _len) {
            throw "out of range exception";
        }
        return _data[idx];
    }

    bool empty() const {
        return _len == 0;
    }

    size_t capacity() const {
        return _cap;
    }

    size_t size() const {
        return _len;
    }

    T* data() {
        return _data;
    }

    T& front() {
        if (!_len) {
            throw "out of range exception";
        }
        return _data[0];
    }

    T& back() {
        if (!_len) {
            throw "out of range exception";
        }
        return _data[_len-1];
    }

    void push_back(const T& value) {
        if (_len == _cap) {
            enlarge(_cap*2+1);
        }
        _data[_len++] = value;
    }

    void pop_back() {
        if (_len > 0) {
            --_len;
        }
    }

private:
    void enlarge(size_t capacity) {
        if (_cap < capacity) {
            T* tmp = new T[capacity];
            std::memcpy(tmp, _data, _len*sizeof(T));
            delete[] _data;
            _cap = capacity;
            _data = tmp;
        }
    }

    void inner_swap(DynamicArray<T>& rhs) {
        std::swap(_data, rhs._data);
        std::swap(_len, rhs._len);
        std::swap(_cap, rhs._cap);
    }

private:
    T* _data;
    size_t _len;
    size_t _cap;
};

template<typename T>
void showInfo(const DynamicArray<T>& darr) {
    std::cout << "capacity: " << darr.capacity() << ", size: " << darr.size() << std::endl;
}


int main() {
    DynamicArray<int> da1;
    showInfo(da1);
    DynamicArray<int> da2(10);
    showInfo(da2);
    std::cout << "Is da1 empty? " << da1.empty() << std::endl;
    std::cout << "Is da2 empty? " << da2.empty() << std::endl;

    da1.push_back(1);
    showInfo(da1);
    da2.push_back(1);
    showInfo(da2);

    da1.push_back(1);
    showInfo(da1);

    da1.push_back(2);
    da1.push_back(3);
    showInfo(da1);
    std::cout << "da1-front: " << da1.front() << ", da1-back: " << da1.back() << std::endl;

    da2 = da1;
    showInfo(da1);
    showInfo(da2);
    std::cout << "da1-front: " << da2[0] << ", da1-back: " << da2[da2.size()-1] << std::endl;
    return 0;
}
