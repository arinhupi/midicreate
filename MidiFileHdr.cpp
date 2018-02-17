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

MidiFileHdr::MidiFileHdr(int format, int tracks) {
	_format = __builtin_bswap16(format);
	_tracks = __builtin_bswap16(tracks);
	_division = __builtin_bswap16(TICKS_PER_QN);
}

MidiFileHdr::MidiFileHdr(int format, int tracks, int division) {
	_format = __builtin_bswap16(format);
	_tracks = __builtin_bswap16(tracks);
	_division = __builtin_bswap16(division);
}

void MidiFileHdr::writetoFile(std::ostream& ofile) {
	char hdrData[14] = {'M', 'T', 'h', 'd', 0, 0, 0, 6 };
	std::memcpy(&hdrData[8], &_format, 2);
	std::memcpy(&hdrData[10], &_tracks, 2);
	std::memcpy(&hdrData[12], &_division, 2);
	ofile.write(hdrData, 14);
}
