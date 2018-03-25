#include <iostream>
#include <string>
#include <cstring>
#include "midiutils/MidiHeader.hpp"
#include "midiutils/MidiUtils.hpp"

using namespace std;

namespace MidiUtils {

    MidiHeader::MidiHeader() {}

    int MidiHeader::read(istream& istream) {
        std::string rawMThd(14, ' ');

        istream.read(&rawMThd[0], 14);
        if (rawMThd.substr(0, 4) != "MThd") {
            throw "Not a MIDI file!";
        }

        memcpy(&this->header, rawMThd.c_str(), 14);

        return 0;
    }

    int MidiHeader::write(ostream& ostream) {
        ostream.write((char*)&this->header, 14);
		return 0;
    }

    int MidiHeader::getFormat() const {
        return byte2_to_uint16(header.midiFormat);
    }

    int MidiHeader::getTrackCount() const {
        return byte2_to_uint16(this->header.midiTrackCnt);
    }

    int MidiHeader::getDeltaTime() const {
        return byte2_to_uint16(this->header.midiDeltaTime);
    }

}