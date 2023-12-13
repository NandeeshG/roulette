#include "betting_strategy.h"
#include "gambler.h"
#include "logger.h"
#include "wheel.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <set>
#include <thread>

using namespace casino::logger;
using namespace casino::gambler;
using namespace casino::wheel;
using namespace casino::betting_strategy;

int main()
{
    GLOBAL_DEFAULT_LOG_LEVEL = WARNING;
    Logger lg = Logger("main");

    Wheel w;
    w.start();

    constexpr double INITIAL_MONEY = 10000;
    RecoverBetMoneyStrategy strategy(INITIAL_MONEY);
    Gambler g1 = Gambler(&strategy, &w);

    g1.start_subscription();
    g1.start_auto_betting();

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    w.stop();

    Stats s = strategy.get_stats();
    s.print();

    return 0;
}
