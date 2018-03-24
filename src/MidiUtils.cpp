#include <istream>
#include <string>
#include <fstream>
#include "midiutils/MidiUtils.hpp"

namespace MidiUtils {

    uint16_t byte2_to_uint16(const byte* buffer) {
        return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
    }

    uint32_t byte4_to_uint32(const byte* buffer) {
        return ((uint32_t)(buffer[0]) << 24) | ((uint32_t)(buffer[1]) << 16) |
            ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
    }

    uint32_t readVariableLengthQuantity(istream& istream) {
        char bt;
        uint32_t ret = 0;

        // bt less than 128 then read new one and
        do {
            istream.get(bt);
            ret = (ret << 7) | (bt & 0x7F);
        } while ((bt & 0x80) == 0x80);

        return ret;
    }

    std::string PrettyKeySignature(const int16_t keysign) {
		static const std::string fsStrTable[2][13] = {
			{"Gb", "Db", "Ab", "Eb", "Bb", "F", "C", "G", "D", "A",  "E",  "B",  "F#"},
			{"eb", "bb", "f",  "c",  "g",  "d", "a", "e", "b", "f#", "c#", "g#", "d#"}
		};
		static const std::string mmStrTable[2] = {"Major", "Minor"};
        int8_t flatsharp = keysign >> 8;
        int8_t majorminor = keysign & 0x00FF;
        return fsStrTable[majorminor][flatsharp + 6] + " " + mmStrTable[majorminor];
    }

    enum EventType getTypeFromByte(const byte rawType) {
        switch(rawType & 0xF0) {
            case NOTE_OFF: 
            case NOTE_ON:
            case KEY_PRESSURE:
            case CONTROL_CHANGE:
            case PROGRAM_CHANGE:
            case CHANNEL_PRESSURE:
            case PITCH_WHEEL_CHANGE:
                return static_cast<enum EventType>(rawType & 0xF0);
            case 0xF0: 
                if ((rawType & 0x0F) == 0x0F) {
                    return E_META;
                } else if ((rawType & 0x0F) == 0x00 || (rawType & 0x0F) == 0x07) {
                    return SYSEX;
                } else {
                    return E_INVALID;
                }
            default:
                return E_INVALID;
        }
    }

}