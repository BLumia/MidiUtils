#ifndef __MIDITRACK_HPP
#define __MIDITRACK_HPP

#include <vector>
#include "MidiUtils.hpp"
#include "MidiEvent.hpp"

using namespace std;

namespace MidiUtils {

    class MidiTrack {
        
    public:
        MidiTrack();
        void appendEvent(MidiEvent event); // TODO: (const MidiEvent& event) to avoid copy
        MidiEvent& operator[](size_t index);

    private:
        std::vector<MidiEvent> eventList;
    };
}

#endif