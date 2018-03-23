#ifndef __MIDITRACK_HPP
#define __MIDITRACK_HPP

#include <vector>
#include <functional>
#include "MidiUtils.hpp"
#include "MidiEvent.hpp"

using namespace std;

namespace MidiUtils {

    class MidiTrack {
        
    public:
        MidiTrack();
        void appendEvent(MidiEvent&& event);
        MidiEvent& operator[](size_t index);

        std::function<void(MidiEvent&)> on_appendEvent;

    private:
        std::vector<MidiEvent> eventList;
    };
}

#endif