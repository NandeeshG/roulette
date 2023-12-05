#include "logger.h"
#include "pub_sub.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <set>
#include <thread>

using namespace casino::logger;
using namespace casino::pub_sub;

class Wheel : public Publisher {
private:
    std::set<Subscriber*> mySubscribers;
    std::thread worker_thread;
    Logger myLogger;

public:
    Wheel()
    {
        myLogger = Logger("Wheel");
    }

    void subscribe(Subscriber* s)
    {
        mySubscribers.insert(s);
    }

    void unsubscribe(Subscriber* s)
    {
        if (mySubscribers.count(s))
            mySubscribers.erase(s);
    }

    void publish(Event e)
    {
        for (Subscriber* s : mySubscribers) {
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
            Event e = Event(0, EventType(et));
            myLogger.warning("publishing event type: ", e.event_type);
            publish(e);
        }
    }

    void wait()
    {
        worker_thread.join();
    }
};

class Gambler : public Subscriber {
private:
    int mGamblerId;
    Publisher* mPublisher;
    Logger myLogger;

    static int GAMBLER_COUNT;

public:
    Gambler(Publisher* pub)
        : mGamblerId(GAMBLER_COUNT++)
    {
        mPublisher = pub;
        myLogger = Logger("Gambler" + std::to_string(mGamblerId));
    }

    void start_subscription()
    {
        mPublisher->subscribe(this);
    }

    void stop_subscription()
    {
        mPublisher->unsubscribe(this);
    }

    void accept(Event e)
    {
        myLogger.warning("Gambler: ", mGamblerId, " received event: ", e.event_type);
    }
};
int Gambler::GAMBLER_COUNT = 0;

int main()
{
    Logger lg = Logger("main");

    Wheel w = Wheel();
    w.start();

    Gambler g1 = Gambler(&w);
    Gambler g2 = Gambler(&w);

    g1.start_subscription();
    g2.start_subscription();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    g1.stop_subscription();
    g2.stop_subscription();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    g1.start_subscription();
    g2.start_subscription();

    w.wait();

    return 0;
}