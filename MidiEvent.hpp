#ifndef __MIDIEVENT_HPP
#define __MIDIEVENT_HPP

#include <cstdint>
#include <cstring>
#include "MidiUtils.hpp"

using namespace std;

namespace MidiUtils {

    class MidiEvent {

    public:
        MidiEvent();
        MidiEvent(uint32_t tickTime, byte rawType, uint32_t para1, uint32_t para2);
        MidiEvent(uint32_t tickTime, byte rawType, byte rawMetaType, uint32_t para, std::string extra);
        MidiEvent(uint32_t tickTime, enum EventType type, uint32_t para1, uint32_t para2);
        MidiEvent(uint32_t tickTime, enum EventType type, uint32_t para, std::string extra);
        enum EventType getType();
        std::string toString();

        std::string getKeySignature(); 
        int16_t getKeySignatureRaw();
        double getTempoMs();
        int32_t getTempoRaw();

    private:
        uint32_t tick;
        uint32_t para1, para2; // para1 can be metatype
        byte rawType;
        std::string extra;
    };

}

#endif