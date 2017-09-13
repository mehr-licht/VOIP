/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "receiver.h"

Receiver::Receiver()
  : self_(),
    running_(true)
{}

Receiver::~Receiver() {
  if (running_) {
    stop();
  }
}

void Receiver::start() {
  self_ = std::thread( [=] { receive(); });
}

void Receiver::stop() {
  running_ = false;
  self_.join();
}

void Receiver::receive() {
  while (running_) {
    std::cout << "Running the thread..." << std::endl;
  }
}
