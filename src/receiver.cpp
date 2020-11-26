/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "receiver.h"
#include <iostream>
using namespace std;


Receiver::Receiver()
  : self_(),
    running_(true)
{}

Receiver::~Receiver() {
  if (running_) {
    stop();
  }
}

void Receiver::start(int port, jBuffer* jb_) {
  jBuffer_ = jb_;
  running_ = true;
  util::Ipv4SocketAddress saddr("", port);
  addr = saddr;
  s.open();
  if (s.isOpen()) {//metihe
      cerr << "Socket is open. Listening... " << endl;//metihe
  }
  if (!s.bind(addr)) {
      cerr << "Error binding socket!" << endl;
      s.close();
      exit(-1);
  }

  self_ = thread( [=] { receive(); });
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
      cout << " #### Receiver: This is the receiver thread. Read packets from the network and ";
      cout << "push them into JB for further processing. Keep in mind that proper synchronization is necessary. ####" << endl;
      once = false;
    }
  }

  while (running_) {
      vector<uint8_t> data(2048, 0);//128, 0
      util::Ipv4SocketAddress from;
      s.recvfrom(from, data, 128);

      cerr << "Received " << data.size() << " bytes from " << from.toString(true) << endl;
      string msg(reinterpret_cast<const char*>(&data[0]), data.size());
      cerr << "Message: " << msg << endl;

      jBuffer_->add(data);
  }

}

bool Receiver::isRunning()
{
    return false;
}


void Receiver::get(util::AudioBuffer& scOutput) 
{
    vector<uint8_t> frame = jBuffer_->fetchFrame();
    ::memcpy(scOutput.data(), &frame[0], frame.size());
}
