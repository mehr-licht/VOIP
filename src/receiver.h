/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_RECEIVER_H
#define VOIP_RECEIVER_H

#include <thread>
#include <iostream>
#include <audiobuffer.h>
#include "socket.h"

class Receiver {
public:
  Receiver();
  ~Receiver();
  //void start(int, SimpleJB*, int);
  void start(int);
  bool isRunning();
  void get(util::AudioBuffer&);
  void stop();

  

private:
  void receive();

  std::thread self_;
  bool        running_;

  //int			port;
  util::Ipv4SocketAddress addr;
  util::UdpSocket s;

  //int			maxUnitSize;

  //unsigned int	outCh_;
};

#endif /* VOIP_RECEIVER_H */
