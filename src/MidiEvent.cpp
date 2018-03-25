#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "midiutils/MidiEvent.hpp"
#include "midiutils/MidiUtils.hpp"

using namespace std;

namespace MidiUtils {

    MidiEvent::MidiEvent() {
        rawType = 0;
        tick = para1 = para2 = 0;
        extra = "";
    }

    MidiEvent::MidiEvent(istream& istream) {
        rawType = 0;
        tick = para1 = para2 = 0;
        extra = "";
        read(istream);
    }

    int MidiEvent::read(istream& istream) {

        char paraByte1, paraByte2;

        delta = readVariableLengthQuantity(istream);
        istream.read((char*)&rawType, 1);
        
        switch(rawType & 0xF0) { //0xF0 = 11110000
            case NOTE_OFF:
            case NOTE_ON:
            case KEY_PRESSURE:
            case CONTROL_CHANGE:
            case PITCH_WHEEL_CHANGE:
				istream.get(paraByte1);
                istream.get(paraByte2);
                para1 = paraByte1;
                para2 = paraByte2;
                return 0;
            case PROGRAM_CHANGE:
            case CHANNEL_PRESSURE:
                istream.get(paraByte1);
                para1 = paraByte1;
                para2 = 0;
                return 0;
            case 0xF0: // Meta and SysEx. 0x0F = 1111
                if ((rawType & 0x0F) == 0x0F) { //Meta Event
                    char metaType;
                    byte tmp4ByteBuffer[4];
                    uint32_t len;
                    istream.get(metaType);
                    len = readVariableLengthQuantity(istream);
                    switch (metaType) {
                    case SEQUENCE_NUM:
                        cout << "\tSEQUENCE_NUM" << endl;
                        istream.ignore(len);
                        return 0;
                    case CHANNEL_PREFIX:
                        istream.ignore(len);
                        return 0;
                    case END_OF_TRACK:
                        istream.ignore(len);
                        return 0;
                    case SET_TEMPO:
                        istream.read((char*)&tmp4ByteBuffer[1], 3);
                        tmp4ByteBuffer[0] &= 0x00;
                        para1 = metaType;
						para2 = byte4_to_uint32(tmp4ByteBuffer);
                        return 0;
                    case SMTPE_OFFSET:
                        istream.ignore(len);
                        return 0;
                    case TIME_SIGNATURE:
                        istream.read((char*)&tmp4ByteBuffer, 4);
                        para1 = metaType;
						para2 = byte4_to_uint32(tmp4ByteBuffer);
                        return 0;
                    case KEY_SIGNATURE:
                        istream.read((char*)&tmp4ByteBuffer, 2);
                        para1 = metaType;
						para2 = byte2_to_uint16(tmp4ByteBuffer);
                        return 0;
                    case TEXT_EVENT:
                    case COPYRIGHT:
                    case TRACK_NAME:
                    case INSTRUMENT_NAME:
                    case LYRIC:
                    case MARKER:
                    case CUE_POINT:
                    case Sequencer_Specific_Meta_event:
                    default: //Text-like Event
                        std::string data(len, ' ');
                        istream.read(&data[0], len); 
                        para1 = metaType;
                        para2 = len;
                        extra = data;
                        return 0;
                    }
                } else if((rawType & 0x0F) == 0x00 || (rawType & 0x0F) == 0x07) { //SysEx
                    uint32_t len = readVariableLengthQuantity(istream);
                    istream.seekg(len, ios::cur);
                } else {
                    cout << "Event INVALID." << endl;
                }
                return 0;
            default:
                cout << "Event INVALID. at: " << istream.tellg() << endl;
                return 0;
            }

        return 0;
    }

    int MidiEvent::write(ostream& ostream) {
        throw "Not f**king implemented";
    }

    enum EventType MidiEvent::getType() const {
        enum EventType theType = getTypeFromByte(rawType);
        switch(theType) {
            case NOTE_ON:
                if(para2) return NOTE_ON;
                else {
                    // a NOTE_ON with zero velocity will be set as a NOTE_OFF?
                    return NOTE_OFF;
                }
            case E_META: 
                return static_cast<enum EventType>(para1);
            default:
                return theType;
        }
    }

    std::string MidiEvent::toString() const {
        switch(getType()) {
            case NOTE_OFF: return "NOTE_OFF:Key=" + std::to_string(getKeyNumber()) + ",Velocity=" + std::to_string(getVelocity()) + ".";
			case NOTE_ON: return "NOTE_ON:Key=" + std::to_string(getKeyNumber()) + ",Velocity=" + std::to_string(getVelocity()) + ".";
            case KEY_PRESSURE: return "KEY_PRESSURE";
			case CONTROL_CHANGE: return "CONTROL_CHANGE:Type=" + std::to_string(getControllerNumber()) + ",Value=" + std::to_string(getControllerValue()) + ".";
            case PROGRAM_CHANGE: return "PROGRAM_CHANGE:Channel=" + std::to_string(getChannel()) + ",ProgramNo:"+std::to_string(para1)+".";
            case CHANNEL_PRESSURE: return "CHANNEL_PRESSURE";
			case PITCH_WHEEL_CHANGE: return "PITCH_WHEEL_CHANGE:Value=" + std::to_string(getPitchWhellValue())+".";
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

    int32_t MidiEvent::getDeltaTime() const {
        return delta;
    }
    
    int32_t MidiEvent::getChannel() const {
        return static_cast<int32_t>(rawType & 0x0F);
    }

    int32_t MidiEvent::getKeyNumber() const {
        return para1;
    }

    int32_t MidiEvent::getVelocity() const {
        return para2;
    }

	int32_t MidiEvent::getPitchWhellValue() const {
		return (para1 | (para2 << 7)) & 0x3FFF;
	}
    
    int32_t MidiEvent::getKeyPressure() const {
        return para2;
    }

    int32_t MidiEvent::getControllerNumber() const {
        return para1;
    }
    
    int32_t MidiEvent::getControllerValue() const {
        return para2;
    }

    int32_t MidiEvent::getChannelPressure() const {
        return para1;
    }

    int16_t MidiEvent::getKeySignatureRaw() const {
        return static_cast<int16_t>(para2);
    }

    double MidiEvent::getTempoMs() const {
        if (getType() != SET_TEMPO) return -1.0; 
        return 60.0 / (para2 / 1e6);
    }

    int32_t MidiEvent::getTempoRaw() const {
        if (getType() != SET_TEMPO) return -1; 
        return para2;
    }

    void MidiEvent::setChannel(uint8_t value) {
        rawType = ((rawType & 0xF0) | (value & 0x0F));
    }

    void MidiEvent::setKeyNumber(int32_t value) {
        para1 = value;
    }
}
