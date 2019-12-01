/*
 * MidiFileHdr.cpp
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */

#include <cstring>
#include "MidiFileHdr.h"
#include "MidiConsts.h"

MidiFileHdr::MidiFileHdr() {
    // TODO Auto-generated constructor stub

}

MidiFileHdr::~MidiFileHdr() {
    // TODO Auto-generated destructor stub
}

MidiFileHdr::MidiFileHdr(int format, int tracks)
{
    m_format = __builtin_bswap16(format);
    m_tracks = __builtin_bswap16(tracks);
    m_division = __builtin_bswap16(TICKS_PER_QN);
}

MidiFileHdr::MidiFileHdr(int format, int tracks, int division)
{
    m_format = __builtin_bswap16(format);
    m_tracks = __builtin_bswap16(tracks);
    m_division = __builtin_bswap16(division);
}

void MidiFileHdr::writetoFile(std::ostream& ofile)
{
    char hdrData[14] = {'M', 'T', 'h', 'd', 0, 0, 0, 6 };
    std::memcpy(&hdrData[8], &m_format, 2);
    std::memcpy(&hdrData[10], &m_tracks, 2);
    std::memcpy(&hdrData[12], &m_division, 2);
    ofile.write(hdrData, 14);
}
