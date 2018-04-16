#include "Dispatcher.h"

#include <iostream>

std::queue<AbstractRequest*> Dispatcher::requests_queue_;
std::queue<Worker*> Dispatcher::workers_queue_;
std::mutex Dispatcher::requests_mutex_;
std::mutex Dispatcher::workers_mutex_;
std::vector<Worker*> Dispatcher::workers_;
std::vector<std::thread*> Dispatcher::threads_;

// 初始化 worker 数量，工作线程和 worker 绑定
bool Dispatcher::init(int workers) {
    std::thread* t = nullptr;
    Worker* w = nullptr;
    for ( int i = 0; i < workers; i++ ) {
        w = new Worker;
        workers_.push_back(w);
        t = new std::thread(&Worker::run, w);
        threads_.push_back(t);
    }
    return true;
}

// 停止 worker 线程
bool Dispatcher::stop() {
    for ( auto& x : workers_ ) {
        x->stop();
    }
    std::cout << "Stopped workers." << std::endl;
    for ( auto& x : threads_ ) {
        x->join();
        std::cout << "Joined threads." << std::endl;
    }
    return true;
}

void Dispatcher::addRequest(AbstractRequest* request) {
    workers_mutex_.lock();
    if ( !workers_.empty() ) {
        Worker* worker = workers_.front();
        worker->setRequest(request);
        std::condition_variable* cv;
        worker->getCondition(cv);
        cv->notify_one();
        workers_queue_.pop();
        workers_mutex_.unlock();
    } else {
        workers_mutex_.unlock();
        requests_mutex_.lock();
        requests_queue_.push(request);
        requests_mutex_.unlock();
    }
}

bool Dispatcher::addWorker(Worker* worker) {
    bool wait = true;
    requests_mutex_.lock();
    if ( !requests_queue_.empty() ) {
        AbstractRequest* request = requests_queue_.front();
        worker->setRequest(request);
        requests_queue_.pop();
        wait = false;
        requests_mutex_.unlock();
    } else {
        requests_mutex_.unlock();
        workers_mutex_.lock();
        workers_queue_.push(worker);
        workers_mutex_.unlock();
    }

    return wait;
}
