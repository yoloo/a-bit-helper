//
// Created by zhaoyongle on 2020/5/19.
//

#ifndef STUDY_A_BIT_DIVISION_H
#define STUDY_A_BIT_DIVISION_H

static const char *const DIVISION_BY_ZERO_MESSAGE = "Division by zero is illegal";

#include <iostream>
#include <stdexcept>

using namespace std;

namespace Division {
    // 除0
    class DivisionByZero : public exception
    {
    public:
        virtual const char *what() const throw()
        {
            return DIVISION_BY_ZERO_MESSAGE;
        }
    };

    struct Fraction
    {
        long long numerator;
        long long denominator;
    };

    struct DivisionResult
    {
        long long division;
        long long remainder;

        friend bool operator== (const DivisionResult& lhs, const DivisionResult& rhs)
        {
            return lhs.division == rhs.division ? lhs.remainder < rhs.remainder : lhs.division < rhs.division;
        }
    };

    class Division
    {
    public:
        explicit Division(Fraction fraction)
        {
            this->fraction = fraction;
        }

        ~Division() {};

        DivisionResult divide();

    protected:
        Fraction       fraction;
        DivisionResult result;
    };
}

#endif //STUDY_A_BIT_DIVISION_H
