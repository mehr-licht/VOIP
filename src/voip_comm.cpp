/******************************************************************************/
/*
 *  Project: Voice-over-IP
 *  Author: Matthias Tessmann <matthias.tessmann@th-nuernberg.de
 *  Date: October, 5th 2015
 *
 *  Contents: Application wrapper class skeleton.
 *            Intended for educational purposes.
 *
 *  Additoinal notes:
 *         - There is very little error handling.
 *         - The implementation may contain bugs of any kind!
 *
 * The MIT License (MIT)
 *
 *  Copyright (c) 2015 Matthias Teßmann <matthias.tessmann@th-nuernberg.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
 /******************************************************************************/

#include <iostream>
#include "voip_comm.h"
#include "soundcard.h"
#include "receiver.h"
#include "tclap/CmdLine.h"

int VoIPComm::process(util::AudioBuffer& output, util::AudioBuffer const& input) {
	se.send(input);
	r.get(output);
	return 0; // This is currently a no-op
}

VoIPComm::VoIPComm() : soundcard(this), fs_(480), inCh_(1), outCh_(1), inDev_(-1), outDev_(-1), lp_(1976), rp_(1976), s_(24000), packer(NULL), depacker(NULL) {}
VoIPComm::~VoIPComm() {}

int VoIPComm::exec(int argc, char* argv[]) {

	if (!init(argc, argv)) {
		std::cerr << "Error initializing!!" << std::endl;
		return -1;
	}

	{
		char input;
		std::cout << std::endl;
		std::cout << "Press <enter> to start the program." << std::endl;
		std::cin.get(input);
	}

	if (soundcard.start()) {
		char input;
		std::cout << "Now playing...    (Press <enter> to stop)" << std::endl;
		std::cin.get(input);

		r.stop();
		se.stop();

		std::cout << "Receiver and Sender stopped!" << std::endl;

		soundcard.stop();

		std::cout << "Soundcard stopped!" << std::endl;
	}

	if (r.isRunning())
		r.stop();

	if (se.isRunning())
		se.stop();

	// Just wait for enter
	std::cout << std::endl;
	std::cout << "Press <enter> to close..." << std::endl;
	char input;
	std::cin.get(input);

	return 0;
}

bool VoIPComm::init(int argc, char* argv[]) {

	/* Set up command line arguments */
	try {

		TCLAP::CmdLine cmd("VoIP Real-Time Communication Project", ' ', "0.1");

		// This is the only command line argument currently used
		TCLAP::SwitchArg listDevices("l", "list-devices", "List audio devices", cmd, false);

		// These here show you what you might need
		TCLAP::ValueArg<int>          inDev("i", "input-device", "Select input device", false, 1, "int", cmd);//-1  ~1
		TCLAP::ValueArg<int>          outDev("o", "output-device", "Select output device", false, 0, "int", cmd);//-1  ~0
		TCLAP::ValueArg<unsigned int> inCh("", "inCh", "Number of input channels (default: 1)", false, 2, "unsigned int", cmd);//1  ~2
		TCLAP::ValueArg<unsigned int> outCh("", "outCh", "Number of output channels (default 1)", false, 1, "unsigned int", cmd);//1  ~2
		TCLAP::ValueArg<unsigned int> fs("f", "Framesize", "Framesize (default: 512)", false, 480, "unsigned int", cmd);
		TCLAP::ValueArg<unsigned int> s("s", "samplerate", "Samplerate (default: 44100)", false, 24000, "unsigned int", cmd);
		TCLAP::ValueArg<unsigned int> rPort("", "rPort", "Remote Port (default: 1976)", false, 1976, "unsigned int", cmd);
		TCLAP::ValueArg<unsigned int> lPort("", "lPort", "Local Port (default: 1976)", false, 1976, "unsigned int", cmd);
		TCLAP::UnlabeledValueArg<string> destIp("destIp", "Destination IP address (default: 148.71.208.172)", false, "148.71.208.172", "string", cmd);//public
		TCLAP::SwitchArg useOpus("p", "opus", "Use opus encoder", cmd, false);//add
		cmd.parse(argc, argv);

		if (listDevices.getValue()) {
			this->listDevices();
			exit(0);
		}

		// if -l is not specified, the IP is mandatory
		// in order to establish an endpoint connection
		if (destIp.getValue() == "") {
			TCLAP::StdOutput().usage(cmd);
			exit(-1);
		}

		// get command line arguments
		s_ = s.getValue();
		fs_ = fs.getValue();
		rp_ = rPort.getValue();
		lp_ = lPort.getValue();
		inDev_ = inDev.getValue();
		outDev_ = outDev.getValue();
		inCh_ = inCh.getValue();
		outCh_ = outCh.getValue();
		outCh_ = outCh.getValue();
		if (destIp.getValue() == "localhost") {
			destIp_ = LOCALHOST;
		}
		else {
			destIp_ = destIp.getValue();
		}

		jBuffer.config(fs_, outCh_, s_);
		initSoundcard();

		// Wire up all required connections.
		// Then somewhere, start the receiver (this can also become a member variable - its entirely up to you!)
		se.start(destIp_, rp_, 512, useOpus.getValue(), inCh_, s_);//[TODO]macros
		r.start(lp_, &jBuffer, 65536, inCh_, s_);//1024 //[TODO]macros SEM 65536 dá erro no recvfrom

	}
	catch (TCLAP::ArgException& argEx) {
		std::cerr << "Error parsing command line arguments: " << argEx.error() << " for argument " << argEx.argId() << std::endl;
		return false;
	}

	return true;
}

void VoIPComm::listDevices() {
	util::SoundCard::listDevices();
}

void VoIPComm::initSoundcard() {
	soundcard.init(inDev_, outDev_, inCh_, outCh_, s_, fs_, util::AudioBuffer::FLOAT32);//original
	//soundcard.init(inDev_, outDev_, inCh_, outCh_, s_, fs_, util::AudioBuffer::INT16);//sub
}

