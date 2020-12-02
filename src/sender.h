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
#include "rtp_packer.h"
using namespace std;

class Sender {
public:
	Sender();
	~Sender();
	
	void start(string, int, int);
	bool isRunning();
	void send(util::AudioBuffer const&);
	void loopToPrepare(float fractions, util::ByteBuffer& data, uint32_t& send_cnt);
	void prepare(uint32_t& send_cnt, util::ByteBuffer& unit);
	void stop();

private:
	bool running;
	util::UdpSocket s;
	util::Ipv4SocketAddress raddr;
	RtpPacker	rtpWrapper;
	int	size_;
};

#endif /* VOIP_SENDER_H */
