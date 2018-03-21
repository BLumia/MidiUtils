#ifndef __MIDIFILE_HPP
#define __MIDIFILE_HPP

#include <istream>
#include <fstream>
#include <functional>
#include "MidiUtils.hpp"
#include "MidiTrack.hpp"
#include "MidiHeader.hpp"

using namespace std;

namespace MidiUtils {

    class MidiFile {

    public:
        MidiFile();
        MidiFile(const std::string filepath);
        ~MidiFile();
        void load(const std::string filepath);
        int read(istream& istream);
        int write(ostream& ostream);
        void appendTrack(MidiTrack& track);
        void updateHeader(MidiHeader& oneHeader);
        MidiHeader generateHeader();
        MidiHeader getHeader() const;
        MidiTrack& operator[](size_t index);

        std::function<void(MidiEvent&)> on_appendEvent;
        std::function<void(MidiTrack&)> on_appendTrack;
        std::function<void(MidiHeader&)> on_updateHeader;
    
    private:
        MidiTrack MTrkReader(istream& istream);
        std::vector<MidiTrack> trackList;
        MidiHeader header;
    };

}
#endif /* __MIDIFILE_HPP */