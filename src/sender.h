/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_SENDER_H
#define VOIP_SENDER_H

#define MAX_FRAME_SIZE	6*960
#define PAYLOAD_TYPE	10//[TODO]remover 10 é o PT. está certo para o opus. é para L16	2ch	44100KHz

#include <string>
#include "socket.h"
#include <audiobuffer.h>
#include "rtp_packer.h"
#include "audioencoder.h"

using namespace std;

class Sender {
public:
	Sender();
	~Sender();
	
	void start(string, int, int, bool, int, int);
	bool isRunning();
	void send(util::AudioBuffer const&);
	void loopToPrepare(float, util::ByteBuffer&, uint32_t&);
	void prepare(uint32_t&, util::ByteBuffer&);
	void stop();
	bool useOpus;
private:
	int sr;
	int ch;
	bool running;
	util::UdpSocket s;
	util::Ipv4SocketAddress raddr;
	RtpPacker rtpWrapper;
	int	size_;
	void setOpus();
	AudioEncoder* encoder;
};

#endif /* VOIP_SENDER_H */
