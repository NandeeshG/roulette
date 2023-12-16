#include "bet.h"
#include "logger.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace casino {
namespace bet {

    const std::unordered_map<BetType, int> _BET_PAYOUT = {
        { STRAIGHT_UP, 35 },
        { SPLIT, 17 },
        { STREET, 11 },
        { CORNER, 8 },
        { FIVE_WAY_LINE, 6 },
        { LINE, 5 },
        { SECTION, 2 },
        { COLUMN, 2 },
        { EVEN_ODD, 1 },
        // { RED_BLACK, 1 }
    };

    Bet::Bet(t_bet_id id)
    {
        Bet(std::get<0>(id), std::get<1>(id), std::get<2>(id));
    }

    Bet::Bet(BetType bt, int num1, int num2 = -2)
    {
        _payout = _BET_PAYOUT.at(bt);
        _all_numbers = _get_vector_numbers(t_bet_id { bt, num1, num2 });
    }

    std::vector<t_bet_numbers> Bet::_get_list_of_vector_numbers(const BetType bt)
    {
        if (__memo_list_of_vectors.count(bt))
            return __memo_list_of_vectors[bt];
#define VI std::vector<int>
        std::vector<t_bet_numbers> complete_list;
        switch (bt) {
        case STRAIGHT_UP:
            for (int i = -1; i <= 36; ++i) {
                complete_list.push_back(VI { i });
            }
            break;

        case SPLIT:
            complete_list.push_back(VI { -1, 0 });
            for (int i = 1; i <= 31; i += 3) {
                complete_list.push_back(VI { i, i + 3 });
                complete_list.push_back(VI { i, i + 1 });
                complete_list.push_back(VI { i + 1, i + 4 });
                complete_list.push_back(VI { i + 1, i + 2 });
                complete_list.push_back(VI { i + 2, i + 5 });
            }
            break;

        case STREET:
            complete_list.push_back(VI { -1, 0, 2 });
            for (int i = 1; i <= 34; i += 3) {
                complete_list.push_back(VI { i, i + 1, i + 2 });
            }
            break;

        case CORNER:
            auto hlpr = [](int i) { return VI { i, i + 1, i + 3, i + 4 }; };
            for (int i = 1; i <= 34; i += 3) {
                complete_list.push_back(hlpr(i));
                complete_list.push_back(hlpr(i + 1));
            }
            break;

        case FIVE_WAY_LINE:
            complete_list.push_back(VI { -1, 0, 1, 2, 3 });
            break;

        case LINE:
            for (int i = 1; i <= 31; i += 3) {
                VI tmp;
                for (int j = i; j <= i + 5; ++j) {
                    tmp.push_back(j);
                }
                complete_list.push_back(tmp);
            }
            break;

        case SECTION:
            for (int i = 1; i <= 25; i += 12) {
                VI tmp;
                for (int j = i; j <= i + 11; ++j) {
                    tmp.push_back(j);
                }
                complete_list.push_back(tmp);
            }
            break;

        case COLUMN:
            for (int i = 1; i <= 3; i++) {
                VI tmp;
                for (int j = i; j <= i + 33; j += 3) {
                    tmp.push_back(j);
                }
                complete_list.push_back(tmp);
            }
            break;

        case EVEN_ODD: {
            VI even, odd;
            for (int i = 1; i <= 36; i++) {
                if (i & 1)
                    odd.push_back(i);
                else
                    even.push_back(i);
            }
            complete_list.push_back(even);
            complete_list.push_back(odd);
        } break;

        default:
            logger::logger.severe("unknown bet type received!");
        }
#undef VI
        for (t_bet_numbers nums : complete_list) {
            std::sort(nums.begin(), nums.end());
        }
        return __memo_list_of_vectors[bt] = complete_list;
    }

    t_bet_numbers Bet::_get_vector_numbers(const t_bet_id id)
    {
        BetType bt = std::get<0>(id);
        int num1 = std::get<1>(id);
        int num2 = std::get<2>(id);
        // TODO: Use tie instead of above two lines

        if (num2 < num1) {
            std::swap(num1, num2);
        }
        if (num1 < -1 && num2 < -1 && bt != FIVE_WAY_LINE) {
            logger::logger.severe("both num1 and num2 were unknown");
        }

        auto complete_list = _get_list_of_vector_numbers(bt);

        switch (bt) {
        case STRAIGHT_UP:
            for (t_bet_numbers list : complete_list) {
                if (list[0] == num1) {
                    return list;
                }
            }
            break;

        case SPLIT:
            for (t_bet_numbers list : complete_list) {
                if (list[0] == num1 && list[1] == num2) {
                    return list;
                }
            }
            break;

        case STREET:
            for (t_bet_numbers list : complete_list) {
                if (list[0] == num1 || list[2] == num1) {
                    return list;
                }
            }
            break;

        case CORNER:
            for (t_bet_numbers list : complete_list) {
                if ((list[0] == num1 && list[3] == num2)
                    || (list[1] == num1 && list[2] == num2)) {
                    return list;
                }
            }
            break;

        case FIVE_WAY_LINE:
            return complete_list[0];
            break;

        case LINE:
            for (t_bet_numbers list : complete_list) {
                if ((list[0] == num1 && list[3] == num2)
                    || (list[2] == num1 && list[5] == num2)) {
                    return list;
                }
            }
            break;

        case SECTION:
        case COLUMN:
            for (t_bet_numbers list : complete_list) {
                for (int num : list) {
                    if (num == num1) {
                        return list;
                    }
                }
            }
            break;

        case EVEN_ODD:
            if (num1 <= 0)
                logger::logger.severe("zero or zero-zero not allowed in even-odd");
            return complete_list[num1 & 1];

        default:
            logger::logger.severe("unknown bet type received!");
        }
        logger::logger.severe("numbers not found!");
        return t_bet_numbers {};
    }

    std::shared_ptr<Bet> Bet::get_bet(const BetType bt, const int num) const
    {
        return get_bet(t_bet_id { bt, num, -2 });
    }

    std::shared_ptr<Bet> Bet::get_bet(const BetType bt, const int num1, const int num2) const
    {
        return get_bet(t_bet_id { bt, num1, num2 });
    }

    std::shared_ptr<Bet> Bet::get_bet(const t_bet_id id) const
    {
        if (_BET_OBJECT.count(id))
            return _BET_OBJECT[id];
        return _BET_OBJECT[id] = std::make_shared<Bet>(id);
    }

    /**
     * Gets the payout multiplier by bet
     */
    int Bet::get_payout() const
    {
        return _payout;
    }

    /**
     * Gets all the numbers included in this bet
     */
    t_bet_numbers Bet::get_all_numbers() const
    {
        return t_bet_numbers { _all_numbers };
    }

    bool Bet::operator==(Bet& o1)
    {
        return _all_numbers == o1._all_numbers
            && _payout == o1._payout;
    }

} // namespace bet
} // namespace casino
