#include <istream>
#include <fstream>
#include "MidiUtils.hpp"

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
        int8_t flatsharp = keysign >> 8;
        int8_t majerminor = keysign & 0x00FF;
        return std::to_string(flatsharp) + " " + std::to_string(majerminor); 
    }

}