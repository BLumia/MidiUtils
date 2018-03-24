#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "midiutils/MidiFile.hpp"
#include "midiutils/MidiEvent.hpp"
#include "midiutils/MidiHeader.hpp"

using namespace std;

namespace MidiUtils {

    MidiFile::MidiFile() {

    }

    MidiFile::MidiFile(const std::string filepath) {
        load(filepath);
    }

    MidiFile::~MidiFile() {

    }

    void MidiFile::load(const std::string filepath) {
        fstream input;
        input.open(filepath.data(), ios::binary | ios::in);

        if (!input.is_open()) {
            throw "File cannot be open";
        }

        read(input);
        input.close();
    }

    int MidiFile::read(istream& istream) {
        
        // Read MThd header chunk

        MidiHeader tmpMThd;
        tmpMThd.read(istream);
        updateHeader(std::move(tmpMThd));

        // Read MTrk chunks

        int readedTrackCnt = 0;
        while (readedTrackCnt < header.getTrackCount()) {
            appendTrack(MTrkReader(istream));

            readedTrackCnt++;
        }

        cout << "Process End" << endl;

        return 0;
    }

    int MidiFile::write(ostream& ostream) {
        return 0;
    }

    void MidiFile::appendTrack(MidiTrack&& track) {
        if (on_appendTrack) on_appendTrack(track);
        trackList.push_back(track);
    }

    void MidiFile::updateHeader(MidiHeader&& oneHeader) {
        if (on_updateHeader) on_updateHeader(oneHeader);
        header = oneHeader;
    }

    MidiHeader MidiFile::generateHeader() {
        throw "Not f**king implemented";
    }

    MidiHeader MidiFile::getHeader() const {
        return header;
    }

    MidiTrack& MidiFile::operator[](size_t index) {
        return trackList[index];
    }

    MidiTrack MidiFile::MTrkReader(istream& istream) {

        MidiTrack curTrack;

        if (on_appendEvent) curTrack.on_appendEvent = on_appendEvent;

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
        bool ALREADY_END_OF_TRACK = false;

        while ((istream.tellg() < payloadEnd) && !ALREADY_END_OF_TRACK) {
            MidiEvent e(istream);

            tmpCurTime += e.getDeltaTime();
            e.setTickTime(tmpCurTime);

            curTrack.appendEvent(std::move(e));
        }

        //istream.seekg(payloadEnd, ios::beg);

        return curTrack;
    }

}