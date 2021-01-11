/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "rtp_packer.h"
using namespace std;

RtpPacker::RtpPacker() : ver(2), p(0), x(0), cc(0), marker(0), payloadType(0), sequenceNo(0), timeStamp(0)
{
	setSequenceNoStart();
	setSSRC();
	setTimeStampStart();
}

RtpPacker::~RtpPacker() {}

void RtpPacker::setSequenceNoStart() {
	srand(time(0));
	sequenceNo1 = rand() % 256;
	sequenceNo2 = rand() % 256;
	sequenceNo = (sequenceNo1 << 8) | sequenceNo2;
}

void RtpPacker::setSSRC() {
	srand(time(0));
	ssrc1 = rand() % 256;
	ssrc2 = rand() % 256;
	ssrc3 = rand() % 256;
	ssrc4 = rand() % 256;
}

void RtpPacker::setTimeStampStart() {
	previousTime = time(0);
	srand(time(0));
	timeStamp = (rand() % 256 << 24) | (rand() % 256 << 16) | (rand() % 256 << 8) | rand() % 256;
}

void RtpPacker::makeHeader() {
	header.clear();

	uint8_t B1 = (ver << 6) | (p << 5) | (x << 4) | cc;
	header.push_back(B1);
	uint8_t B2 = (marker << 7) | payloadType;
	header.push_back(B2);

	header.push_back(sequenceNo1);
	header.push_back(sequenceNo2);

	uint8_t* timeStamp_ = (uint8_t*)&timeStamp;
	header.push_back(timeStamp_[3]);
	header.push_back(timeStamp_[2]);
	header.push_back(timeStamp_[1]);
	header.push_back(timeStamp_[0]);

	header.push_back(ssrc1);
	header.push_back(ssrc2);
	header.push_back(ssrc3);
	header.push_back(ssrc4);
}

vector<uint8_t> RtpPacker::makePacket(vector<uint8_t>& data, uint8_t payloadType_)
{
	uint32_t currentTime = time(0);
	timeStamp += currentTime - previousTime;
	previousTime = currentTime;
	payloadType = payloadType_;

	makeHeader();

	vector<uint8_t> rtpPacket(header.size() + data.size(), 0);
	::memcpy(&rtpPacket[0], &header[0], header.size());
	::memcpy(&rtpPacket[header.size()], &data[0], data.size());

	sequenceNo++;
	return rtpPacket;
}