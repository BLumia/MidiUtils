#ifndef __MIDIFILE_HPP
#define __MIDIFILE_HPP

#include <istream>
#include <fstream>
#include <functional>
#include "MidiUtils.hpp"
#include "MidiTrack.hpp"

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
        void appendTrack(const MidiTrack& track);
        MidiTrack& operator[](size_t index);

        std::function<void(const MidiEvent&)> on_appendEvent;
        std::function<void(const MidiTrack&)> on_appendTrack;
    
    private:
        struct FileHeader header;
        MidiTrack MTrkReader(istream& istream);
        std::vector<MidiTrack> trackList;
    };

}
#endif /* __MIDIFILE_HPP */