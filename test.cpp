#include "logger.h"
#include "pub_sub.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <thread>
#include <time.h>
#include <utility>
#include <vector>

// template <typename T>
// T returnWhenTrue(T val, bool flag)
// {
//     if (flag)
//         return val;
//     else
//         return 0;
// }

bool KILL_THREAD = false;

void wait2SecondsPrintHi()
{
    while (true) {
        if (KILL_THREAD) {
            return;
        }
        std::cout << "hi" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

template <typename>
struct is_std_vector : std::false_type { };

template <typename T>
struct is_std_vector<std::vector<T>> : std::true_type { };

template <typename T>
class Adder {
public:
    double add(double o1, T o2, std::true_type t)
    {
        return std::accumulate(o2.begin(), o2.end(), o1);
    }
    double add(double o1, T o2, std::false_type t)
    {
        return o1 + o2;
    }
    double add(double o1, T o2)
    {
        return add(o1, o2, is_std_vector<T> {});
    }
};

enum TestEnum {
    TestA,
    TestB,
    TestC
};

int main()
{
    std::tuple<TestEnum, int, int> t;
    t = { TestA, 1, 2 };
    std::cout << std::get<0>(t) << std::endl;
    std::cout << std::get<1>(t) << std::endl;
    std::cout << std::get<2>(t) << std::endl;
    t = { TestC, 5, 7 };
    std::cout << std::get<0>(t) << std::endl;
    std::cout << std::get<1>(t) << std::endl;
    std::cout << std::get<2>(t) << std::endl;
    TestEnum te;
    int a, b;
    std::tie(te, a, b) = t;
    std::cout << te << a << b << std::endl;
    return 0;

    // using namespace casino::pub_sub;
    // Event e1 { 0, UNKNOWN, "", 1, 2 };

    // continue testing this, then upgrade pub sub
    // then upgrade betting strategy
    // then upgrade others
    // remove my money mgmt from gambler

    // EventType e1(0, UNKNOWN, "", 1, 2);

    // bool a = 0;
    // bool b = 1;
    // std::vector<bool> vb { 0, 1, 1, 1, 1, 1, 0 };
    // std::vector<int> vi { 1, 2, 3, 4 };
    // Adder<bool> boolAdder;
    // Adder<std::vector<bool>> vectorAdderBool;
    // Adder<std::vector<int>> vectorAdderInt;

    // std::cout << boolAdder.add(a, b) << '\n';
    // // boolAdder.add(a, v);

    // // vectorAdder.add(a, b);
    // std::cout << vectorAdderBool.add(a, vb) << '\n';

    // std::cout << vectorAdderInt.add(1, vi) << '\n';

    // return 0;

    //    double s = 0;
    //    for (double val : v) {
    //        s += val;
    //    }
    //    std::cout << s << '\n';
    //    return 0;
    //
    //    double a = 0.0;
    //    double b = 0.1;
    //    double c = 1.0;
    //    double d = 1.1;
    //    bool A = 0;
    //    bool B = 1;
    //
    //    std::cout << (a && B) << std::endl;
    //    std::cout << (a || B) << std::endl;
    //    std::cout << (A && c) << std::endl;
    //    std::cout << (A || c) << std::endl;
    //
    //    return 0;
    //
    //    std::thread t1 = std::thread(wait2SecondsPrintHi);
    //
    //    std::this_thread::sleep_for(std::chrono::milliseconds(8000));
    //    std::cout << t1.joinable() << std::endl;
    //    KILL_THREAD = true;
    //    std::cout << "killed thread once." << std::endl;
    //
    //    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //    std::cout << t1.joinable() << std::endl;
    //    KILL_THREAD = true;
    //    std::cout << "killed thread twice." << std::endl;
    //
    //    t1.join();
    //
    //    return 0;
    //
    //    //     returnWhenTrue(1, true);
    //    //     returnWhenTrue(1, false);
    //    //     return 0;
    //    //
    //    //     std::map<int, std::map<std::string, int>> mps;
    //    //     mps[0]["zero"] = 0;
    //    //     mps[0]["one"] = 1;
    //    //     mps[0]["two"] = 2;
    //    //     mps[1]["zero"] = 1;
    //    //     mps[1]["one"] = 2;
    //    //     mps[2]["two"] = 4;
    //    //     for (auto [str, num] : mps[0]) {
    //    //         std::cout << str << ' ' << num << std::endl;
    //    //     }
    //    //     return 0;
    //    //
    //    //     srand(time(NULL));
    //    //     for (int i = 0; i < 10; ++i) {
    //    //         std::cout << rand() << std::endl;
    //    //     }
    //    //     return 0;
    //    //
    //    //     time_t t1 = time(NULL);
    //    //     std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    //    //     time_t t2 = time(NULL);
    //    //     std::cout << t1 << ' ' << t2 << std::endl;
    //    //     return 0;
    //    //
    //    //     std::set<int> test_set;
    //    //
    //    //     test_set.insert(1);
    //    //     auto fit = test_set.find(1);
    //    //     std::cout << (fit == test_set.end()) << std::endl;
    //    //     int cnt = test_set.erase(1);
    //    //     std::cout << cnt << std::endl;
    //    //
    //    //     test_set.insert(1);
    //    //     fit = test_set.find(2);
    //    //     std::cout << (fit == test_set.end()) << std::endl;
    //    //     cnt = test_set.erase(2);
    //    //     std::cout << cnt << std::endl;
    //    //
    //    //     return 0;
}