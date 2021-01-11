/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_RTP_DEPACKER_H
#define VOIP_RTP_DEPACKER_H

 /*MASKS*/
#define VERMASK			0x03
#define PMASK			0x01
#define XMASK			0x01
#define CCMASK			0x0F
#define MARKERMASK		0x01
#define PAYLOADTYPEMASK 0x7F
#define SHIFTRIGHT(x, n) ( (x) << (n) )
#define SHIFTLEFT(x, n) ( (x) >> (n) )
#define BITAND(x, m) ( (x) & (m) )
#define BITOR(x, m) ( (x) | (m) )
#define SHIFTLEFTAND(x, n, m) ( ( (x) >> (n) ) & (m) )
#define SHIFTLEFTOR(x, n, m) ( ( (x) >> (n) ) | (m) )
#define SHIFTRIGHTAND(x, n, m) ( ( (x) << (n) ) & (m) )
#define SHIFTRIGHTOR(x, n, m) ( ( (x) << (n) ) | (m) )
#define SELFBITAND(x, m) ( (x) &= (m) )
#define SELFBITOR(x, m) ( (x) |= (m) )
#define SELFSHIFTLEFTAND(x, n, m) ( ( (x) >> (n) ) &= (m) )
#define SELFSHIFTLEFTOR(x, n, m) ( ( (x) >> (n) ) |= (m) )
#define SELFSHIFTRIGHTAND(x, n, m) ( ( (x) << (n) ) &= (m) )
#define SELFSHIFTRIGHTOR(x, n, m) ( ( (x) << (n) ) |= (m) )
#define JOIN4BYTES(x, y, z, w) ( ( (x) << 24 ) | ( (y) << 16 ) | ( (z) << 8 ) | ( (w) ) )

#include <stdint.h>
#include <vector>
using namespace std;

class RtpDepacker {
public:
	RtpDepacker();
	~RtpDepacker();

	vector<uint8_t> unpack(std::vector<uint8_t>&);
	void checkCSRCLength(std::vector<uint8_t>&);

private:
	uint8_t  ver;
	uint8_t  p;
	uint8_t  x;
	uint8_t  cc;
	uint8_t  marker;
	uint8_t  payloadType;
	uint16_t sequenceNo;
	uint32_t timeStamp;
	uint32_t ssrc;
	vector<uint8_t> dataVector;
	vector<uint8_t> csrc;
	uint8_t  padding;
};

#endif /* VOIP_RTP_DEPACKER_H */
