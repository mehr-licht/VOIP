/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "audioencoder.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

AudioEncoder::AudioEncoder(opus_int32 fs, int channelsP) : fs(fs), channels(channelsP)
{
    int error;
    opus = opus_encoder_create(fs, channels, OPUS_APPLICATION_VOIP, &error);

    if (error != OPUS_OK) {
        cout << opus_strerror(error);
        throw("failed creating opus");
    }
}
/*/
unsigned char* AudioEncoder::Encoder(float* pcm, unsigned char* data)
{
    opus_encode_float(opus, pcm, fs, data, MAX_PACKET_SIZE);//max_data_bytes//1000
    //tcomp.cbits = new unsigned char;
    //tcomp.nbBytes = opus_encode_float(enc, data.recordedSamples, FRAME_SIZE, tcomp.cbits, MAX_PACKET_SIZE);
    return data;
}
*/

AudioEncoder::~AudioEncoder()
{
    opus_encoder_destroy(opus);
}


/*
   OpusEncoder *enc;
   OpusMSEncoder *MSenc;

 enc = opus_encoder_create(48000, 2, OPUS_APPLICATION_VOIP, &err);
   if(err != OPUS_OK || enc==NULL)test_failed();
   MSenc = opus_multistream_encoder_create(8000, 2, 2, 0, mapping, OPUS_APPLICATION_AUDIO, &err);

   
   OpusEncoder *enccpy;
      The opus state structures contain no pointers and can be freely copied
enccpy = (OpusEncoder*)malloc(opus_encoder_get_size(2));
memcpy(enccpy, enc, opus_encoder_get_size(2));
memset(enc, 255, opus_encoder_get_size(2));
opus_encoder_destroy(enc);
enc = enccpy;
   */

/*
dec = opus_decoder_create(48000, 2, &err);
   if(err != OPUS_OK || dec==NULL)test_failed();

   MSdec = opus_multistream_decoder_create(48000, 2, 2, 0, mapping, &err);
   if(err != OPUS_OK || MSdec==NULL)test_failed();

   MSdec_err = opus_multistream_decoder_create(48000, 3, 2, 0, mapping, &err);
   if(err != OPUS_OK || MSdec_err==NULL)test_failed();

   dec_err[0]=(OpusDecoder *)malloc(opus_decoder_get_size(2));*/


//void *memcpy(void *dest, const void *src, size_t n);
//The memcpy() function copies n bytes from memory area src to memory area dest.
//The memory areas must not overlap.Use memmove(3) if the memory areas do overlap.


vector<uint8_t> AudioEncoder::encode(util::ByteBuffer& subData, int ch)
{
    //cout << "encode00" << endl;
    size_t size = MAX_FRAME_SIZE * (size_t)ch * sizeof(short*); //size é 92160
    //cout << "encode01" << endl;
    opus_int16* in;
    //opus_int16 in[FRAME_SIZE * CHANNELS];//substituido pela anterior e seguinte - apagar
    in = (opus_int16*) malloc(FRAME_SIZE * channels);
   // short* in = (short*)malloc(size);
    unsigned char cbits[MAX_PACKET_SIZE];
    memset(cbits, 0, sizeof(cbits));
    //cout << "encode02" << endl;
    vector<uint8_t> out(size, 0);
    //opus_int16 opusIn[size];
    //cout << "encode03" << endl;
    //cout << "size: "<<size << endl;
    //memcpy(in, &subData, sizeof(&subData));//com 1024 já dá access violation // 960 passa
    //cout << "encode04" << endl;
    //int fs = subData.frameSize();
    int nbBytes = opus_encode(opus, in, FRAME_SIZE, cbits, MAX_PACKET_SIZE);
    //opus_int32 resSize = opus_encode(opus, in, fs, out.data(), MAX_PACKET_SIZE);
    cout << "encode05" << endl;
    cout << "resSize: " << nbBytes<<endl;
    if (nbBytes < 0)
        throw("encoding error");
    cout << "encoded size: " << nbBytes << endl;
    out.resize(nbBytes);
    return out;
}


//outra vers
/*
vector<unsigned char> AudioEncoder::EncodeFrame(const vector<opus_int16>::const_iterator& frame_start, int frame_size) {
    const auto frame_length = (frame_size * channels * sizeof(*frame_start));
    vector<unsigned char> encoded(frame_length);
    auto num_bytes = opus_encode(opus, &*frame_start, frame_size, encoded.data(), encoded.size());
    if (num_bytes < 0) {
        cout << "Encode error: " << opus_strerror(num_bytes);
        return {};
    }
    encoded.resize(num_bytes);
    return encoded;
}*/

