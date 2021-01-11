/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "audiodecoder.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

AudioDecoder::AudioDecoder(unsigned int srP, unsigned int channelsP) :
   // fs(fsP),
    channels(channelsP),
    sr(srP)
{
    int error;
    opus = opus_decoder_create(sr, channelsP, &error);

    if (error != OPUS_OK) {
        cout << opus_strerror(error) << endl;
        throw("failed creating opus decoder");
    }
}

opus_int16* AudioDecoder::decode(vector<uint8_t> in)//(unsigned char* in)//, util::AudioBuffer& to
{
    //unsigned char cbits[MAX_PACKET_SIZE];
    //size_t size = MAX_FRAME_SIZE * (size_t)channels * sizeof(short*);
    //short* out = (short*)malloc(size);
    opus_int16* out;
    //opus_int16 out[MAX_FRAME_SIZE * 2];//substituido por linha anterior e seguinte - apagar
    out = (opus_int16*)malloc(MAX_FRAME_SIZE * (size_t)channels);
    //opus_int16 out[frs * channels];

    unsigned char* newIn = new unsigned char[in.size()];//init this with the correct size
    std::copy(in.begin(), in.end(), newIn);

    size_t outSize = opus_decode(opus, newIn, 209, out, MAX_FRAME_SIZE, 0);//209 é nbBytes encodado [TODO] como saber deste lado quanto?
    if (outSize < 0)
    {
        cout <<"failed decoding Opus" << endl;
    }
    //size_t outSize = opus_decode(opus, in.data(), in.size(), out, fs, 0);
    //outSize *= channels;

    //size_t toSize = to.size();
    //if ((uint32_t)toSize != outSize * (uint32_t)2)
    //    throw("wrong size");

    //memcpy(to.data(), out, to.size());
    return out;
}

//outra vers
//vector<opus_int16> AudioDecoder::Decode_p(const std::vector<unsigned char>& packet, int frame_size, bool decode_fec) {
//    const auto frame_length = (frame_size * channels * sizeof(opus_int16));
//    std::vector<opus_int16> decoded(frame_length);
//    auto num_samples = opus_decode(opus, packet.data(), packet.size(),
//        decoded.data(), frame_size, decode_fec);
//    if (num_samples < 0) {
//        cout << "Decode error: " << opus_strerror(num_samples) << endl;
//        return {};
//    }
//    decoded.resize(num_samples * channels);
//    return decoded;
//}

//
//util::ByteBuffer& AudioDecoder::unOpus()
//{
//    return util::ByteBuffer();
//}
