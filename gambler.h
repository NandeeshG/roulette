#ifndef CASINO_GAMBLER_H
#define CASINO_GAMBLER_H

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

        static int GAMBLER_COUNT;

        void account_win(pub_sub::Event e);

        std::string log_prefix();
        std::string log_prefix(std::string);

    public:
        Gambler(double, wheel::Wheel*);
        void start_subscription();
        void stop_subscription();
        void accept(pub_sub::Event e);

        /**
         * Places a bet with the given amt on the given number.
         * If amt is <= 0 then any bet placed by this gambler on that number is reset.
         */
        void place_bet(betting_strategy::t_bet);
    };

} // namespace gambler

} // namespace casino

#endif // CASINO_GAMBLER_H
