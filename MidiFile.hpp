#ifndef __MIDIFILE_HPP
#define __MIDIFILE_HPP

#include <istream>
#include <fstream>
#include "MidiUtils.hpp"

using namespace std;

namespace MidiUtils {

    class MidiFile {

    public:
        MidiFile();
        MidiFile(const std::string filepath);
        ~MidiFile();
        int read(istream& istream);
        int write(ostream& ostream);
    
    private:
        struct FileHeader header;
        void MTrkReader(istream& istream);
        uint32_t noteCnt = 0; // test for now
    };

}
#endif /* __MIDIFILE_HPP */