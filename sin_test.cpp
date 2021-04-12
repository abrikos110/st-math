#include <iostream>
#include <cmath>
#include "ddouble.cpp"


static const double pi = std::atan2(0, -1);

ddouble my_sin(double z) {
    ddouble x = z;
    x -= 2*pi * int(z/2/pi);
    // x - x**3/3! + x**5/5!
    ddouble f = 1;
    ddouble px = x;
    ddouble ans = 0;
    ddouble add = 0;
    double R;
    long long i = 2;
    do {
        add = px / f;
        ans += add;
        px *= -x*x;
        f *= i * (i+1);
        i += 2;
        std::fesetround(FE_UPWARD);
        // R_n = x^n+1 / (n+1)! * (d/dx)^(n+1) f(theta x), 0 < theta < 1
        R = std::abs((px * x / f * (i-1)).max);
        // std::cerr << "    " << ans << " " << px << " " << x << " " << f <<  std::endl;
    } while (R > 1e-16);

    return ans;
}

int main() {
    std::cerr.precision(16);
    std::cout.precision(16);
    double x;
    while (std::cin >> x) {
        std::cout << my_sin(x) << " " << std::sin(x) << std::endl;
        std::cout << "  " << std::abs(my_sin(x) - std::sin(x)).max << std::endl;
    }
}
