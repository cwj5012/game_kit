#pragma once

#include "AbstractRequest.h"
#include "Worker.h"

#include <queue>
#include <mutex>
#include <thread>
#include <vector>

class Dispatcher {
public:
    static bool init(int workers);
    static bool stop();
    static void addRequest(AbstractRequest* request);
    static bool addWorker(Worker* worker);
private:
    static std::queue<AbstractRequest*> requests_queue_;
    static std::queue<Worker*> workers_queue_;
    static std::mutex requests_mutex_;
    static std::mutex workers_mutex_;
    static std::vector<Worker*> workers_;
    static std::vector<std::thread*> threads_;
};
