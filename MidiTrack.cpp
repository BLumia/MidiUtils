#include "MidiTrack.hpp"
#include "MidiUtils.hpp"

#include <iostream>

using namespace std;

namespace MidiUtils {
    
    MidiTrack::MidiTrack() {

    }

    void MidiTrack::appendEvent(MidiEvent event) {
        enum EventType type = event.getType();
        if (type != NOTE_ON && type != NOTE_OFF) cout << "\t" << event.toString() << endl;
        eventList.push_back(event);
    }

    MidiEvent& MidiTrack::operator[](size_t index) {
        return eventList[index];
    }
}