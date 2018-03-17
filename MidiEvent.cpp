#include "MidiEvent.hpp"
#include "MidiUtils.hpp"
#include <string>
#include <cstring>

using namespace std;

namespace MidiUtils {

    MidiEvent::MidiEvent() {
        rawType = 0;
        tick = para1 = para2 = 0;
        extra = "";
    }

    MidiEvent::MidiEvent(uint32_t tickTime, byte rawType, uint32_t para1, uint32_t para2) {
        this->rawType = rawType;
        this->tick = tickTime;
        this->para1 = para1;
        this->para2 = para2;
        extra = "";
    }

    MidiEvent::MidiEvent(uint32_t tickTime, byte rawType, byte rawMetaType, uint32_t para, std::string extra) {
        char c;
        memcpy(&c, &rawMetaType, 1);
        this->rawType = rawType;
        this->tick = tickTime;
        this->para1 = c;
        this->para2 = para;
        this->extra = extra;
    }

    MidiEvent::MidiEvent(uint32_t tickTime, enum EventType type, uint32_t para1, uint32_t para2) {
        throw "Not f**king implemented";
    }

    MidiEvent::MidiEvent(uint32_t tickTime, enum EventType type, uint32_t para, std::string extra) {
        throw "Not f**king implemented";
    }

    enum EventType MidiEvent::getType() {
        switch(rawType & 0xF0) {
            case NOTE_OFF: return NOTE_OFF;
            case NOTE_ON:
                if(para2) return NOTE_ON;
                else {
                    // a NOTE_ON with zero velocity will be set as a NOTE_OFF?
                    return NOTE_OFF;
                }
            case KEY_PRESSURE:
            case CONTROL_CHANGE:
            case PROGRAM_CHANGE:
            case CHANNEL_PRESSURE:
            case PITCH_WHEEL_CHANGE:
                return static_cast<enum EventType>(rawType & 0xF0);
            case 0xF0: 
                if ((rawType & 0x0F) == 0x0F) {
                    return static_cast<enum EventType>(para1);
                } else if ((rawType & 0x0F) == 0x00 || (rawType & 0x0F) == 0x07) {
                    return SYSEX;
                } else {
                    return E_INVALID;
                }
            default:
                return E_INVALID;
        }
    }

    std::string MidiEvent::toString() {
        switch(getType()) {
            case NOTE_OFF: return "NOTE_OFF";
            case NOTE_ON:
                if(para2) return "NOTE_ON";
                else {
                    // a NOTE_ON with zero velocity will be set as a NOTE_OFF?
                    return "NOTE_OFF(From NOTE_ON)";
                }
            case KEY_PRESSURE: return "KEY_PRESSURE";
            case CONTROL_CHANGE: return "CONTROL_CHANGE";
            case PROGRAM_CHANGE: return "PROGRAM_CHANGE";
            case CHANNEL_PRESSURE: return "CHANNEL_PRESSURE";
            case PITCH_WHEEL_CHANGE: return "PITCH_WHEEL_CHANGE";
            case SEQUENCE_NUM: return "SEQUENCE_NUM";
            case CHANNEL_PREFIX: return "CHANNEL_PREFIX";
            case END_OF_TRACK: return "END_OF_TRACK";
            case SET_TEMPO: return "SET_TEMPO:" + std::to_string(getTempoMs());
            case SMTPE_OFFSET: return "SMTPE_OFFSET";
            case TIME_SIGNATURE: return "TIME_SIGNATURE";
			case KEY_SIGNATURE: return "KEY_SIGNATURE:'" + PrettyKeySignature(para2) + "'";
            case TEXT_EVENT: return "TEXT_EVENT";
            case COPYRIGHT: return "COPYRIGHT";
            case TRACK_NAME: return "TRACK_NAME:'" + extra + "'";
            case INSTRUMENT_NAME: return "INSTRUMENT_NAME";
            case LYRIC: return "LYRIC";
            case MARKER: return "MARKER";
            case CUE_POINT: return "CUE_POINT";
            case Sequencer_Specific_Meta_event: return "Sequencer_Specific_Meta_event";
            default: return "E_INVALID";
        }
    }
    
    int16_t MidiEvent::getKeySignatureRaw() {
        return static_cast<int16_t>(para2);
    }

    double MidiEvent::getTempoMs() {
        if (getType() != SET_TEMPO) return -1.0; 
        int32_t t = para2 & 0x00FFFFFF;
        return 60.0 / (t / 1e6);
    }

    int32_t MidiEvent::getTempoRaw() {
        if (getType() != SET_TEMPO) return -1; 
        return para2;
    }
}
