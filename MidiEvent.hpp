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

        int32_t getChannel();
        int32_t getKeyNumber();
        int32_t getVelocity();
        int32_t getKeyPressure();
        int32_t getControllerNumber();
        int32_t getControllerValue();
        int32_t getChannelPressure();
        int16_t getKeySignatureRaw();
        double getTempoMs();
        int32_t getTempoRaw();

    private:
        uint32_t tick;
        uint32_t para1, para2; // aka. data byte(s). para1 can be metatype
        byte rawType; // aka. status byte
        std::string extra;
    };

}

#endif