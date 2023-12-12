#ifndef CASINO_GAMBLER_H
#define CASINO_GAMBLER_H

#include "logger.h"
#include "pub_sub.h"

namespace casino {
namespace gambler {

    class Gambler : public pub_sub::Subscriber {
    private:
        int mGamblerId;
        pub_sub::Publisher* mPublisher;
        logger::Logger myLogger;

        static int GAMBLER_COUNT;

    public:
        Gambler(pub_sub::Publisher* pub);
        void start_subscription();
        void stop_subscription();
        void accept(pub_sub::Event e);
    };

} // namespace gambler

} // namespace casino

#endif // CASINO_GAMBLER_H
