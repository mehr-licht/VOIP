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
using namespace std;

class RtpPacker {

public:
	RtpPacker();
	~RtpPacker();

	vector<uint8_t> makePacket(vector<uint8_t>&, uint8_t);

private:
	void makeHeader();//createHeader();
	void setSequenceNoStart();//setStartSequNumber();
	void setTimeStampStart();//setStartTimestamp();
	void setSSRC();//setMySSRC();
	
	vector<uint8_t> header;
	uint8_t		ver;
	uint8_t		p;//padding;
	uint8_t		x;//ext;
	uint8_t		cc;//csrcCnt;
	uint8_t		marker;
	uint8_t		payloadType;//pt;
	uint16_t	sequenceNo;//sqn;
	uint8_t		sequenceNo1;//sqn_b1;
	uint8_t		sequenceNo2;//sqn_b2;
	uint32_t	timeStamp;//ts;
	//uint32_t	ssrc;
	uint8_t		ssrc1;//ssrc_b1;
	uint8_t		ssrc2;//ssrc_b2;
	uint8_t		ssrc3;//ssrc_b3;
	uint8_t		ssrc4;//ssrc_b4;
	uint32_t	previousTime;
};

#endif /* VOIP_RTP_PACKER_H */
