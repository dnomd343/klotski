#include "helper/concurrent.h"

using helper::Racer;
using helper::Executor;

// ----------------------------------------------------------------------------------------- //

std::function<void(std::function<void()> &&)> Executor::Entry() {
    return [this](auto &&func) {
        pool_.detach_task(func);
    };
}

Executor::~Executor() {
    pool_.wait();
}

// ----------------------------------------------------------------------------------------- //

void Racer::Execute(std::function<void()> &&item) {
    auto wrapper = [item = std::move(item)](const int) {
        item(); // execute racing function
    };
    pool_.detach_sequence(0, race_num_, wrapper);
    pool_.wait();
}

int Racer::RaceNum() const {
    return race_num_; // number of racing threads
}

Racer::~Racer() {
    pool_.wait();
}

// ----------------------------------------------------------------------------------------- //
