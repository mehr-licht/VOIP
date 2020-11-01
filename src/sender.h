/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_SENDER_H
#define VOIP_SENDER_H

#include <string>
#include "socket.h"
#include <audiobuffer.h>

class Sender {
public:
	Sender();
	~Sender();
	util::UdpSocket s;
	void start(std::string, int);
	bool isRunning();
	void send(util::AudioBuffer const&);
	void stop();

private:
	bool running;
	util::Ipv4SocketAddress raddr;
};

#endif /* VOIP_SENDER_H */
