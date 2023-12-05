#include "wheel.h"
#include "logger.h"
#include "pub_sub.h"

namespace casino {
namespace wheel {

    Wheel::Wheel()
    {
        myLogger = logger::Logger("Wheel");
    }

    void Wheel::subscribe(pub_sub::Subscriber* s)
    {
        mySubscribers.insert(s);
    }

    void Wheel::unsubscribe(pub_sub::Subscriber* s)
    {
        if (mySubscribers.count(s))
            mySubscribers.erase(s);
    }

    void Wheel::publish(pub_sub::Event e)
    {
        for (pub_sub::Subscriber* s : mySubscribers) {
            s->accept(e);
        }
    }

    void Wheel::start()
    {
        worker_thread = std::thread([this] { _start(); });
    }

    void Wheel::_start()
    {
        int t = 100;
        while (t--) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            int et = rand() % 3;
            pub_sub::Event e = pub_sub::Event(0, pub_sub::EventType(et));
            myLogger.warning("publishing event type: ", e.event_type);
            publish(e);
        }
    }

    void Wheel::wait()
    {
        worker_thread.join();
    }
} // namespace wheel
} // namespace casino
