#include "calc.h"

double division(double a, double b) {
    if (b == 0) {
        return 0.0 / 0.0;
    }
    return a / b;
}

