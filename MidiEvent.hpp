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
        enum EventType getType() const;
        std::string toString() const;

        int32_t getChannel() const;
        int32_t getKeyNumber() const;
        int32_t getVelocity() const;
        int32_t getPitchWhellValue() const;
        int32_t getKeyPressure() const;
        int32_t getControllerNumber() const;
        int32_t getControllerValue() const;
        int32_t getChannelPressure() const;
        int16_t getKeySignatureRaw() const;
        double getTempoMs() const;
        int32_t getTempoRaw() const;

    private:
        uint32_t tick;
        uint32_t para1, para2; // aka. data byte(s). para1 can be metatype
        byte rawType; // aka. status byte
        std::string extra;
    };

}

#endif