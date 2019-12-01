/*
 * MidiFileHdr.h
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */

#ifndef MIDIFILEHDR_H_
#define MIDIFILEHDR_H_

#include <fstream>

#define FORMAT_SINGLE_TRACK 0
#define FORMAT_SIMULT_TRACKS 1
#define FORMAT_INDEP_TRACKS 2

class MidiFileHdr {
public:
    MidiFileHdr();
    MidiFileHdr(int format, int tracks);
    MidiFileHdr(int format, int tracks, int division);
    virtual ~MidiFileHdr();
    void writetoFile(std::ostream& ofile);
private:
    short m_format;
    short m_tracks;
    short m_division; // ticks per quarter note
};

#endif /* MIDIFILEHDR_H_ */
