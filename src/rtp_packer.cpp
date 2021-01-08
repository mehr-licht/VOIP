/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "rtp_packer.h"
using namespace std;

RtpPacker::RtpPacker() : ver(2),p(0),//padding(0),
	x(0),//	ext(0),
	cc(0),//	csrcCnt(0),
	marker(0),	payloadType(0),//pt(0),
	sequenceNo(0),//	sqn(0),
	timeStamp(0)//,//ts(0),
	//ssrc(0)
{
	setSequenceNoStart();//setStartSequNumber();
	setSSRC();//	setMySSRC();
	setTimeStampStart();//setStartTimestamp();
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
	//ssrc = 0;
	//ssrc = (ssrc1 << 8) | ssrc2;
	//ssrc = (ssrc1 << 24) | ssrc2 << 16 | (ssrc3 << 8) | ssrc4;
	//o de cima era diferente ssrc = (ssrc << 8) | ssrc4;
}

void RtpPacker::setTimeStampStart() {
	previousTime = time(0);
	srand(time(0));
	//uint8_t timeStamp1 = rand() % 256;//ts_b4 
	//uint8_t timeStamp2 = rand() % 256;
	//uint8_t timeStamp3 = rand() % 256;
	//uint8_t timeStamp4 = rand() % 256;
	//timeStamp = 0;
	//timeStamp = rand() % 4294967295;//meue soeh p�ra ficareh
	timeStamp = (rand() % 256 << 24) | (rand() % 256 << 16) | (rand() % 256 << 8) | rand() % 256;
	//timeStamp = (timeStamp << 8) | timeStamp3;
	//timeStamp = (timeStamp << 8) | timeStamp4;
}

void RtpPacker::makeHeader() {
	header.clear();

	//uint8_t byte1 = 0;
	//uint8_t byte2 = 0;

	uint8_t B1 = (ver << 6) | (p << 5) | (x <<4) | cc;
	//B1 = (B1 << 1) | x;
	//B1 = (B1 << 4) | cc;
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

vector<uint8_t> RtpPacker::makePacket(vector<uint8_t>& data, uint8_t payloadType_) {
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