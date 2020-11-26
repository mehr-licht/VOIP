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
using namespace std;

int VoIPComm::process(util::AudioBuffer& output, util::AudioBuffer const& input) {
    //s.send(input);
    //r.get(output);
    return 0; // This is currently a no-op
}

//in 3 = laptop realtek micro stereo
//out 0= BT phones stereo
//out 2= laptop realtek speakers stereo
VoIPComm::VoIPComm() : soundcard(this), s_(44100), fs_(512), inDev_(3), outDev_(0), inCh_(2), outCh_(2), rp_(1976), lp_(1976), destIp_("") {}
VoIPComm::~VoIPComm() {}

int VoIPComm::exec(int argc, char* argv[]) {

    if (!init(argc, argv)) {
        cerr << "Error initializing!" << endl;
        return -1;
    }
    
   
    // Start the sound card and process I/O
    //cout << " ** Now you should start the soundcard and begin to process I/O." << endl;
    //cout << " ** Note: because the receiver runs in parallel, you may read this message before the message printed" << endl;
    //cout << " ** by the receiver or they may even be interleaved!" << endl;
    //cout << endl;
    //cout << endl << " ** Actually, this implementation is just a dummy. Press enter to exit..." << endl;

    // Just wait for enter
    char input;
    //  if (system("CLS")) system("clear");
    cout << "Press ENTER to start " << endl;
    cin.get(input);

    if (soundcard.start()) {
        char input;
        cout << "Working - press ENTER to stop" << endl;
        cin.get(input);

        r.stop();
        se.stop();
        soundcard.stop();

        cout << "Soundcard stopped!" << endl;
    }
    r.~Receiver();//apagar
    se.~Sender();//apagar
}

bool VoIPComm::init(int argc, char* argv[]) {

    /* Set up command line arguments */
    try {

        TCLAP::CmdLine cmd("VoIP Real-Time Communication Project", ' ', "0.1");

        // This is the only command line argument currently used
        TCLAP::SwitchArg listDevices("l", "list-devices", "List audio devices", cmd, false);

        // These here show you what you might need
        TCLAP::ValueArg<int>          inDev("i", "input-device", "Select input device", false, -1, "int", cmd);
        TCLAP::ValueArg<int>          outDev("o", "output-device", "Select output device", false, -1, "int", cmd);
        TCLAP::ValueArg<unsigned int> inCh("", "inCh", "Number of input channels (default: 1)", false, 1, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> outCh("", "outCh", "Number of output channels (default 1)", false, 1, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> fs("f", "Framesize", "Framesize (default: 512)", false, 512, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> s("s", "samplerate", "Samplerate (default: 44100)", false, 44100, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> rPort("", "rPort", "Remote Port (default: 1976)", false, 1976, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> lPort("", "lPort", "Local Port (default: 1976)", false, 1976, "unsigned int", cmd);
        TCLAP::UnlabeledValueArg<string> destIp("destIp", "Destination IP address", false, "192.168.1.199", "string", cmd);

        cmd.parse(argc, argv);

        /* Add argument processing here */
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
        cout << destIp.getValue() << endl;
        //assign cmdline arguments to variables
        s_ = s.getValue();
        fs_ = fs.getValue();
        rp_ = rPort.getValue();
        lp_ = lPort.getValue();
        inDev_ = inDev.getValue();
        outDev_ = outDev.getValue();
        inCh_ = inCh.getValue();
        outCh_ = outCh.getValue();
        destIp_ = destIp.getValue();
        jBuffer.config(fs_, outCh_, s_);
        initSoundcard();
        cout << "destIp_: " << destIp_ << " ; rp_:" << rp_ << endl;
        se.start(destIp_, rp_);
        r.start(lp_, &jBuffer);
    }
    catch (TCLAP::ArgException& argEx) {
        cerr << "Error parsing command line arguments: " << argEx.error() << " for argument " << argEx.argId() << endl;
        return false;
    }

    return true;
}


void VoIPComm::listDevices() {
    util::SoundCard::listDevices();
}

void VoIPComm::initSoundcard()
{
    soundcard.init(-1, -1, 1, 1, s_, fs_, util::AudioBuffer::FLOAT32);
}
