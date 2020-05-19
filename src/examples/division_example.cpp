#include "division.h"

#include <iostream>

int main() {
    Division::Division divide       = Division::Division(Division::Fraction{100, 2});
    Division::DivisionResult result = divide.divide();
    std::cout << "division:" << result.division << std::endl;
    std::cout << "remainder:" << result.remainder << std::endl;

    return 0;
}