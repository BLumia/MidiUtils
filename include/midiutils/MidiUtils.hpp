#ifndef _MIDIUTILS_HPP
#define _MIDIUTILS_HPP

#include <cinttypes>
#include <istream>
#include <fstream>

typedef unsigned char byte;

using namespace std;

namespace MidiUtils {

    //Header Chunk size: 14 byte
    struct FileHeader {
        byte chunkType[4];  //MThd
        byte headerLengh[4];//Always be 6
        byte midiFormat[2]; //type will be 0, 1, or 2
        byte midiTrackCnt[2];
        byte midiDeltaTime[2];
    };

    //Track(Chunk) Header size: 8 byte
    struct TrackHeader {
        byte chunkType[4];
        byte trackLengh[4];
    };

    enum EventType {
        E_INVALID = -1,
        E_META = -2,
        // Meta Types:
        SEQUENCE_NUM = 0x00,
        TEXT_EVENT = 0x01,
        COPYRIGHT = 0x02,
        TRACK_NAME = 0x03,
        INSTRUMENT_NAME = 0x04,
        LYRIC = 0x05,
        MARKER = 0x06,
        CUE_POINT = 0x07,
        CHANNEL_PREFIX = 0x20,
        END_OF_TRACK = 0x2F,
        SET_TEMPO = 0x51,
        SMTPE_OFFSET = 0x54,
        TIME_SIGNATURE = 0x58,
        KEY_SIGNATURE = 0x59,
        Sequencer_Specific_Meta_event = 0x7F,
        // Normal Midi Event Types:
        NOTE_OFF = 0x80,           //1000xxxx 8x (Key is pressed)
        NOTE_ON = 0x90,            //1001xxxx 9x (Key is released)
        KEY_PRESSURE = 0xA0,       //1010xxxx Ax (aka. Key after-touch)
        CONTROL_CHANGE = 0xB0,     //1011xxxx Bx
        PROGRAM_CHANGE = 0xC0,     //1100xxxx Cx (aka. Patch change)
        CHANNEL_PRESSURE = 0xD0,   //1101xxxx Dx (any midifile use it?)
        PITCH_WHEEL_CHANGE = 0xE0, //1110xxxx Ex
        SYSEX
    };

    uint16_t byte2_to_uint16(const byte* buffer);
    uint32_t byte4_to_uint32(const byte* buffer);
    uint32_t readVariableLengthQuantity(istream& istream);
    std::string PrettyKeySignature(const int16_t keysign);
    enum EventType getTypeFromByte(const byte rawType);

}

#endif