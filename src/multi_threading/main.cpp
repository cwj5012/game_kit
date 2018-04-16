#include "dispatcher.h"
#include "AbstractRequest.h"

#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>

sig_atomic_t signal_caught = 0;
std::mutex logMutex;

void sigint_handler(int sig) {
    signal_caught = 1;
}

void logFnc(std::string text) {
    logMutex.lock();
    std::cout << text << "\n";
    logMutex.unlock();
}

int main() {
    // Install signal handler.
    signal(SIGINT, &sigint_handler);

    // Initialise the dispatcher with 10 worker threads.
    Dispatcher::init(10);

    std::cout << "Initialised.\n";

    // Generate requests in a continuous loop until terminated with SIGINT or
    // limit has been reached.
    int cycles = 0;
    Request* rq = nullptr;
    while ( !signal_caught && cycles < 50 ) {
        rq = new Request();
        rq->setValue(cycles);
        rq->setOutput(&logFnc);
        Dispatcher::addRequest(rq);
        cycles++;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Cleanup.
    Dispatcher::stop();
    std::cout << "Clean-up done.\n";

    return 0;
}
