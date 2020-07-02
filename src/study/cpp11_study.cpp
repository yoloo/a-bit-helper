#include <iostream>
#include <cmath>
#include <cfloat>

void test_epsilon() {
    double a = 0.5;
    if (0.5 == a) {
        std::cout << "Yes, direct compare, it equals" << std::endl;
    } else {
        std::cout << "No, direct compare, not equal" << std::endl;
    }

    double b = sin(3.1415926272829 / 6);
    if (0.5 == b) {
        std::cout << "Yes, direct compare, it equals" << std::endl;
    } else {
        std::cout << "No, direct compare, not equal" << std::endl;
    }

    if (fabs(b - 0.5) < FLT_EPSILON) {
        std::cout << "Yes, use FLT_EPSILON, it equals" << std::endl;
    } else {
        std::cout << "No, use FLT_EPSILON, not equal" << std::endl;
    }

    if (fabs(b - 0.5) < DBL_EPSILON) {
        std::cout << "Yes, use DBL_EPSILON, it equals" << std::endl;
    } else {
        std::cout << "No, use DBL_EPSILON, not equal" << std::endl;
    }
}

int main() {
    test_epsilon();

    return 0;
}