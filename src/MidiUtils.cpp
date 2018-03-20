#include <istream>
#include <string>
#include <fstream>
#include "midiutils/MidiUtils.hpp"

namespace MidiUtils {

    uint16_t byte2_to_uint16(byte* buffer) {
        return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
    }

    uint32_t byte4_to_uint32(byte* buffer) {
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

    std::string PrettyKeySignature(int16_t keysign) {
		static const std::string fsStrTable[2][13] = {
			{"Gb", "Db", "Ab", "Eb", "Bb", "F", "C", "G", "D", "A",  "E",  "B",  "F#"},
			{"eb", "bb", "f",  "c",  "g",  "d", "a", "e", "b", "f#", "c#", "g#", "d#"}
		};
		static const std::string mmStrTable[2] = {"Major", "Minor"};
        int8_t flatsharp = keysign >> 8;
        int8_t majerminor = keysign & 0x00FF;
        return fsStrTable[majerminor][flatsharp + 6] + " " + mmStrTable[majerminor];
    }

}