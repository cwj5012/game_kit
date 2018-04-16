#pragma once

#include <string>

class AbstractRequest {
public:
    virtual void setValue(int value) = 0;
    virtual void process() = 0;
    virtual void finish() = 0;
};

typedef void (*logFunction)(std::string text);

class Request : public AbstractRequest {
public:
    void setValue(int value) override {
        this->value = value;
    }

    void setOutput(logFunction fnc) {
        this->outFnc = fnc;
    }

    void process() override {
        outFnc("Starting processing request " + std::to_string(value) + "...");
    }

    void finish() override {
        outFnc("Finished request " + std::to_string(value));
    }

private:
    int value;
    logFunction outFnc;

};
