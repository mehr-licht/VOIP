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

Receiver::Receiver() : self_(), running_(true), jBuffer_(NULL), max_(0), useOpus(false), decoder(NULL) {}

Receiver::~Receiver() {
	if (running_) {
		stop();
	}
}

void Receiver::start(int port, jBuffer* jb_, int max, unsigned int ch_, unsigned int sr_) {
	max_ = max;//max; //if I put 65536 there is no recvfrom error //[TODO]USE MACROS
	jBuffer_ = jb_;
	running_ = true;
	util::Ipv4SocketAddress saddr("", port);
	addr = saddr;
	channels = channels;
	sr = sr_;
	s.open();
	if (s.isOpen()) {
		cerr << "Socket is open. Listening... " << endl;
	}
	if (!s.bind(addr)) {
		cerr << "Error binding socket!" << endl;
		s.close();
		exit(-1);
	}

	self_ = thread([=] { receive(); });
}

void Receiver::stop() {
	running_ = false;
	self_.join();
	s.close();
}

void Receiver::setOpus(bool value)
{
	useOpus = true;
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

		vector<uint8_t> depackedData = rtpDepacker.unpack(data);
		cout << "Received " << data.size() << " bytes from " << from.toString(true) << endl;
		//string msg(reinterpret_cast<const char*>(&data[0]), data.size());
		//cerr << "Message: " << msg << endl;

		if (useOpus) {
			/*vector<uint8_t>::iterator ptr;
			for (ptr = depackedData.begin(); ptr < depackedData.end(); ptr++) {*/


				decoder = new AudioDecoder(sr, channels);
				cout << "OPUS apos new decoder" << endl;//[TODO]remover 
				//[TODO]data=AudioDecoder::Decode(data,size)//vector de entrada e audioBuffer
				opus_int16* newData = decoder->decode(depackedData);

			}

		if (!depackedData.empty())
			jBuffer_->add(depackedData);
	}

}

bool Receiver::isRunning()
{
	return false;
}


void Receiver::get(util::AudioBuffer& scOutput)
{
	//[TODO]decoder = new AudioDecoder(s.getValue(), fs.getValue(), outCh.getValue());//add
	vector<uint8_t> frame = jBuffer_->fetchFrame();
	memcpy(scOutput.data(), &frame[0], frame.size());//tinha ::
}
