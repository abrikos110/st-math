#include <iostream>
#include <cmath>
#include <cfenv>
#include <algorithm>


struct ddouble {
    double min, max;

    ddouble(double min, double max)
            : min(min), max(max) {
        if (this->min > this->max) {
            std::swap(this->min, this->max);
        }
    }

    ddouble(double val = 0)
        : min(val), max(val) {}


    double sample(double r) {
        return min * r + (1-r) * max;
    }

    bool contain(ddouble oth) {
        if (min + max != min + max) {
            return true;
        }
        return min <= oth.min && oth.max <= max;
    }
    bool contain(double oth) {
        if (min + max != min + max) {
            return true;
        }
        return min <= oth && oth <= max;
    }

    double uncertainty() {
        std::fesetround(FE_UPWARD);
        return max - min;
    }


    ddouble operator-() {
        return ddouble(-max, -min);
    }

    ddouble &operator+=(ddouble oth) {
        std::fesetround(FE_DOWNWARD);
        min += oth.min;
        std::fesetround(FE_UPWARD);
        max += oth.max;
        return *this;
    }
    ddouble operator+(ddouble oth) {
        return oth += *this;
    }


    ddouble &operator-=(ddouble oth) {
        std::fesetround(FE_DOWNWARD);
        min -= oth.max;
        std::fesetround(FE_UPWARD);
        max -= oth.min;
        return *this;
    }
    ddouble operator-(ddouble oth) {
        return -(oth -= *this);
    }


    ddouble &operator*=(ddouble oth) {
        std::fesetround(FE_DOWNWARD);
        double old_min = min;
        min = std::min({min * oth.min, min * oth.max, max * oth.min, max * oth.max});
        std::fesetround(FE_UPWARD);
        max = std::max({old_min * oth.min, old_min * oth.max, max * oth.min, max * oth.max});
        return *this;
    }
    ddouble operator*(ddouble oth) {
        return oth *= *this;
    }


    ddouble &operator/=(ddouble oth) {
        if (oth.min <= 0.0 && 0.0 <= oth.max) {
            min = max = NAN;
        }
        if (oth.min < 0) {
            oth = -oth;
            std::swap(min, max);
            min = -min;
            max = -max;
        }
        std::fesetround(FE_DOWNWARD);
        min = min / (min < 0 ? oth.min : oth.max);
        std::fesetround(FE_UPWARD);
        max = max / (max < 0 ? oth.max : oth.min);
        return *this;
    }
    ddouble operator/(ddouble oth) {
        ddouble copy(*this);
        return copy /= oth;
    }
};


namespace std {
    ddouble abs(ddouble x) {
        if (x.min <= 0 && 0 <= x.max) {
            return ddouble(0, std::max(-x.min, x.max));
        }
        if (x.max <= 0) {
            return ddouble(-x.max, -x.min);
        }
        return x;
    }
}


std::ostream& operator<<(std::ostream &in, ddouble x) {
    return in << "ddouble(" << x.min << ", " << x.max << ")";
}
