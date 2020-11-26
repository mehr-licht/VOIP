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

Sender::Sender() : running(false) {}

Sender::~Sender() {	if (running) stop(); }

void Sender::start(string receiverIp, int receiverPort)
{
	running = true;
	//util::Ipv4SocketAddress raddr(destIp.getValue(), rport.getValue()); // está no start
	util::Ipv4SocketAddress address(receiverIp, receiverPort);
	raddr = address;

	if (s.open())
		cout << "Socket to " << raddr.toString() << " is open." << endl;
	else
		cout << "Socket connection failed!" << endl;
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

		uint32_t send_cnt = 0, i = 0;//hádif tirouoi

		while (true) {
			send_cnt = s.sendto(raddr, data);
			cerr << "Sent " << send_cnt << " bytes." << endl;
		}
		
	}
	else
		cerr << "Sender not started" << endl;
}

void Sender::stop()
{
	s.close();
	if (!s.isOpen())
		running = false;
}
