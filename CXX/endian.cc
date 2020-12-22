#include <cstdio>
#include <cinttypes>
#include <stdexcept>

void checkEndian() {
    uint32_t num = 0x04030201;
    unsigned char* cp = reinterpret_cast<unsigned char*>(&num);

    if (*cp == 1) {
        printf("little-endian\n");
    } else if (*cp == 4) {
        printf("big-endian\n");
    } else {
        printf("who knows?\n");
    }
}

template<typename T, size_t N>
struct SwapBytes {
    T operator()(T val) {
        throw std::out_of_range("data size");
    }
};

template<typename T>
struct SwapBytes<T, 1> {
    T operator()(T val) {
        return val;
    }
};

template<typename T>
struct SwapBytes<T, 2> {
    T operator()(T val) {
        return ((((val) >> 8) & 0xff) | (((val) & 0xff) << 8));
    }
};

template<typename T>
struct SwapBytes<T, 4> {
    T operator()(T val) {
        return ((((val) & 0xff000000) >> 24) |
                (((val) & 0x00ff0000) >>  8) |
                (((val) & 0x0000ff00) <<  8) |
                (((val) & 0x000000ff) << 24));
    }
};

template<typename T>
struct SwapBytes<T, 8> {
    T operator()(T val) {
        return ((((val) & 0xff00000000000000ull) >> 56) |
                (((val) & 0x00ff000000000000ull) >> 40) |
                (((val) & 0x0000ff0000000000ull) >> 24) |
                (((val) & 0x000000ff00000000ull) >> 8 ) |
                (((val) & 0x00000000ff000000ull) << 8 ) |
                (((val) & 0x0000000000ff0000ull) << 24) |
                (((val) & 0x000000000000ff00ull) << 40) |
                (((val) & 0x00000000000000ffull) << 56));
    }
};

template<typename T>
struct DoByteSwap {
    T operator()(T val) {
        return SwapBytes<T, sizeof(T)>()(val);
    }
};

template<typename T>
T byteSwap(T val) {
    return DoByteSwap<T>()(val);
}

int main() {
    checkEndian();

    uint8_t  num1 = 0x01;
    uint16_t num2 = 0x0102;
    uint32_t num3 = 0x01020304;
    uint64_t num4 = 0x0102030405060708;

    printf("%#X -> %#X\n",     num1, byteSwap(num1));
    printf("%#X -> %#X\n",     num2, byteSwap(num2));
    printf("%#X -> %#X\n",     num3, byteSwap(num3));
    printf("%#llX -> %#llX\n", num4, byteSwap(num4));

    return 0;
}