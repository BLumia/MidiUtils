#include <iostream>
#include <fstream>
#include <string>
#include <midiutils/MidiFile.hpp>

using namespace std;
using namespace MidiUtils;

int main() {
    MidiFile a;
    a.on_appendEvent = [](const MidiEvent& e) {
        enum EventType type = e.getType();
        if (type != NOTE_ON && type != NOTE_OFF) cout << "\t:" << e.toString() << endl;
    };
    a.on_appendTrack = [](const MidiTrack& t) {
        cout << "-- One Track Process Done --" << endl;
    };
    a.load("test.mid");
    cout << "Nya!" << endl;
} 
