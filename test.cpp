#include <iostream>
#include <set>
#include <thread>
#include <time.h>

int main()
{
    time_t t1 = time(NULL);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    time_t t2 = time(NULL);
    std::cout << t1 << ' ' << t2 << std::endl;
    return 0;

    std::set<int> test_set;

    test_set.insert(1);
    auto fit = test_set.find(1);
    std::cout << (fit == test_set.end()) << std::endl;
    int cnt = test_set.erase(1);
    std::cout << cnt << std::endl;

    test_set.insert(1);
    fit = test_set.find(2);
    std::cout << (fit == test_set.end()) << std::endl;
    cnt = test_set.erase(2);
    std::cout << cnt << std::endl;

    return 0;
}