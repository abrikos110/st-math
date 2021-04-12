#include <iostream>
#include <utility>
#include <chrono>
#include <unistd.h>
#include "ddouble.cpp"


long long cur_time() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
}


template<typename T> T add(T x, T y) { return x + y; }
template<typename T> T mul(T x, T y) { return x * y; }
template<typename T> T sub(T x, T y) { return x - y; }
template<typename T> T div(T x, T y) { return x / y; }


int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << argv[0] << " " << "N" << std::endl;
        return 1;
    }
    int N = strtol(argv[1], nullptr, 10);
    double x = 1/3., y = 2/7., z = 0;
    ddouble a(1/3., 2/7.), b(4/11., 7/13.), c;
    long long t;

    for (auto &[ddop, dop] : {
            std::pair(add<ddouble>, add<double>),
            std::pair(sub<ddouble>, sub<double>),
            std::pair(mul<ddouble>, mul<double>),
            std::pair(div<ddouble>, div<double>) }) {

        for (int j = 0; j < 5; ++j) {
            t = cur_time();
            for (int i = 0; i < N; ++i) {
                z = dop(x, y);
            }
            if (j == 4) {
                std::cout << " double time: " << cur_time() - t << std::endl;
            }

            t = cur_time();
            for (int i = 0; i < N; ++i) {
                c = ddop(a, b);
            }
            if (j == 4) {
                std::cout << "ddouble time: " << cur_time() - t << std::endl;
            }
        }
    }
    return int(z * 0);
}
