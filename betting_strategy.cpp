#include "betting_strategy.h"
#include "logger.h"
#include "pub_sub.h"

#include <assert.h>
#include <cstdlib>
#include <math.h>
#include <numeric>
#include <utility>

namespace {
template <typename>
struct is_std_vector : std::false_type { };

template <typename T>
struct is_std_vector<std::vector<T>> : std::true_type { };
} // namespace anonymous

namespace casino {
namespace betting_strategy {

    template <typename T>
    bool LiveStats<T>::_is_even(T val, std::true_type _)
    {
        return val.size() % 2 == 0;
    }

    template <typename T>
    bool LiveStats<T>::_is_even(T val, std::false_type _)
    {
        return val % 2 == 0;
    }

    template <typename T>
    bool LiveStats<T>::is_even(T val)
    {
        return is_even(val, is_std_vector<T> {});
    }

    template <typename T>
    bool LiveStats<T>::_is_positive(T val, std::true_type _)
    {
        return val.size() > 0;
    }

    template <typename T>
    bool LiveStats<T>::_is_positive(T val, std::false_type _)
    {
        return val > 0;
    }

    template <typename T>
    bool LiveStats<T>::is_positive(T val)
    {
        return _is_positive(val, is_std_vector<T> {});
    }

    template <typename T>
    bool LiveStats<T>::_by_threshold(T val, long long t, std::true_type _)
    {
        for (T v : val) {
            if (v <= t) {
                return false;
            }
        }
        return true;
    }

    template <typename T>
    bool LiveStats<T>::_by_threshold(T val, long long t, std::false_type _)
    {
        return val > t;
    }

    template <typename T>
    template <long long H>
    bool LiveStats<T>::by_threshold(T val)
    {
        return _by_threshold(val, H, is_std_vector<T> {});
    }

    template <typename T>
    double LiveStats<T>::_addition(double o1, T o2, std::true_type _)
    {
        return std::accumulate(o2.begin(), o2.end(), o1);
    }

    template <typename T>
    double LiveStats<T>::_addition(double o1, T o2, std::true_type _)
    {
        return o1 + o2;
    }

    template <typename T>
    double LiveStats<T>::addition(double o1, T o2)
    {
        return _addition(o1, o2, is_std_vector<T> {});
    }

    template <typename T>
    LiveStats<T>::LiveStats(
        bool (*partitioner)(T) = &is_positive,
        double (*aggregator)(double, T)
        = &addition)
    {
        _mean = _size = _aggregate = _mode = _front_idx = 0;
        _partitioner = partitioner;
        _aggregator = aggregator;
        _historical_values.resize(MAX_SIZE, 0);
        _partition_agg.resize(2, 0);
    }

    template <typename T>
    void LiveStats<T>::update(T val)
    {
        T old_val = _historical_values[_front_idx];

        _historical_values[_front_idx++] = val;
        _front_idx %= MAX_SIZE;
        _size++;

        _mean = ((_size - 1) * _mean + (double)val) / (double)_size;
        _value_freq[val]++;
        if (_value_freq[val] > _value_freq[_mode]) {
            _mode = val;
        }
        _aggregate = _aggregator(_aggregate, val);
        _partition_agg[_partitioner(val)] = _aggregator(
            _partition_agg[_partitioner(val)], val);
    }

    template <typename T>
    int LiveStats<T>::get_frequency(T val)
    {
        return _value_freq[val];
    }

    template <typename T>
    double LiveStats<T>::get_aggregated_parition_by_member(T val)
    {
        return _partition_agg[_partitioner(val)];
    }

    template <typename T>
    T LiveStats<T>::get_recent_value(int i)
    {
        assert(i >= 0);
        i %= MAX_SIZE;
        return _historical_values[(_front_idx - i - 1 + MAX_SIZE) % MAX_SIZE];
    }

    template <typename T>
    T LiveStats<T>::get_mean()
    {
        return _mean;
    }

    template <typename T>
    T LiveStats<T>::get_mode()
    {
        return _mode;
    }

    template <typename T>
    T LiveStats<T>::get_aggregate()
    {
        return _aggregate;
    }

    template <typename T>
    int LiveStats<T>::get_size()
    {
        return _size;
    }

    // -------------------------------------------------
    // -------------------------------------------------

