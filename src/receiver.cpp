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


Receiver::Receiver() : self_(), running_(true),jBuffer_(NULL), max_(0) {}

Receiver::~Receiver() {
  if (running_) {
    stop();
  }
}

void Receiver::start(int port, jBuffer* jb_, int max) {
    max_ = 65536;//max; //if I put 65536 there is no recvfrom error //USE MACROS
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
    cout<<endl<<"stop 00";
    running_ = false;
    cout << endl << "stop 01";
    self_.join();
    cout << endl << "stop 02";
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
      vector<uint8_t> data(max_, 0);//2048, 0
      //vector<uint8_t> data(2048, 0);//128, 0
      util::Ipv4SocketAddress from;
      s.recvfrom(from, data, max_);//128
      vector<uint8_t> depackedData = rtpDepacker.unpack(data);//HAHGOHRAH
      cout << "Received " << data.size() << " bytes from " << from.toString(true) << endl;
      string msg(reinterpret_cast<const char*>(&data[0]), data.size());
      cerr << "Message: " << msg << endl;
      if (!depackedData.empty())
          jBuffer_->add(depackedData);
      //jBuffer_->add(data);
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
