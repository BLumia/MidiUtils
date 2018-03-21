#include <iostream>
#include <string>
#include "midiutils/MidiTrack.hpp"
#include "midiutils/MidiUtils.hpp"

using namespace std;

namespace MidiUtils {
    
    MidiTrack::MidiTrack() {

    }

    void MidiTrack::appendEvent(MidiEvent& event) {
        if (on_appendEvent) on_appendEvent(event);
        eventList.push_back(event);
    }

    MidiEvent& MidiTrack::operator[](size_t index) {
        return eventList[index];
    }
}