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

int main()
{
    Logger lg = Logger("main");

    Wheel w = Wheel();
    w.start();

    Gambler g1 = Gambler(&w);
    Gambler g2 = Gambler(&w);

    g1.start_subscription();
    g2.start_subscription();

    w.wait();

    return 0;
}
