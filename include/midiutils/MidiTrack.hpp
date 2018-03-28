#ifndef __MIDITRACK_HPP
#define __MIDITRACK_HPP

#include <vector>
#include <functional>
#include "MidiUtils.hpp"
#include "MidiEvent.hpp"

using namespace std;

namespace MidiUtils {

	/*! \brief class of a single track of a MidiFile
	 * 
	 * This class hold Standard Midi File header (`MTrk` chunk) information.
	 * */
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
