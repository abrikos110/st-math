#include <iostream>
#include <random>
#include <cstdlib>
#include "ddouble.cpp"


double my_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    return std::generate_canonical<double, 16>(gen);
}


#ifndef OP
#error "Should define macro OP equal to one from *, /, -, +"
#endif
int main(int argc, char **args) {
    int n = (argc <= 1 ? 100 : strtol(args[1], nullptr, 10));

    for (int i = 0; i < n; ++i) {
        ddouble x(-0.5+my_random(), -0.5+my_random()), y(-0.5+my_random(), -0.5+my_random());
        ddouble val = x OP y;
        for (int j = 0; j < n; ++j) {
            for (auto mode : {FE_DOWNWARD, FE_TONEAREST, FE_TOWARDZERO, FE_UPWARD}) {
                std::fesetround(mode);
                double xs = x.sample(my_random());
                double ys = y.sample(my_random());
                if (!val.contain(xs OP ys)) {
                    std::cout << "BAD " << x << " " << y << " " << val << "\n    "
                            << xs << " " << ys << " " << xs OP ys << std::endl;
                }
            }
        }
    }
}
