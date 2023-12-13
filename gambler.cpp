#include "gambler.h"
#include "logger.h"
#include "pub_sub.h"
#include "wheel.h"

namespace casino {
namespace gambler {

    int Gambler::GAMBLER_COUNT = 0;

    Gambler::Gambler(double money, wheel::Wheel* wheel)
        : myGamblerID(GAMBLER_COUNT++)
        , myMoney(money)
    {
        myWheel = wheel;
        myLogger = logger::Logger("Gambler<" + std::to_string(myGamblerID) + ">");
        if (myMoney <= 0) {
            myLogger.severe("starting money can't be <= 0, we were given: ", myMoney);
        }
    }

    void Gambler::start_subscription()
    {
        myWheel->subscribe(this);
    }

    void Gambler::stop_subscription()
    {
        myWheel->unsubscribe(this);
    }

    void Gambler::accept(pub_sub::Event e)
    {
        if (e.event_type == pub_sub::MONEY_WON) {
            myLogger.debug(log_prefix() + "received event: ", e.to_string());
            account_win(e);
        } else {
            myLogger.debug(log_prefix() + "received event: ", e.to_string());
        }
    }

    void Gambler::account_win(pub_sub::Event e)
    {
        myLogger.info(log_prefix("has won: " + std::to_string(e.double_data)));
        myMoney += e.double_data;
    }

    void Gambler::place_bet(betting_strategy::t_bet bet)
    {
        double amt = bet.second;
        if (amt > myMoney) {
            myLogger.severe(log_prefix("can't place trades with leverage as of now."));
        }
        bool ret = myWheel->place_bet(this, bet);
        if (!ret) {
            myLogger.error(log_prefix("failed placing bet."));
            return;
        }
        myMoney -= amt;
    }

    std::string Gambler::log_prefix()
    {
        return "GamblerID: " + std::to_string(myGamblerID) + " ";
    }

    std::string Gambler::log_prefix(std::string suffix)
    {
        return "GamblerID: " + std::to_string(myGamblerID) + " " + suffix;
    }

} // namespace gambler
} // namespace casino
