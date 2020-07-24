#include <iostream>

#include <boost/date_time/gregorian/gregorian.hpp>

int main() {
    boost::gregorian::date d{2020, 07, 24};
    std::cout << d.year() << '\n';
    std::cout << d.month() << '\n';
    std::cout << d.day() << '\n';
    std::cout << d.day_of_week() << '\n';
    std::cout << d.end_of_month() << '\n';

    d = boost::gregorian::day_clock::universal_day();
    std::cout << d.year() << '\n';
    std::cout << d.month() << '\n';
    std::cout << d.day() << '\n';

    d = boost::gregorian::date_from_iso_string("20140131");
    std::cout << d.year() << '\n';
    std::cout << d.month() << '\n';
    std::cout << d.day() << '\n';

    return 0;
}