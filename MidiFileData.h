/*
 * MidiFileData.h
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */

#ifndef MIDIFILEDATA_H_
#define MIDIFILEDATA_H_

#include <fstream>
#include <stack>
#include "MTrkEvent.h"
#include "MidiSection.h"


class MidiFileData {
public:
	MidiFileData();
	MidiFileData(int maxSize);
	virtual ~MidiFileData();
	void writeToFileWithTrackEnding(std::ostream& ofile);
	void addEvent(MTrkEvent& mtrkEvent);
	void addDrumPattern(DrumPattern& drumPattern, int patternLength);
	void addMelodyPattern(Byte channel, char instrument, const std::string& melodyPattern);
	void addDrumSection(Byte channel, MidiSection& section, int patternLength);
	void addTempoTrack(int tempo, std::string text);
private:
	Byte getDrumInstrument(char drum);
	bool setActiveNotesOff(Byte channel, std::stack<Byte>& notes, int delay);
	bool getNote(const std::string& melodyString, unsigned int& position, Byte& noteValue, int& noteLength);
	int _length;
	Byte* _data;
	int _lastSilence; // relevant if last note of previous pattern is silence
};

#endif /* MIDIFILEDATA_H_ */
