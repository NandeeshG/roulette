#ifndef CASINO_BETTING_STRATEGY_H
#define CASINO_BETTING_STRATEGY_H

#include "logger.h"
#include "pub_sub.h"
#include <unordered_map>
#include <utility>
#include <vector>

namespace casino {
namespace betting_strategy {

    template <typename T>
    class LiveStats {
    private:
        /**
         * Partition function to use.
         */
        bool (*_partitioner)(T);

        /**
         * Aggregating function to use.
         */
        double (*_aggregator)(double, T);

        /**
         * Index of the latest element in history vector.
         */
        int _front_idx;

        /**
         * Size of full list.
         */
        int _size;

        /**
         * The maximum number of stats to keep.
         */
        static const int MAX_SIZE = 10000;

        /**
         * Count of each kind of value.
         */
        std::unordered_map<T, int> _value_freq;

        /**
         * List of MAX_SIZE historical values.
         */
        std::vector<T> _historical_values;

        /**
         * Aggregation of values by the partition.
         */
        std::vector<double> _partition_agg;

        double _mean;
        T _aggregate;
        T _mode;

        /**
         * Actual partitioning function for vector types.
         */
        static bool _is_even(T, std::true_type);
        static bool _is_positive(T, std::true_type);
        static bool _by_threshold(T, long long, std::true_type);

        /**
         * Actual partitioning function for non-vector types.
         */
        static bool _is_even(T, std::false_type);
        static bool _is_positive(T, std::false_type);
        static bool _by_threshold(T, long long, std::false_type);

        /**
         * Actual aggregating functions for vector types.
         */
        static double _addition(double, T, std::true_type);

        /**
         * Actual aggregating functions for non-vector types.
         */
        static double _addition(double, T, std::false_type);

    public:
        /**
         * Partitioning functions.
         */
        static bool is_even(T);
        static bool is_positive(T);
        template <long long H>
        static bool by_threshold(T);

        /**
         * Aggregating functions.
         */
        static double addition(double, T);

        LiveStats(
            bool (*partitioner)(T) = &is_positive,
            double (*aggregator)(double, T))
            = &addition;

        T get_mean();

        T get_mode();

        T get_aggregate();

        /**
         * Updates the stats with the given value
         */
        void update(T);

        /**
         * Gets the number of times a value appeared
         */
        int get_frequency(T val);

        /**
         * Get the aggregated partition number by member.
         */
        double get_aggregated_parition_by_member(T);

        /**
         * Gets the last iTh value in reverse from historical data.
         * Default argument 0 gets the last value.
         * argument 1 gets the second last value.
         */
        T get_recent_value(int = 0);

        /**
         * Gets the current size of stats.
         */
        int get_size();

        // TODO: Add running median using left maxHeap and right minHeap
        // T median = 0;
    };

    class Stats {
    private:
        /**
         * Win or loss outcomes in boolean
         */
        LiveStats<bool> _outcomes;

        /**
         * The amount won or lost in that round.
         * Example, money won after placing x bet = x*35
         * Example, money lost after placing x bet = -x
         */
        LiveStats<double> _result_amounts;

        /**
         * The numbers bet on in that round.
         */
        LiveStats<std::vector<int>> _bet_numbers;

        /**
         * The amounts bet on in that round.
         */
        LiveStats<std::vector<double>> _bet_amounts;
        static constexpr long long THRESHOLD = 100;

        /**
         * The starting money with us.
         */
        double _starting_money;

        logger::Logger _logger;

    public:
        // Initialize Stats with starting money
        Stats(double);

        void accept(pub_sub::Event);

        int get_rounds_played();
        int get_win_count();
        int get_lost_count();
        double get_win_percentage();

        /**
         * First is the most bet number
         * Second is the number of times it was bet
         */
        std::pair<int, int> get_most_bet_number();
        double get_average_bet_number();
        int get_last_bet_number();

        double get_total_bet_amount();
        double get_last_bet_money();

        /**
         * First is the most bet amount
         * Second is the number of times it was bet
         */
        std::pair<double, int> get_most_bet_amount();
        double get_average_bet_amount();

        double get_money_delta_from_start();
        double get_money_won();
        double get_money_lost();

        double get_starting_money();
        double get_total_money();
        double get_roi_percentage();

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