#include "Worker.h"

#include <chrono>

#include "Dispatcher.h"

void Worker::run() {
    while (running) {
        if (ready) {
            // Execute the request.
            ready = false;
            request->process();
            request->finish();
        }

        // Add self to Dispatcher queue and execute next request or wait.
        if (Dispatcher::addWorker(this)) {
            // Use the ready loop to deal with spurious wake-ups.
            while (!ready && running) {
                if (cv.wait_for(ulock, std::chrono::seconds(1)) == std::cv_status::timeout) {
                    // We timed out, but we keep waiting unless the worker is
                    // stopped by the dispatcher.
                }
            }
        }
    }
}

void Worker::setRequest(AbstractRequest* request) {
    this->request = request;
    ready = true;
}

void Worker::getCondition(std::condition_variable*& cv) {
    cv = &(this)->cv;
}

