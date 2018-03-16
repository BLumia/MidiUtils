#include <iostream>
#include <fstream>
#include <cstring>
#include "MidiFile.hpp"

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
    }

    MidiFile::~MidiFile() {

    }

    int MidiFile::read(istream& istream) {
        //std::string tmp(4, ' ');
        std::string rawMThd(14, ' ');
        
        istream.read(&rawMThd[0], 14);
        if (rawMThd.substr(0, 4) != "MThd") {
            throw "Not a MIDI file!";
        }

        memcpy(&this->header, rawMThd.c_str(), 14);

        cout << "Format: " << byte2_to_uint16(this->header.midiFormat) << endl;
        cout << "Track Count: " << byte2_to_uint16(this->header.midiTrackCnt) << endl;
        cout << "Delta Time: " << byte2_to_uint16(this->header.midiDeltaTime) << endl;

        return 0;
    }

    int MidiFile::write(ostream& ostream) {
        return 0;
    }

}