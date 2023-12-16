#ifndef CASINO_BET_H
#define CASINO_BET_H

#include <memory>
#include <unordered_map>
#include <vector>

namespace casino {
namespace bet {
    /**
     * List of numbers inside a bet
     */
    typedef std::vector<int> t_bet_numbers;

    /**
     * Unique identification for a bet
     */
    typedef std::tuple<BetType, int, int> t_bet_id;

    enum BetType {
        STRAIGHT_UP,
        SPLIT,
        STREET,
        CORNER,
        FIVE_WAY_LINE,
        LINE,
        SECTION,
        COLUMN,
        EVEN_ODD,
        // RED_BLACK
    };

    class Bet {
    private:
        /**
         * Private map of bet payouts.
         */
        static const std::unordered_map<BetType, int> _BET_PAYOUT;

        /**
         * Map of all bet instances.
         * Lazy growing.
         */
        static std::unordered_map<t_bet_id, std::shared_ptr<Bet>> _BET_OBJECT;

        /**
         * Gets the list of vector of numbers according to bet type
         */
        static std::vector<t_bet_numbers> _get_list_of_vector_numbers(const BetType bt);
        static std::unordered_map<BetType, std::vector<t_bet_numbers>> __memo_list_of_vectors;

        /**
         * Gets the correct vector of numbers from the list of vectors
         * according to num1 and num2.
         */
        static t_bet_numbers _get_vector_numbers(const t_bet_id);

        /**
         * All numbers contributing to this bet
         */
        t_bet_numbers _all_numbers;

        /**
         * The payout multiplier
         */
        int _payout;

        /**
         * Private constructor to restrict the created objects
         * First is bet type
         * Next are two numbers to uniquely identify the bet type
         * 0 on wheel is represented with -1
         * 00 on wheel is represented with 0
         * 1-36 on wheel are represented by self
         *
         * Second number has default -2, to allow using in one int mode
         * also.
         */
        Bet(BetType, int, int = -2);
        Bet(t_bet_id);

    public:
        /**
         * Get object by bet type and one int
         */
        std::shared_ptr<Bet> get_bet(const BetType, const int) const;

        /**
         * Get object by bet type and two ints
         */
        std::shared_ptr<Bet> get_bet(const BetType, const int, const int) const;

        /**
         * Get object by bet id
         */
        std::shared_ptr<Bet> get_bet(const t_bet_id) const;

        /**
         * Gets the payout multiplier by bet
         */
        int get_payout() const;

        /**
         * Gets all the numbers included in this bet
         */
        t_bet_numbers get_all_numbers() const;

        bool operator==(Bet&);
    };

} // namespace bet
} // namespace casino

#endif // CASINO_BET_H