#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <time.h>

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

int main()
{

    std::thread t1 = std::thread(wait2SecondsPrintHi);

    std::this_thread::sleep_for(std::chrono::milliseconds(8000));
    std::cout << t1.joinable() << std::endl;
    KILL_THREAD = true;
    std::cout << "killed thread once." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << t1.joinable() << std::endl;
    KILL_THREAD = true;
    std::cout << "killed thread twice." << std::endl;

    t1.join();

    return 0;

    //     returnWhenTrue(1, true);
    //     returnWhenTrue(1, false);
    //     return 0;
    //
    //     std::map<int, std::map<std::string, int>> mps;
    //     mps[0]["zero"] = 0;
    //     mps[0]["one"] = 1;
    //     mps[0]["two"] = 2;
    //     mps[1]["zero"] = 1;
    //     mps[1]["one"] = 2;
    //     mps[2]["two"] = 4;
    //     for (auto [str, num] : mps[0]) {
    //         std::cout << str << ' ' << num << std::endl;
    //     }
    //     return 0;
    //
    //     srand(time(NULL));
    //     for (int i = 0; i < 10; ++i) {
    //         std::cout << rand() << std::endl;
    //     }
    //     return 0;
    //
    //     time_t t1 = time(NULL);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    //     time_t t2 = time(NULL);
    //     std::cout << t1 << ' ' << t2 << std::endl;
    //     return 0;
    //
    //     std::set<int> test_set;
    //
    //     test_set.insert(1);
    //     auto fit = test_set.find(1);
    //     std::cout << (fit == test_set.end()) << std::endl;
    //     int cnt = test_set.erase(1);
    //     std::cout << cnt << std::endl;
    //
    //     test_set.insert(1);
    //     fit = test_set.find(2);
    //     std::cout << (fit == test_set.end()) << std::endl;
    //     cnt = test_set.erase(2);
    //     std::cout << cnt << std::endl;
    //
    //     return 0;
}