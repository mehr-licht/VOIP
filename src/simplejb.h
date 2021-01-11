/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_SIMPLEJB_H
#define VOIP_SIMPLEJB_H

#include <vector>
#include "audiobuffer.h"
using namespace std;

class jBuffer {
public:
	jBuffer();
	~jBuffer();

	void add(vector<uint8_t>);
	void addToBufferW();
	void addToBuffer(util::ByteBuffer&);
	void addToWaiting(util::ByteBuffer&);
	vector<uint8_t> fetchFrame();
	void checkCapacity(const uint32_t&, util::ByteBuffer&);
	void config(int, unsigned int, int);

private:
	void checkLocking();
	void checkWaitSize();
	void setLock(bool);
	vector<uint8_t> buffer;
	vector<uint8_t> waitingPackages;
	int frameSize_;
	unsigned int nChannels_;
	int sampleRate_;
	util::AudioBuffer::SampleFormat format_;
	bool locked;
};

#endif /* VOIP_jBuffer_H */
