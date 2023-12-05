#include "gambler.h"
#include "logger.h"
#include "pub_sub.h"

using namespace casino;

class Gambler : public pub_sub::Subscriber {
private:
    int mGamblerId;
    pub_sub::Publisher* mPublisher;
    logger::Logger myLogger;

    static int GAMBLER_COUNT;

public:
    Gambler(pub_sub::Publisher* pub)
        : mGamblerId(GAMBLER_COUNT++)
    {
        mPublisher = pub;
        myLogger = logger::Logger("Gambler" + std::to_string(mGamblerId));
    }

    void start_subscription()
    {
        mPublisher->subscribe(this);
    }

    void stop_subscription()
    {
        mPublisher->unsubscribe(this);
    }

    void accept(pub_sub::Event e)
    {
        myLogger.warning("Gambler: ", mGamblerId, " received event: ", e.event_type);
    }
};
int Gambler::GAMBLER_COUNT = 0;
