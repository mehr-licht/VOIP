/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "receiver.h"
#include <iostream>

Receiver::Receiver()
  : self_(),
    running_(true)
{}

Receiver::~Receiver() {
  if (running_) {
    stop();
  }
}

void Receiver::start(int port) {
  running_ = true;
  util::Ipv4SocketAddress saddr("", port);
  addr = saddr;
  s.open();
  if (s.isOpen()) {//metihe
      std::cerr << "Socket is open. Listening... " << std::endl;//metihe
  }
  if (!s.bind(addr)) {
      std::cerr << "Error binding socket!" << std::endl;
      s.close();
      exit(-1);
  }

  self_ = std::thread( [=] { receive(); });
}

void Receiver::stop() {
    running_ = false;
    self_.join();
    s.close();
}

void Receiver::receive() {
  static bool once = true;
  while (running_) {
   
    if (once) {
      std::cout << " #### Receiver: This is the receiver thread. Read packets from the network and ";
      std::cout << "push them into JB for further processing. Keep in mind that proper synchronization is necessary. ####" << std::endl;
      once = false;
    }
  }

  while (running_) {
      std::vector<uint8_t> data(128, 0);
      util::Ipv4SocketAddress from;
      s.recvfrom(from, data, 128);

      std::cerr << "Received " << data.size() << " bytes from " << from.toString(true) << std::endl;
      std::string msg(reinterpret_cast<const char*>(&data[0]), data.size());
      std::cerr << "Message: " << msg << std::endl;
  }

}

bool Receiver::isRunning()
{
    return false;
}

void Receiver::get(util::AudioBuffer&)
{
}
