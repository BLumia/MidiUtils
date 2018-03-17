#include <iostream>
#include <fstream>
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

        cout << "Note Count: " << this->noteCnt << endl;

        return 0;
    }

    int MidiFile::write(ostream& ostream) {
        return 0;
    }

    void MidiFile::MTrkReader(istream& istream) {

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
        uint32_t para1, para2;
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
                istream.read((char*)&para1, 1);
                istream.read((char*)&para2, 1);
                //cout<<"\tNOTE_OFF" << endl;
                break;
            case NOTE_ON:
                istream.read((char*)&para1, 1);
                istream.read((char*)&para2, 1);
                if(para2) {
                    this->noteCnt++;
                    //cout<<"\tNOTE_ON" << endl;
                } else {
                    // a NOTE_ON with zero velocity will be set as a NOTE_OFF
                    MidiEvent a(eventDeltaTime, evtType, para1, para2);
                    cout<<"\tNOTE_OFF(from NON)\t" << a.toString() << endl;
                }
                break;
            case KEY_PRESSURE:
            case CONTROL_CHANGE:
                istream.read((char*)&para1, 1);
                istream.read((char*)&para2, 1);
                {MidiEvent a(eventDeltaTime, evtType, para1, para2);
                cout<<"\tKEY_PRESSURE/CTRL_CHANGE\t" << a.toString() << endl;}
                break;
            case PROGRAM_CHANGE:
            case CHANNEL_PRESSURE:
                istream.read((char*)&para1, 1);
                {MidiEvent a(eventDeltaTime, evtType, para1, 0);
                cout<<"\tPROGRAM_CHANGE/CHANNEL_PRESSURE\t" << a.toString() << endl;}
                break;
            case PITCH_WHEEL_CHANGE:
                istream.read((char*)&para1, 1);
                istream.read((char*)&para2, 1);
                {MidiEvent a(eventDeltaTime, evtType, para1, para2);
                cout<<"\tPITCH_WHEEL_CHANGE\t" << a.toString() << endl;}
                break;
            case 0xF0: // Meta and SysEx. 0x0F = 1111
                if ((evtType & 0x0F) == 0x0F) { //Meta Event
                    char metaType;
                    byte tmp4ByteBuffer[4];
                    istream.get(metaType);
                    switch (metaType) {
                    case SEQUENCE_NUM:
                        istream.seekg(+3, ios::cur);
                        break;
                    case CHANNEL_PREFIX:
                        istream.seekg(+2, ios::cur);
                        break;
                    case END_OF_TRACK:
                        istream.seekg(+1, ios::cur);
                        ALREADY_END_OF_TRACK = true;
                        break;
                    case SET_TEMPO: // please notice that, to calc tempo, we should do para2 & 0x00ffffff
                        istream.read((char*)&tmp4ByteBuffer, 4);
                        para1 = byte4_to_uint32(tmp4ByteBuffer);
                        {MidiEvent a(eventDeltaTime, evtType, metaType, para1);
                        cout<<"\tTEMPO\t" << a.toString() << endl;}
                        break;
                    case SMTPE_OFFSET:
                        istream.seekg(+6, ios::cur);
                        break;
                    case TIME_SIGNATURE:
                        istream.seekg(+1, ios::cur);
                        istream.read((char*)&tmp4ByteBuffer, 4);
                        para1 = byte4_to_uint32(tmp4ByteBuffer);
                        {MidiEvent a(eventDeltaTime, evtType, metaType, para1);
                        cout<<"\tTIME_SIGNATURE\t" << a.toString() << endl;}
                        break;
                    case KEY_SIGNATURE:
                        istream.seekg(+1, ios::cur);
                        istream.read((char*)&tmp4ByteBuffer, 4);
                        para1 = byte2_to_uint16(tmp4ByteBuffer);
                        {MidiEvent a(eventDeltaTime, evtType, metaType, para1);
                        cout<<"\tKEY_SIGNATURE\t" << para1 << a.toString() << endl;}
                        break;
                    case TEXT_EVENT:
                    case COPYRIGHT:
                    case TRACK_NAME:
                    case INSTRUMENT_NAME:
                    case LYRIC:
                    case MARKER:
                    case CUE_POINT:
                    case Sequencer_Specific_Meta_event:
                    case 0x40:
                        cout << "??";
                    default: //Text-like Event
                        uint32_t len = readVariableLengthQuantity(istream);
                        std::string data(len + 1, ' ');
                        istream.read(&data[0], len); 
                        {MidiEvent a(eventDeltaTime, evtType, metaType, len, data);
                        cout<<"\tTEXT(" << metaType << "): " << data << "\t//"<< a.toString() << endl;}
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

        return;
    }

}