#include "MidiUtils.hpp"

namespace MidiUtils {

    uint16_t byte2_to_uint16(byte* buffer) {
        return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
    }

    uint32_t byte4_to_uint32(byte* buffer) {
        return ((uint32_t)(buffer[0]) << 24) | ((uint32_t)(buffer[1]) << 16) |
            ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
    }

}