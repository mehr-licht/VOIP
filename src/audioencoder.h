/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_AUDIOENCODER_H
#define VOIP_AUDIOENCODER_H

#define FRAME_SIZE 960      // 20ms frame size (sample_rate * ms = frame_size)
#define SAMPLE_RATE 48000
#define BITRATE 24000
#define BITDEPTH 16
#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_VOIP
#define MAX_FRAME_SIZE 6*960
#define MAX_PACKET_SIZE (3*1276)

#include "opus.h"
#include "audiobuffer.h"

using namespace std;

class AudioEncoder {
public:
	AudioEncoder(opus_int32, int);
	//static unsigned char* Encoder(float*, unsigned char*);
	~AudioEncoder();

	//vector<unsigned char> EncodeFrame(const vector<opus_int16>::const_iterator&, int);
	vector<uint8_t> encode(util::ByteBuffer&, int);
private:
	OpusEncoder* opus;
	unsigned int fs;
	unsigned int channels;
};

#endif /* VOIP_AUDIOENCODER_H */
