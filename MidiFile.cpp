#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "MidiFile.hpp"
#include "MidiEvent.hpp"

using namespace std;

namespace MidiUtils {

    MidiFile::MidiFile() {

    }

    MidiFile::MidiFile(const std::string filepath) {
        cout << "File Path:" << filepath << endl;
        fstream input;
        input.open(filepath.data(), ios::binary | ios::in);

        if (!input.is_open()) {
            throw "File cannot be open";
        }

        read(input);
        input.close();
    }

    MidiFile::~MidiFile() {

    }

    int MidiFile::read(istream& istream) {

        std::string rawMThd(14, ' ');
        
        // Read MThd header chunk

        istream.read(&rawMThd[0], 14);
        if (rawMThd.substr(0, 4) != "MThd") {
            throw "Not a MIDI file!";
        }

        memcpy(&this->header, rawMThd.c_str(), 14);

        cout << "Format: " << byte2_to_uint16(this->header.midiFormat) << endl;
        cout << "Track Count: " << byte2_to_uint16(this->header.midiTrackCnt) << endl;
        cout << "Delta Time: " << byte2_to_uint16(this->header.midiDeltaTime) << endl;

        // Read MTrk chunks

        int readedTrackCnt = 0;
        while (readedTrackCnt < byte2_to_uint16(header.midiTrackCnt)) {
            MTrkReader(istream);

            readedTrackCnt++;
        }

        cout << "Process End" << endl;

        return 0;
    }

    int MidiFile::write(ostream& ostream) {
        return 0;
    }

    void MidiFile::appendTrack(const MidiTrack& track) {
        trackList.push_back(track);
    }

    MidiTrack& MidiFile::operator[](size_t index) {
        return trackList[index];
    }

    MidiTrack MidiFile::MTrkReader(istream& istream) {

        MidiTrack curTrack;

        struct TrackHeader tmpTrackHeader;
        std::string rawMTrkHeader(8, ' ');

        istream.read(&rawMTrkHeader[0], 8);
        if (rawMTrkHeader.substr(0, 4) != "MTrk") {
            throw "Unexpected chunk header!";
        }

        memcpy(&tmpTrackHeader, rawMTrkHeader.c_str(), 8);

        uint32_t payloadSize = byte4_to_uint32(tmpTrackHeader.trackLengh);
        streampos payloadStart = istream.tellg();
        streampos payloadEnd = payloadStart + static_cast<streampos>(payloadSize);

        cout << "MTrk payload size: " << payloadSize << endl;

        //Event Process
        uint32_t tmpCurTime = 0;
        uint32_t eventDeltaTime;
        uint32_t paraUint32;
		char paraByte1, paraByte2;
        byte evtType, lastType;
        bool ALREADY_END_OF_TRACK = false;

        while ((istream.tellg() < payloadEnd) && !ALREADY_END_OF_TRACK) {
            eventDeltaTime = readVariableLengthQuantity(istream);
            tmpCurTime += eventDeltaTime;
            istream.read((char*)&evtType, 1);
            if ((evtType & 0x80) == 0x00) {
                evtType = lastType;
                istream.seekg(-1, ios::cur);
            } else {
                lastType = evtType;
            }
retry:
            switch(evtType & 0xF0) { //0xF0 = 11110000
            case NOTE_OFF:
            case NOTE_ON:
            case KEY_PRESSURE:
            case CONTROL_CHANGE:
            case PITCH_WHEEL_CHANGE:
				istream.get(paraByte1);
                istream.get(paraByte2);
                curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, paraByte1, paraByte2));
                break;
            case PROGRAM_CHANGE:
            case CHANNEL_PRESSURE:
                istream.get(paraByte1);
                curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, paraByte1, 0));
                break;
            case 0xF0: // Meta and SysEx. 0x0F = 1111
                if ((evtType & 0x0F) == 0x0F) { //Meta Event
                    char metaType;
                    byte tmp4ByteBuffer[4];
                    uint32_t len;
                    istream.get(metaType);
                    len = readVariableLengthQuantity(istream);
                    switch (metaType) {
                    case SEQUENCE_NUM:
                        cout << "\tSEQUENCE_NUM" << endl;
                        istream.ignore(len);
                        break;
                    case CHANNEL_PREFIX:
                        istream.ignore(len);
                        break;
                    case END_OF_TRACK:
                        istream.ignore(len);
                        ALREADY_END_OF_TRACK = true;
                        curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, metaType, 0));
                        break;
                    case SET_TEMPO:
                        istream.read((char*)&tmp4ByteBuffer[1], 3);
                        tmp4ByteBuffer[0] &= 0x00;
						paraUint32 = byte4_to_uint32(tmp4ByteBuffer);
                        curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, metaType, paraUint32));
                        break;
                    case SMTPE_OFFSET:
                        istream.ignore(len);
                        break;
                    case TIME_SIGNATURE:
                        istream.read((char*)&tmp4ByteBuffer, 4);
						paraUint32 = byte4_to_uint32(tmp4ByteBuffer);
                        curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, metaType, paraUint32));
                        break;
                    case KEY_SIGNATURE:
                        istream.read((char*)&tmp4ByteBuffer, 2);
						paraUint32 = byte2_to_uint16(tmp4ByteBuffer);
                        curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, metaType, paraUint32));
                        break;
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
                        curTrack.appendEvent(MidiEvent(eventDeltaTime, evtType, metaType, len, data));
                        break;
                    }
                } else if((evtType&0x0F)==0x00||(evtType&0x0F)==0x07) { //SysEx
                    uint32_t len = readVariableLengthQuantity(istream);
                    istream.seekg(len, ios::cur);
                } else {
                    cout << "Event INVALID." << endl;
                }
                break;
            default:
                cout << "Event INVALID. Trying to repeat last event. at: " << istream.tellg() << endl;
                istream.seekg(-1, ios::cur);
                evtType = lastType;
                goto retry;
            }
            lastType = evtType;
        }

        //istream.seekg(payloadEnd, ios::beg);

        return curTrack;
    }

}