    Stats::Stats(double start_money)
        : _starting_money(start_money)
        , _bet_numbers(&LiveStats<std::vector<int>>::is_even)
        , _bet_amounts(&LiveStats<std::vector<double>>::by_threshold<THRESHOLD>)
    //: _outcomes(&LiveStats<bool>::is_positive, &LiveStats<bool>::addition)
    //, _result_amounts(&LiveStats<double>::is_positive, &LiveStats<double>::addition)
    //, _bet_numbers(&LiveStats<std::vector<int>>::is_even, &LiveStats<std::vector<int>>::addition)
    //, _bet_amounts(&LiveStats<std::vector<double>>::by_threshold<THRESHOLD>, &LiveStats<std::vector<double>>::addition)
    {
        _logger = logger::Logger("Stats<" + std::to_string(_starting_money) + ">");
    }

    void Stats::accept(pub_sub::Event ev)
    {
        if (ev.event_type == pub_sub::PLYR_WON) {
            _outcomes.update(1);
            _result_amounts.update(ev.double_data);
        } else if (ev.event_type == pub_sub::PLYR_LOST) {
            _outcomes.update(0);
            _result_amounts.update(-ev.double_data);
        } else if (ev.event_type == pub_sub::BET_ACCEPTED) {
            _bet_numbers.update(ev.int_data);
            _bet_amounts.update(ev.double_data);
        } else {
            _logger.debug("ignoring event: ", ev.to_string());
        }
    }

    int Stats::get_rounds_played()
    {
        return _outcomes.getSize();
    }

    int Stats::get_win_count()
    {
        return _outcomes.get_frequency(1);
    }

    int Stats::get_lost_count()
    {
        return _outcomes.get_frequency(0);
    }

    double Stats::get_win_percentage()
    {
        return _outcomes._mean;
    }

    std::pair<int, int> Stats::get_most_bet_number()
    {
        return { _bet_numbers._mode, _bet_numbers.mode_cnt };
    }

    double Stats::get_average_bet_number()
    {
        return _bet_numbers._mean;
    }

    int Stats::get_last_bet_number()
    {
        return _bet_numbers.getLastValue();
    }

    double Stats::get_total_bet_amount()
    {
        return _bet_amounts._aggregate;
    }

    double Stats::get_last_bet_money()
    {
        return _bet_amounts.getLastValue();
    }

    std::pair<double, int> Stats::get_most_bet_amount()
    {
        return { _bet_amounts._mode, _bet_amounts.mode_cnt };
    }

    double Stats::get_average_bet_amount()
    {
        return _bet_amounts._mean;
    }

    double Stats::get_money_delta_from_start()
    {
        return _result_amounts._aggregate;
    }

    double Stats::get_money_won()
    {
        return _result_amounts.get_partition_aggregate_by_member(1);
    }

    double Stats::get_money_lost()
    {
        return _result_amounts.get_partition_aggregate_by_member(-1);
    }

    double Stats::get_starting_money()
    {
        return _starting_money;
    }

    double Stats::get_total_money()
    {
        return get_starting_money() + get_money_delta_from_start();
    }

    double Stats::get_roi_percentage()
    {
        double moneyNow = get_total_money();
        return ((moneyNow - get_starting_money()) / get_starting_money()) * (double)100;
    }

    void Stats::print(int level)
    {
        if (level == 0) {
            std::cout << "Stats: "
                      << "After starting with " << get_starting_money() << "rs, "
                      << "in " << get_rounds_played() << " rounds, "
                      << "we won total of " << get_money_won() << ", "
                      << "by placing bets totalling " << get_total_bet_amount() << ". "
                      << "With an ROI of " << get_roi_percentage() << "% "
                      << "we are now standing with " << get_total_money()
                      << std::endl;
        }
    }

    // // -------------------------------------------------
    // // -------------------------------------------------

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
        if (s.get_rounds_played() == 0 || s.get_total_bet_amount() == lastTotalBetAmount) {
            number = LUCKY_NUMBER;
            bet_amount = ceil(0.1 * s.get_starting_money());
        } else {
            number = s.get_last_bet_number();
            bet_amount = ceil((s.get_total_bet_amount() - lastTotalBetAmount) / (double)(STRAIGHT_UP_WIN_RETURNS)) + 0.01 * s.get_starting_money();
        }

        return { number, bet_amount };
    }

    void RecoverBetMoneyStrategy::accept(pub_sub::Event ev)
    {
        if (ev.event_type == pub_sub::PLYR_WON) {
            lastTotalBetAmount = s.get_total_bet_amount();
        }
        s.accept(ev);
    }

} // namespace betting_strategy
} // namespace casino
