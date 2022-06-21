#include <iostream>
#include <cmath>
#include <cfenv>
#include <algorithm>


namespace {
    void r_up() {
        // round up
        std::fesetround(FE_UPWARD);
    }

    void r_down() {
        // round down
        std::fesetround(FE_DOWNWARD);
    }
}

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
        if (!std::isfinite(min) || !std::isfinite(max)) {
            return true;
        }
        return min <= oth.min && oth.max <= max;
    }
    bool contain(double oth) {
        if (!std::isfinite(min) || !std::isfinite(max)) {
            return true;
        }
        return min <= oth && oth <= max;
    }

    double uncertainty() {
        r_up();
        return max - min;
    }


    ddouble operator-() {
        return ddouble(-max, -min);
    }

    ddouble &operator+=(ddouble oth) {
        r_down();
        min += oth.min;

        r_up();
        max += oth.max;
        return *this;
    }
    ddouble operator+(ddouble oth) {
        return oth += *this;
    }


    ddouble &operator-=(ddouble oth) {
        r_down();
        min -= oth.max;

        r_up();
        max -= oth.min;
        return *this;
    }
    ddouble operator-(ddouble oth) {
        return -(oth -= *this);
    }

    ddouble &operator*=(ddouble oth) {
        double old_min = min;
        if (oth.max <= 0) {
            if (max <= 0) {
                r_down();
                min = max * oth.max;

                r_up();
                max = old_min * oth.min;
            }
            else if (min <= 0 && 0 <= max) {
                r_down();
                min = max * oth.min;

                r_up();
                max = old_min * oth.min;
            }
            else /* 0 <= min */ {
                r_down();
                min = max * oth.min;

                r_up();
                max = old_min * oth.max;
            }
        }
        else if (oth.min <= 0 && 0 <= oth.max) {
            if (max <= 0) {
                r_down();
                min = min * oth.max;

                r_up();
                max = old_min * oth.min;
            }
            else if (min <= 0 && 0 <= max) {
                r_down();
                min = std::min(min * oth.max, max * oth.min);

                r_up();
                max = std::max(old_min * oth.min, max * oth.max);
            }
            else /* 0 <= min */ {
                r_down();
                min = max * oth.min;

                r_up();
                max = max * oth.max;
            }
        }
        else /* 0 <= oth.min */ {
            if (max <= 0) {
                r_down();
                min = min * oth.max;

                r_up();
                max = max * oth.min;
            }
            else if (min <= 0 && 0 <= max) {
                r_down();
                min = min * oth.max;

                r_up();
                max = max * oth.max;
            }
            else /* 0 <= min */ {
                r_down();
                min = min * oth.min;

                r_up();
                max = max * oth.max;
            }
        }
        return *this;
    }
#if 0
    ddouble &operator*=(ddouble oth) {
        r_down();
        double old_min = min;
        min = std::min({min * oth.min, min * oth.max, max * oth.min, max * oth.max});
        r_up();
        max = std::max({old_min * oth.min, old_min * oth.max, max * oth.min, max * oth.max});
        return *this;
    }
#endif
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

        r_down();
        min = min / (min < 0 ? oth.min : oth.max);

        r_up();
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

    bool isfinite(ddouble x) {
        return isfinite(x.min) && isfinite(x.max);
    }
}


std::ostream& operator<<(std::ostream &in, ddouble x) {
    return in << "ddouble(" << x.min << ", " << x.max << ")";
}
