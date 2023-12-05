#include "gambler.h"
#include "logger.h"
#include "pub_sub.h"

namespace casino {
namespace gambler {

    int Gambler::GAMBLER_COUNT = 0;

    Gambler::Gambler(pub_sub::Publisher* pub)
        : mGamblerId(GAMBLER_COUNT++)
    {
        mPublisher = pub;
        myLogger = logger::Logger("Gambler" + std::to_string(mGamblerId));
    }

    void Gambler::start_subscription()
    {
        mPublisher->subscribe(this);
    }

    void Gambler::stop_subscription()
    {
        mPublisher->unsubscribe(this);
    }

    void Gambler::accept(pub_sub::Event e)
    {
        myLogger.warning("Gambler: ", mGamblerId, " received event: ", e.event_type);
    }

} // namespace gambler
} // namespace casino
