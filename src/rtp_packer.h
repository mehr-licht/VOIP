/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_RTP_PACKER_H
#define VOIP_RTP_PACKER_H

#include <vector>
#include <ctime>
#include "audioencoder.h"

using namespace std;

class RtpPacker {

public:
	RtpPacker();
	~RtpPacker();

	vector<uint8_t> makePacket(vector<uint8_t>&, uint8_t);

private:
	void makeHeader();
	void setSequenceNoStart();
	void setTimeStampStart();
	void setSSRC();
	AudioEncoder* encoder;
	vector<uint8_t> header;
	uint8_t ver;
	uint8_t p;
	uint8_t x;
	uint8_t cc;
	uint8_t marker;
	uint8_t  payloadType;
	uint16_t sequenceNo;
	uint8_t  sequenceNo1;
	uint8_t  sequenceNo2;
	uint32_t timeStamp;
	uint8_t  ssrc1;
	uint8_t  ssrc2;
	uint8_t  ssrc3;
	uint8_t  ssrc4;
	uint32_t previousTime;
};

#endif /* VOIP_RTP_PACKER_H */
