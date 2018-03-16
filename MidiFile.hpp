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
    };

}
#endif /* __MIDIFILE_HPP */