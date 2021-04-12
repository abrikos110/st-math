#include <iostream>
#include <utility>
#include <random>
#include <cstdlib>
#include "ddouble.cpp"


double my_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    return std::generate_canonical<double, 16>(gen);
}


template<typename T> T add(T x, T y) { return x + y; }
template<typename T> T mul(T x, T y) { return x * y; }
template<typename T> T sub(T x, T y) { return x - y; }
template<typename T> T div(T x, T y) { return x / y; }


int main(int argc, char **args) {
    int n = (argc <= 1 ? 100 : strtol(args[1], nullptr, 10));

    for (auto &[ddop, dop] : {
            std::pair(add<ddouble>, add<double>),
            std::pair(sub<ddouble>, sub<double>),
            std::pair(mul<ddouble>, mul<double>),
            std::pair(div<ddouble>, div<double>) }) {

        for (int i = 0; i < n; ++i) {
            ddouble x(-0.5+my_random(), -0.5+my_random()),
                    y(-0.5+my_random(), -0.5+my_random());
            ddouble val = ddop(x, y);

            for (int j = 0; j < n; ++j) {
                for (auto mode : {FE_DOWNWARD, FE_TONEAREST, FE_TOWARDZERO, FE_UPWARD}) {
                    std::fesetround(mode);
                    double xs = x.sample(my_random());
                    if (my_random() > 0.9) {
                        xs = x.sample(my_random() > 0.5);
                    }
                    double ys = y.sample(my_random());
                    if (my_random() > 0.9) {
                        ys = y.sample(my_random() > 0.5);
                    }
                    if (!val.contain(dop(xs, ys))) {
                        std::cout << "BAD " << x << " " << y << " " << val << "\n    "
                                << xs << " " << ys << " " << dop(xs, ys) << std::endl;
                    }
                }
            }
        }
    }
}
