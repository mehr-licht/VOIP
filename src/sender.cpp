/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "sender.h"	
#include <iostream>"
using namespace std;

Sender::Sender() : running(false), size_(0), useOpus(false), ch(2), sr(24000), encoder(NULL) {}

Sender::~Sender() { if (running) stop(); }

void Sender::start(string rIP, int rPort, int size, bool useOpus, int channels, int samplr)
{
	if (useOpus)
		setOpus();

	sr = samplr;
	ch = channels;
	running = true;
	size_ = size;
	util::Ipv4SocketAddress address(rIP, rPort);
	raddr = address;

	if (!s.open())
		cout << "Socket connection failed!" << endl;
	else
		cout << "Socket to " << raddr.toString() << " is open." << endl;
}

bool Sender::isRunning()
{
	return false;
}

void Sender::send(util::AudioBuffer const& input)
{
	if (running) {

		if (s.isOpen()) {
			cerr << "Socket is open. Sending the audio to " << raddr.toString(false) << endl;
		}

		vector<uint8_t> data(input.size(), 0);
		::memcpy(&data[0], input.data(), input.size());
		float fractions = (float)data.size() / size_;
		uint32_t send_cnt = 0, i = 0;

		loopToPrepare(fractions, data, send_cnt);
	}
	else
		cerr << "Sender not started" << endl;
}

void Sender::loopToPrepare(float fractions, util::ByteBuffer& data, uint32_t& send_cnt)
{
	for (size_t i = 0; i < fractions; i++)
	{
		int offset = i * size_;
		util::ByteBuffer::iterator tmp;
		if (fractions >= i + 1) {
			tmp = data.end();
		}
		else {
			tmp = data.begin() + offset + (int8_t)size_;
		}

		vector<uint8_t> unit(data.begin() + offset, tmp);
		prepare(send_cnt, unit);
	}
}


void Sender::setOpus()
{
	useOpus = true;
}

void Sender::prepare(uint32_t& bytesSent, util::ByteBuffer& subData)
{
	util::ByteBuffer& out = subData;
	if (useOpus) {
		encoder = new AudioEncoder(sr, ch);
		cout << "OPUS apos new encoder" << endl;//[TODO]remover 
		out = encoder->encode(subData, ch);//(util::AudioBuffer const& in)
		cout << "OPUS apos create" << endl;//[TODO]remover 
		//[TODO]remover  out = AudioEncoder::Encoder(uint32_t& bytesSent, util::ByteBuffer& out)
	}
	bytesSent = s.sendto(raddr, rtpWrapper.makePacket(out, PAYLOAD_TYPE));
	cout << "Sent Bytes: " << bytesSent << " to: " << raddr.toString(true) << endl;
	subData.clear();
}

void Sender::stop()
{
	s.close();
	if (!s.isOpen())
		running = false;
}
