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
#include "simplejb.h"
#include "rtp_depacker.h"
#include "audiodecoder.h"
#include<iterator>
using namespace std;

class Receiver {
public:
	Receiver();
	~Receiver();
	void start(int, jBuffer*, int, unsigned int, unsigned int);
	bool isRunning();
	void get(util::AudioBuffer&);
	void stop();
	void setOpus(bool);
	bool useOpus;

private:
	void receive();
	AudioDecoder* decoder;
	thread self_;
	bool running_;
	util::Ipv4SocketAddress addr;
	util::UdpSocket s;
	jBuffer* jBuffer_;
	int max_;
	RtpDepacker rtpDepacker;
	unsigned int channels;
	unsigned int sr;
};

#endif /* VOIP_RECEIVER_H */
