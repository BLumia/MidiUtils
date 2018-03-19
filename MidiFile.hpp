#ifndef __MIDIFILE_HPP
#define __MIDIFILE_HPP

#include <istream>
#include <fstream>
#include "MidiUtils.hpp"
#include "MidiTrack.hpp"

using namespace std;

namespace MidiUtils {

    class MidiFile {

    public:
        MidiFile();
        MidiFile(const std::string filepath);
        ~MidiFile();
        int read(istream& istream);
        int write(ostream& ostream);
        void appendTrack(const MidiTrack& track);
        MidiTrack& operator[](size_t index);
    
    private:
        struct FileHeader header;
        MidiTrack MTrkReader(istream& istream);
        std::vector<MidiTrack> trackList;
    };

}
#endif /* __MIDIFILE_HPP */