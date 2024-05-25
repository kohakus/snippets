#include <iostream>
#include <string>
#include <utility>

void ovfunc(std::string& param) {
    std::cout << "std::string& version" << std::endl;
}

void ovfunc(std::string&& param) {
    std::cout << "std::string&& version" << std::endl;
}

template<typename T>
void forwardR(T&& param) {
    ovfunc(std::forward<T>(param));
}

template<typename T>
void plainR(T&& param) {
    ovfunc(param);
}

int main() {
    std::string pes;

    ovfunc(pes); // std::string& version
    ovfunc(std::move(pes)); // std::string&& version

    forwardR(pes); // std::string& version
    forwardR(std::move(pes)); // std::string&& version

    plainR(pes); // std::string& version
    plainR(std::move(pes)); // std::string& version
}
