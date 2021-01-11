/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_AUDIODECODER_H
#define VOIP_AUDIODECODER_H

#define MAX_FRAME_SIZE 6*960
#define MAX_PACKET_SIZE (3*1276)

#include "opus.h"
#include "audiobuffer.h"
using namespace std;

class AudioDecoder {
public:
	AudioDecoder( unsigned int, unsigned int);
	opus_int16* decode(vector<uint8_t>);//(unsigned char*);//, util::AudioBuffer&);
	//vector<opus_int16> Decode_p(const std::vector<unsigned char>&, int, bool);
private:
	OpusDecoder* opus;
	unsigned int fs;
	unsigned int sr;
	unsigned int channels;
};

#endif /* VOIP_AUDIODECODER_H */
