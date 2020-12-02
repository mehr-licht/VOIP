/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "rtp_depacker.h"
using namespace std;

RtpDepacker::RtpDepacker() : ver(0), p(0), x(0), cc(0),	marker(0), payloadType(0), sequenceNo(0), timeStamp(0), ssrc(0), padding(0) {}

RtpDepacker::~RtpDepacker() {}

vector<uint8_t> RtpDepacker::unpack(std::vector<uint8_t>& packet) {
	if (!packet.size()) {
		return dataVector;
	}
	dataVector.clear();
	csrc.clear();
	/*uint8_t packet_0 = packet.at(0);
	uint8_t packet_1 = packet.at(1);*/

	ver = SHIFTLEFTAND(packet.at(0), 6, VERMASK);
	p = SHIFTLEFTAND(packet.at(0), 5, PMASK);
	x = SHIFTLEFTAND(packet.at(0), 4, XMASK);
	cc = BITAND(packet.at(0), CCMASK);
	marker = SHIFTLEFTAND(packet.at(1), 7, MARKERMASK);
	payloadType = BITAND(packet.at(1), PAYLOADTYPEMASK);

	if (ver !=2 || !(payloadType == 10 || payloadType == 11))
		return dataVector;

	//sqn_b1 = packet.at(2);
	//sqn_b2 = packet.at(3);
	//sequenceNo = 0;
	sequenceNo = SHIFTRIGHTOR(packet.at(2), 8, packet.at(3));

	/*timeStamp1 = packet.at(4);
	timeStamp2 = packet.at(5);
	timeStamp3 = packet.at(6);
	timeStamp4 = packet.at(7);
	timeStamp = 0;*/
	timeStamp = JOIN4BYTES(packet.at(4),packet.at(5),packet.at(6),packet.at(7));

	//ssrc1 = packet.at(8);
	//ssrc2 = packet.at(9);
	//ssrc3 = packet.at(10);
	//ssrc4 = packet.at(11);
	//ssrc = 0;
	ssrc = JOIN4BYTES(packet.at(8),packet.at(9),packet.at(10),packet.at(11));

	checkCSRCLength(packet);

	if (p != 1) {
		padding = 0;
	}
	else {
		padding = packet.back();
	}
	/*padding = 0;
	if (p == 1) {
		padding = packet.back();
	}*/

	int headerSize = 12 + 4 * cc;
	uint8_t payloadSize = packet.size() - ((uint8_t)headerSize + padding);

	dataVector.resize(payloadSize, 0);

	::memcpy(&dataVector[0], &packet[headerSize], payloadSize);

	return dataVector;
}
void RtpDepacker::checkCSRCLength(std::vector<uint8_t>& packet)
{
	if (cc > 0) {
		for (size_t i = 0; i < cc; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				csrc.push_back(packet.at(12 + 4 * i + j));
			}
		}
	}
}
