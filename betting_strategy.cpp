#include "betting_strategy.h"
#include "logger.h"
#include "pub_sub.h"

#include <cstdlib>
#include <numeric>
#include <utility>

namespace casino {
namespace betting_strategy {

    template <typename T>
    bool LiveStats<T>::is_even_partition(T val)
    {
        return val % 2 == 0;
    }

    template <typename T>
    bool LiveStats<T>::is_positive_partition(T val)
    {
        return val > 0;
    }

    template <typename T>
    LiveStats<T>::LiveStats()
    {
        last_1k_values.resize(MAX_SIZE, 0);
        partitionBy = is_positive_partition;
        partitioned_sum.resize(2, 0);
    }

    template <typename T>
    void LiveStats<T>::update(T val)
    {
        T old_val = last_1k_values[vector_ptr];

        last_1k_values[vector_ptr++] = val;
        vector_ptr %= MAX_SIZE;
        generation++;

        mean = ((generation - 1) * mean + (double)val) / (double)generation;
        cnt[val]++;
        if (cnt[val] > mode_cnt) {
            mode_cnt = cnt[val];
            mode = val;
        }
        sum += val;
        partitioned_sum[partitionBy(val)] += val;
    }

    template <typename T>
    int LiveStats<T>::count(T val)
    {
        return cnt[val];
    }

    template <typename T>
    void LiveStats<T>::setPartitionBy(bool (*func)(T))
    {
        partitionBy = func;
    }

    template <typename T>
    double LiveStats<T>::getSumOfParitionByMember(T val)
    {
        return partitioned_sum[partitionBy(val)];
    }

    template <typename T>
    T LiveStats<T>::getLastValue()
    {
        return last_1k_values[(vector_ptr - 1 + MAX_SIZE) % MAX_SIZE];
    }

    // -------------------------------------------------
    // -------------------------------------------------

    Stats::Stats(double start_money)
    {
        starting_money = start_money;
        my_logger = logger::Logger("Stats<" + std::to_string(start_money) + ">");
    }

    void Stats::accept(pub_sub::Event ev)
    {
        if (ev.event_type == pub_sub::MONEY_WON) {
            outcomes.update(1);
            result_amounts.update(ev.double_data);
        } else if (ev.event_type == pub_sub::MONEY_LOST) {
            outcomes.update(0);
            result_amounts.update(-ev.double_data);
        } else if (ev.event_type == pub_sub::BET_PLACED) {
            bet_numbers.update(ev.int_data);
            bet_amounts.update(ev.double_data);
        } else {
            my_logger.debug("ignoring event: ", ev.to_string());
        }
    }

    int Stats::getRoundsPlayed()
    {
        return outcomes.generation;
    }

    int Stats::getWinCount()
    {
        return outcomes.count(1);
    }

    int Stats::getLostCount()
    {
        return outcomes.count(0);
    }

    double Stats::getWinPercentage()
    {
        return outcomes.mean;
    }

    std::pair<int, int> Stats::getMostBetNumber()
    {
        return { bet_numbers.mode, bet_numbers.mode_cnt };
    }

    double Stats::getAverageBetNumber()
    {
        return bet_numbers.mean;
    }

    int Stats::getLastBetNumber()
    {
        return bet_numbers.getLastValue();
    }

    double Stats::getTotalBetAmount()
    {
        return bet_amounts.sum;
    }

    double Stats::getLastBetMoney()
    {
        return bet_amounts.getLastValue();
    }

    std::pair<double, int> Stats::getMostBetAmount()
    {
        return { bet_amounts.mode, bet_amounts.mode_cnt };
    }

    double Stats::getAverageBetAmount()
    {
        return bet_amounts.mean;
    }

    double Stats::getMoneyDeltaFromStart()
    {
        return result_amounts.sum;
    }

    double Stats::getMoneyWon()
    {
        return result_amounts.getSumOfParitionByMember(1);
    }

    double Stats::getMoneyLost()
    {
        return result_amounts.getSumOfParitionByMember(-1);
    }

    double Stats::getStartingMoney()
    {
        return starting_money;
    }

    double Stats::getTotalMoney()
    {
        return getStartingMoney() + getMoneyDeltaFromStart();
    }

    double Stats::getROIPercentage()
    {
        double moneyNow = getTotalMoney();
        return ((moneyNow - getStartingMoney()) / getStartingMoney()) * (double)100;
    }

    void Stats::print(int level)
    {
        if (level == 0) {
            std::cout << "Stats: "
                      << "After starting with " << getStartingMoney() << "rs, "
                      << "in " << getRoundsPlayed() << " rounds, "
                      << "we won total of " << getMoneyWon() << ". "
                      << "With an ROI of " << getROIPercentage() << "% "
                      << "we are now standing with " << getTotalMoney()
                      << std::endl;
        }
    }

    // -------------------------------------------------
    // -------------------------------------------------

    RecoverBetMoneyStrategy::RecoverBetMoneyStrategy(double start_money)
        : s(start_money)
        , lastTotalBetAmount(0)
    {
    }

    Stats RecoverBetMoneyStrategy::get_stats()
    {
        return s;
    }

    t_bet RecoverBetMoneyStrategy::get_next_bet()
    {
        int number;
        double bet_amount;

        // Same number to bet
        if (s.getRoundsPlayed() == 0 || s.getTotalBetAmount() == lastTotalBetAmount) {
            number = LUCKY_NUMBER;
            bet_amount = 0.1 * s.getStartingMoney();
        } else {
            number = s.getLastBetNumber();
            bet_amount = (s.getTotalBetAmount() - lastTotalBetAmount) / (double)STRAIGHT_UP_WIN_RETURNS;
        }

        return { number, bet_amount };
    }

    void RecoverBetMoneyStrategy::accept(pub_sub::Event ev)
    {
        if (ev.event_type == pub_sub::MONEY_WON) {
            lastTotalBetAmount = s.getTotalBetAmount();
        }
        s.accept(ev);
    }

} // namespace betting_strategy
} // namespace casino
