/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "simplejb.h"
using namespace std;

jBuffer::jBuffer() :
	frameSize_(512),
	nChannels_(2),
	sampleRate_(44100),
	format_(util::AudioBuffer::FLOAT32),
	locked(false)
{}

jBuffer::~jBuffer() {}

void jBuffer::add(vector<uint8_t> data) {
	if (!locked) {
		setLock(true);
		addToBufferW();
		addToBuffer(data);
		setLock(false);
	}
	else {
		addToWaiting(data);
	}
}

void jBuffer::addToBufferW()
{
	if (waitingPackages.size()) {
		for (size_t i = 0; i < waitingPackages.size(); i++)
		{
			buffer.push_back(waitingPackages[i]);
		}
		waitingPackages.clear();
	}
}

void jBuffer::addToBuffer(util::ByteBuffer& data)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		buffer.push_back(data[i]);
	}
}

void jBuffer::addToWaiting(util::ByteBuffer& data)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		waitingPackages.push_back(data[i]);
	}
}

void jBuffer::config(int fs, unsigned int nCh, int sr) {
	jBuffer::checkLocking();
	frameSize_ = fs;
	nChannels_ = nCh;
	sampleRate_ = sr;
	buffer.clear();
	waitingPackages.clear();
	setLock(false);
}

vector<uint8_t> jBuffer::fetchFrame() {
	jBuffer::checkLocking();
	uint32_t nBytes = frameSize_ * nChannels_ * (format_ / 8);
	vector<uint8_t> frame(nBytes, 0);

	if (!buffer.size()) {
		setLock(false);
		return frame;
	}

	checkCapacity(nBytes, frame);
	setLock(false);
	return frame;
}

void jBuffer::checkCapacity(const uint32_t& nBytes, util::ByteBuffer& frame)
{
	if (buffer.size() >= nBytes) {
		::memcpy(&frame[0], &buffer[0], nBytes);
		buffer.erase(buffer.begin(), buffer.begin() + nBytes);
	}
}

void jBuffer::checkLocking() {
	if (!locked) {
		setLock(true);
		checkWaitSize();
	}
	else {
		while (locked) {}
		jBuffer::checkLocking();
	}
}

void jBuffer::checkWaitSize()
{
	if (waitingPackages.size()) {
		for (size_t i = 0; i < waitingPackages.size(); i++)
		{
			buffer.push_back(waitingPackages[i]);
		}
		waitingPackages.clear();
	}
}


void jBuffer::setLock(bool lock) {
	locked = lock;
}