#ifndef CASINO_BETTING_STRATEGY_H
#define CASINO_BETTING_STRATEGY_H

#include "logger.h"
#include "pub_sub.h"
#include <unordered_map>
#include <utility>
#include <vector>

namespace casino {
namespace betting_strategy {

    typedef std::pair<int, double> t_bet;

    template <typename T>
    class LiveStats {
    private:
        static bool is_even_partition(T);
        static bool is_positive_partition(T);

        static const int MAX_SIZE = 1000;
        int vector_ptr = 0;
        std::unordered_map<T, int> cnt;

        // by default partition is based on positive
        bool (*partitionBy)(T val);

        std::vector<double> partitioned_sum;

    public:
        LiveStats();
        int generation = 0;
        std::vector<T> last_1k_values;
        double mean = 0;
        // TODO: Add running median using left maxHeap and right minHeap
        // T median = 0;
        T mode = 0;
        T sum = 0;
        int mode_cnt = 0;
        void update(T);
        int count(T val);
        void setPartitionBy(bool (*func)(T));
        double getSumOfParitionByMember(T);
        T getLastValue();
    };

    class Stats {
    private:
        LiveStats<bool> outcomes;
        LiveStats<double> result_amounts;
        LiveStats<int> bet_numbers;
        LiveStats<double> bet_amounts;
        // money won after placing x bet = x*35
        // money lost after placing x bet = -x

        double starting_money;
        logger::Logger my_logger;

    public:
        // Initialize Stats with starting money
        Stats(double);

        void accept(pub_sub::Event);

        int getRoundsPlayed();
        int getWinCount();
        int getLostCount();
        double getWinPercentage();

        /**
         * First is the most bet number
         * Second is the number of times it was bet
         */
        std::pair<int, int> getMostBetNumber();
        double getAverageBetNumber();
        int getLastBetNumber();

        double getTotalBetAmount();
        double getLastBetMoney();

        /**
         * First is the most bet amount
         * Second is the number of times it was bet
         */
        std::pair<double, int> getMostBetAmount();
        double getAverageBetAmount();

        double getMoneyDeltaFromStart();
        double getMoneyWon();
        double getMoneyLost();

        double getStartingMoney();
        double getTotalMoney();
        double getROIPercentage();

        void print(int lvl = 0);
    };

    class Strategy {
    public:
        // TODO: make get_stats non-virtual
        virtual Stats get_stats() = 0;

        virtual std::pair<int, double> get_next_bet() = 0;
        virtual void accept(pub_sub::Event) = 0;
    };

    /**
     * Always does straight up betting
     * Bets 1/10th of starting money on the LUCKY NUMBER.
     * Then bets the amount required to recover money bet
     *  till then on the same LUCKY NUMBER.
     * Once money is recovered, begins again with 1/10th of starting money.
     */
    class RecoverBetMoneyStrategy : public Strategy {
    private:
        Stats s;
        static const int STRAIGHT_UP_WIN_RETURNS = 35;
        static const int LUCKY_NUMBER = 20;
        double lastTotalBetAmount;

    public:
        RecoverBetMoneyStrategy(double start_money);
        Stats get_stats();
        std::pair<int, double> get_next_bet();
        void accept(pub_sub::Event ev);
    };
}
}

#endif // CASINO_BETTING_STRATEGY_H