#include "division.h"

namespace Division
{
    DivisionResult Division::divide()
    {
        if (!fraction.denominator)
            throw DivisionByZero();

        DivisionResult result = DivisionResult
        {
            fraction.numerator / fraction.denominator,
            fraction.numerator % fraction.denominator
        };

        return result;
    }
}
