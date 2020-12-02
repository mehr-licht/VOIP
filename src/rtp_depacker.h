/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_RTP_DEPACKER_H
#define VOIP_RTP_DEPACKER_H

//function as macro
//#define min(a, b) (a)<(b)?(a):(b)
//bitshifting
//Macro to set nth-bit
/*Set single bit at pos to '1' by generating a mask in the proper bit location and ORing (|) x with the mask.*/
//#define SET_BIT(x, pos) (x |= (1U << pos))
//Macro to clear nth-bit
/*Set single bit at pos to '0' by generating a mask in the proper bit location and Anding x with the mask.*/
//#define CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))
//Macro to toggle nth-bit
/*Set single bit at pos to '1' by generating a mask in the proper bit location and ex-ORing x with the mask.*/
//#define TOGGLE_BIT(x, pos) x ^= (1U<< pos)
//Macro to check nth-bit
/*Set single bit at pos to '1' by generating a mask in the proper bit location and Anding x with the mask. It evaluates 1 if a bit is set otherwise 0.*/
//#define CHECK_BIT(x, pos) (x & (1UL << pos) )
//Macro to swap nibbles
//#define SWAP_NIBBLES(x) ((x & 0x0F)<<4 | (x & 0xF0)>>4)
//Macro to Get bit from the given position
//#define GET_BITS(x, pos) ((x & ( 1 << pos)) >> pos)
//Macro to swap byte of 32-bit +ve integer variable
/* #define SWAP_BYTES(u32Value) ((u32Value & 0x000000FF) << 24)\
|((u32Value & 0x0000FF00) << 8) \
|((u32Value & 0x00FF0000) >> 8) \
|((u32Value & 0xFF000000) >> 24)*/
//macro to swap odd-even bits
//#define SWAP_ODD_EVEN_BIT(x) ((x & 0xAAAAAAAA)>>1 | (x & 0x55555555)<<1);
//macro to swap two numbers
//#define SWAP(x, y) (x ^= y ^= x)
//macro to get low and high bytes
//#define LOWBYTE(v)   ((unsigned char) (x))
//#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (x)) >> 8))

/*MASKS*/
#define VERMASK			0x03 //11
#define PMASK			0x01 //1
#define XMASK			0x01 //1
#define CCMASK			0x0F //1111
#define MARKERMASK		0x01 //1
#define PAYLOADTYPEMASK 0x7F //1111111
#define SHIFTRIGHT		(x, n) ( (x) << (n) )
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

	void checkCSRCLength(std::vector<uint8_t>& packet);

private:
	uint8_t		ver;
	uint8_t		p;//padding;
	uint8_t		x;//ext;
	uint8_t		cc;//csrcCnt;
	uint8_t		marker;
	uint8_t		payloadType;//pt;
	uint16_t	sequenceNo;//sqn;
	//uint8_t		sequenceNo1;//sqn_b1;
	//uint8_t		sequenceNo2;//sqn_b2;
	uint32_t	timeStamp;//ts;
	uint32_t	ssrc;
	//uint8_t		ssrc1;//ssrc_b1;
	//uint8_t		ssrc2;//ssrc_b2;
	//uint8_t		ssrc3;//ssrc_b3;
	//uint8_t		ssrc4;//ssrc_b4;
	
	vector<uint8_t> dataVector;
	vector<uint8_t> csrc;

	uint8_t		padding;
};

#endif /* VOIP_RTP_DEPACKER_H */
