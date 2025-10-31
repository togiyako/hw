#include <math.h>
#include "calc.h"

double square_root(double n) {
    if (n < 0) {
        return 0.0 / 0.0;
    }
    return sqrt(n);
}

