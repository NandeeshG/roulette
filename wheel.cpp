#include "wheel.h"
#include "logger.h"
#include "pub_sub.h"

using namespace casino;

class Wheel : public pub_sub::Publisher {
private:
    std::set<pub_sub::Subscriber*> mySubscribers;
    std::thread worker_thread;
    logger::Logger myLogger;

public:
    Wheel()
    {
        myLogger = logger::Logger("Wheel");
    }

    void subscribe(pub_sub::Subscriber* s)
    {
        mySubscribers.insert(s);
    }

    void unsubscribe(pub_sub::Subscriber* s)
    {
        if (mySubscribers.count(s))
            mySubscribers.erase(s);
    }

    void publish(pub_sub::Event e)
    {
        for (pub_sub::Subscriber* s : mySubscribers) {
            s->accept(e);
        }
    }

    void start()
    {
        worker_thread = std::thread([this] { _start(); });
    }

    void _start()
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

    void wait()
    {
        worker_thread.join();
    }
};
