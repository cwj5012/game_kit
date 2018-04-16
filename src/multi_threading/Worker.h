#pragma once

#include <condition_variable>
#include <mutex>

#include "AbstractRequest.h"

class Worker {
public:
    Worker() {
        running = true;
        ready = false;
        ulock = std::unique_lock<std::mutex>(mtx);
    }

    void run();

    void stop() {
        running = false;
    }

    void setRequest(AbstractRequest* request);

    void getCondition(std::condition_variable* & cv);

private:
    std::condition_variable cv;
    std::mutex mtx;
    std::unique_lock<std::mutex> ulock;
    AbstractRequest* request;
    bool running;
    bool ready;
};
