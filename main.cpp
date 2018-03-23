#include <iostream>
#include <fstream>
#include <string>
#include <midiutils/MidiFile.hpp>

using namespace std;
using namespace MidiUtils;

int main() {
    
    MidiFile a;

    a.on_updateHeader = [](MidiHeader& h) {
        cout << "File Format: " << h.getFormat() << endl;
        cout << "Track Count: " << h.getTrackCount() << endl;
        cout << "Delta Time: " << h.getDeltaTime() << endl;
    };

    a.on_appendEvent = [](MidiEvent& e) {
        enum EventType type = e.getType();
        if (type != NOTE_ON && type != NOTE_OFF) cout << "\t" << e.toString() << endl;
    };

    a.on_appendTrack = [](MidiTrack& t) {
        cout << "-- One Track Process Done --" << endl;
    };

    try {
        a.load("test.mid");
    } catch (const char * e) {
        cout << e << endl;
    }

    cout << "Nya!" << endl;

    return 0;
} 
