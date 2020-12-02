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

Sender::Sender() : running(false),size_(0) {}

Sender::~Sender() {	if (running) stop(); }

void Sender::start(string rIP, int rPort, int size)
{
	running = true;
	size_ = size;
	//util::Ipv4SocketAddress raddr(destIp.getValue(), rport.getValue()); // está no start
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
			cerr << "Socket is open. Sending the audio to " << raddr.toString(true) << endl;
		}

		//const char* hellostr = "hello";//muidaidiu
		//vector<uint8_t> data(hellostr, hellostr + strlen(hellostr));//muidaidiu
		vector<uint8_t> data(input.size(), 0);// input.size(),0
		::memcpy(&data[0], input.data(), input.size());
		float fractions = (float)data.size() / size_;
		uint32_t send_cnt = 0, i = 0;//hádif tirouoi

		/*while (true) {
			send_cnt = s.sendto(raddr, data);
			cerr << "Sent " << send_cnt << " bytes." << endl;
		}*/
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
			//vector<uint8_t> unit(data.begin() + offset, data.end());
		}
		else {
			tmp = data.begin() + offset + (int8_t)size_;
			//vector<uint8_t> unit(data.begin() + offset, data.begin() + (offset + unitSize));
		}

		vector<uint8_t> unit(data.begin() + offset, tmp);
		prepare(send_cnt, unit);
	}
}

void Sender::prepare(uint32_t& bytesSent, util::ByteBuffer& subData)
{
	bytesSent = s.sendto(raddr, rtpWrapper.makePacket(subData, 10));
	//cout << "Sent Bytes: " << send_cnt << " to: " << raddr.toString(true) << endl;
	subData.clear();
}

void Sender::stop()
{
	s.close();
	if (!s.isOpen())
		running = false;
}
