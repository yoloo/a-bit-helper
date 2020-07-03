#include <cmath>
#include <cfloat>
#include <map>
#include <list>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

//auto -- deduction of a type from an initializer
namespace Test1 {
    //c++11
    template<class T>
    void printall_1(const std::vector<T>& v) {
        for (auto it = v.begin(); it != v.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    //c++98
    template<class T>
    void printall_2(const std::vector<T>& v) {
        for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)  {
            std::cout << *it << std::endl;
        }
    }

    void test() {
        std::vector<std::string> list1 = {"hello", "world", "Haha"};
        printall_1(list1);
        printall_2(list1);

        std::vector<size_t> list2 = {1, 2, 3, 4, 5, 6};
        printall_1(list2);
        printall_2(list2);
    }
}

//range-for statement
namespace Test2 {
    void test() {
        std::vector<size_t> list1 = {1, 2, 3, 4, 5, 6};
        for (auto item : list1) {
            std::cout << item << std::endl;
        }
        for (auto& item : list1) {
            if (item % 2) item *= 2;

            std::cout << item << std::endl;
        }

        std::map<size_t, std::string> list2 = {{1, "yoloo"}, {2, "family"}};
        for (auto item : list2) {
            std::cout << "first=" << item.first << std::endl;
            std::cout << "second=" << item.second << std::endl;
        }

        std::list<std::string> list3 = {"bejing", "shanghai", "guangzhou"};
        for (auto item : list3) {
            std::cout << item << std::endl;
        }

        std::string str("sohappy");
        for (auto item : str) {
            std::cout << item << std::endl;
        }

        size_t array[4] = {100, 200, 300, 400};
        for (auto item : array) {
            std::cout << item << std::endl;
        }

        for (auto item : {1, 2, 3, 4, 5, 6}) {
            std::cout << item << std::endl;
        }
    }
}

// right-angle brackets
namespace Test3 {
    void test() {
        //c++11
        std::map<size_t, std::vector<std::string>> list1;

        //c++98 - need left space between the two >s.
        std::map<size_t, std::vector<std::string> > list2;
    }
}

// control of defaults: default and delete
namespace Test4 {
    class X {
    public:
        //...
        X() = default;

        X& operator=(const X&) = delete;
        X(const X&) = delete;

        ~X() = delete;
    };

    void test() {
        //X x1;
        //X x2 = x1;
    }
}

// control of defaults: move and copy
namespace Test5 {
    template<class T>
    class Handle {
    private:
        T* p;
    public:
        Handle(T* pp) : p(pp) {}
        ~Handle() { delete p; }

        Handle(Handle&& h) {
            p = h.p;
            h.p = nullptr;
        }
        Handle& operator=(Handle&& h) {
            delete p;
            p   = h.p;
            h.p = nullptr;

            return *this;
        }

        Handle(const Handle&) = delete;
        Handle& operator=(const Handle&) = delete;
    };

    void test() {
        Handle<int> h1(new int(4));
    }
}

// enum class - scoped and strongly typed enums
namespace Test6 {
    //c++98
    enum Alert {
        green = 1,
        yellow,
        orange,
        red
    };

    //c++11
    enum class Color {
        green = 1,
        yellow,
        orange,
        red
    };

    enum class TraffucLight : char {
        red = 1,
        yellow,
        green
    };

    // support forward declaration
    enum class ColorValue : char;
    void foolbar(ColorValue* p);

    enum class ColorValue : char {
        red = 1,
        yellow,
        green
    };

    void test() {
        //Alert a1 = 7;
        Alert a2  = green;
        Alert a3  = Alert::red;
        int a4 = orange;
        std::cout << "a2=" << a2 << std::endl;
        std::cout << "a3=" << a3 << std::endl;
        std::cout << "a4=" << a4 << std::endl;
        std::cout << "sizeof(enum Alert)=" << sizeof(a3) << std::endl;

        //Color b1 = 7;
        //Color b2 = green;
        Color b3 = Color::red;
        if (b3 != Color::green) std::cout << "It's not green" << std::endl;
        //if (b3 != 1) std::cout << "It's not green" << std::endl;
        //int b4 = Color::orange;
        std::cout << "sizeof(enum class Color)=" << sizeof(b3) << std::endl;

        TraffucLight c1 = TraffucLight::yellow;
        std::cout << "sizeof(enum class TraffucLight : char)=" << sizeof(c1) << std::endl;
    }
}

// constexpr -- generalized and guaranteed constant expression
namespace Test7 {

}

// decltype -- the type of an expression
namespace Test8 {
    template<typename T1, typename T2>
    auto add(const T1& t1, const T2& t2) -> decltype(t1 + t2) {
        return t1 + t2;
    }

    auto sub(const int& t1, const int& t2) -> decltype(t1 - t2) {
        return t1 - t2;
    }

    void test() {
        std::string str1("hello");
        std::string str2(" world");
        std::cout << add(str1, str2) << std::endl;

        size_t a    = 1;
        size_t aaaa = 4;
        std::cout << add(a, aaaa) << std::endl;
    }
}

// Initializer lists
namespace Test9 {
    void test() {
        std::map<std::vector<std::string>, std::vector<int>> years;
        years.insert({{"bejing", "shanghai"}, {1950, 1975}});

        std::vector<int> list = {7, 8, 9};
        list                  = {9};
    }
}

// preventing narrowing
namespace Test10 {
    void test() {
        //int x0       = 7.3;
        //int x1{7.3};
        //int x2       = {7.3};

        //char x4 = 7;
        //char x5 = 976;
        //char x6 = {975};

        //std::cout << "x0=" << x0 << "; x1=" << x1 << "; x2=" << x2 << std::endl;
        //std::cout << "x4=" << x4 << " x5=" << x5 << " x6=" << x6 << std::endl;
    }
}

// float and double range->FLT_EPSILON/DBL_EPSILON
namespace Test11 {
    void test() {
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
}

int main() {
    //Test1::test();
    //Test2::test();
    //Test3::test();
    //Test4::test();
    //Test5::test();
    //Test6::test();
    //Test8::test();
    //Test9::test();
    Test11::test();

    return 0;
}