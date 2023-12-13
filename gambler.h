#ifndef CASINO_GAMBLER_H
#define CASINO_GAMBLER_H

#include "betting_strategy.h"
#include "logger.h"
#include "pub_sub.h"
#include "wheel.h"

namespace casino {
namespace gambler {

    class Gambler : public pub_sub::Subscriber {
    private:
        int myGamblerID;
        // TODO: can we decouple gambler and wheel? first think why should we do that?
        wheel::Wheel* myWheel;
        logger::Logger myLogger;
        double myMoney;
        betting_strategy::Strategy* myStrategy;
        bool do_auto_betting;

        static int GAMBLER_COUNT;

        void account_win(pub_sub::Event e);

        std::string log_prefix();
        std::string log_prefix(std::string);

    public:
        Gambler(betting_strategy::Strategy*, wheel::Wheel*);
        void start_subscription();
        void stop_subscription();
        void start_auto_betting();
        void stop_auto_betting();
        void accept(pub_sub::Event e);

        /**
         * Places a bet with the given amt on the given number.
         * If amt is <= 0 then any bet placed by this gambler on that number is reset.
         */
        void place_bet(betting_strategy::t_bet);
        void auto_place_bet();
    };

} // namespace gambler

} // namespace casino

#endif // CASINO_GAMBLER_H
