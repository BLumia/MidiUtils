#ifndef __MIDIHEADER_HPP
#define __MIDIHEADER_HPP

#include "MidiUtils.hpp"

using namespace std;

namespace MidiUtils {

    class MidiHeader {

    public:
        MidiHeader();
        int read(istream& istream);
        int write(ostream& ostream);

        int getFormat() const;
        int getTrackCount() const;
        int getDeltaTime() const;

    private:
        struct FileHeader header;
        int format;
        uint16_t trackCount;
        uint16_t deltaTime;
    };

}

#endif