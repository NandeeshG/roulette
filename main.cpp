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
    GLOBAL_DEFAULT_LOG_LEVEL = DEBUG;
    Logger lg = Logger("main");

    Wheel w;
    w.start();

    constexpr double INITIAL_MONEY = 10000;

    Gambler g1 = Gambler(INITIAL_MONEY, &w);
    // Gambler g2 = Gambler(&w);

    g1.start_subscription();
    // g2.start_subscription();

    // strategy
    RecoverBetMoneyStrategy strategy(INITIAL_MONEY);

    for (int i = 0; i <= 1000; ++i) {
        bool wait = !w.taking_bets();
        while (wait) {
            wait = !w.taking_bets();
        }

        t_bet bet = strategy.get_next_bet();
        lg.warning("Placing on ", bet.first, " rs ", bet.second);
        w.place_bet(&g1, bet);
    }

    w.stop();

    Stats s = strategy.get_stats();
    std::cout << "Stats: "
              << "After starting with " << s.getStartingMoney() << "rs, "
              << "in " << s.getRoundsPlayed() << " rounds, "
              << "we won total of " << s.getMoneyWon() << ". "
              << "With an ROI of " << s.getROIPercentage()
              << "we are now standing with " << s.getTotalMoney()
              << std::endl;

    return 0;
}
