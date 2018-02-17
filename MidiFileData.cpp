/*
 * MidiFileData.cpp
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */

#include "MidiFileData.h"
#include "MidiConsts.h"
#include <cstring>
#include <iostream>
#include <stack>


MidiFileData::MidiFileData() {
	_data = new Byte[10000];
	_length = 0;
	_lastSilence = 0;
}

MidiFileData::~MidiFileData() {
	// TODO Auto-generated destructor stub
}

MidiFileData::MidiFileData(int maxSize) {
	_data = new Byte[maxSize];
	_length = 0;
	_lastSilence = 0;
}

void MidiFileData::addEvent(MTrkEvent& mtrkEvent) {
	int eventSize = mtrkEvent.getEventSize();
	std::memcpy(&_data[_length], mtrkEvent.getEventData(), eventSize);
	_length += eventSize;
}

void MidiFileData::addDrumPattern(DrumPattern& drumPattern, int patternLength) {
	MTrkEvent mtrkEvent;
	std::stack<Byte> activeNotes;
	DrumHit drumHit;
	Byte prevTime = 0;
	int delay;
	for (int i = 0; i < drumPattern.getSize(); i++){
		drumHit = drumPattern.getHitAt(i);
		delay = (drumHit.timeFromStart - prevTime + _lastSilence) * 4; // 1/8 note len is 48 ticks
		if (drumHit.timeFromStart > prevTime){
			if (setActiveNotesOff(DRUM_CHANNEL, activeNotes, delay))
				delay = 0;
		}
		mtrkEvent.noteOn(delay, DRUM_CHANNEL, drumHit.drumInstrument, drumHit.volume);
		addEvent(mtrkEvent);
		activeNotes.push(drumHit.drumInstrument);
		prevTime = drumHit.timeFromStart;
		_lastSilence = 0;
	}
	_lastSilence = patternLength - drumHit.timeFromStart; //delay for the next pattern;
}

void MidiFileData::addDrumSection(unsigned char channel, MidiSection& section, int patternLength) {
	MTrkEvent mtrkEvent;
	mtrkEvent.metaSetTempo(section.getTempo());
	addEvent(mtrkEvent);

	std::vector<DrumPattern> patternVec = section.getPatternVec();
	for (unsigned int i = 0; i < patternVec.size(); i++){
		addDrumPattern(patternVec.at(i), patternLength);
	}
}

void MidiFileData::addTempoTrack(int tempo, std::string text) {
	_length = 0;
	MTrkEvent event;
	event.metaTextEvent(text);
	addEvent(event);
	event.metaSetTempo(tempo);
	addEvent(event);
	event.metaTimeSignature(4, 2, 0x30, 0x08);
	addEvent(event);
	event.metaKeySignature(KEY_C, KEY_MAJOR);
	addEvent(event);
}

bool MidiFileData::setActiveNotesOff(Byte channel, std::stack<Byte>& activeNotes, int delay) {
	MTrkEvent mtrkEvent;
	Byte note;

	if (activeNotes.empty())
		return false;

	// first note in stack, add note off with delay
	if (!activeNotes.empty()){
		note = activeNotes.top();
		mtrkEvent.noteOn(delay, channel, note, 0); // no sound
		activeNotes.pop();
		addEvent(mtrkEvent);
	}
	// following notes in stack (if any), add note off without delay
	while (!activeNotes.empty()){
		note = activeNotes.top();
		activeNotes.pop();
		mtrkEvent.noteOn(0, channel, note, 0); // no sound
	}
	return true;
}

void MidiFileData::writeToFileWithTrackEnding(std::ostream& ofile) {
	// add first the track end event
	MTrkEvent event;
	event.trackEnd(10); addEvent(event);
	// then write track hdr and data to file
	char trackData[8] = {'M', 'T', 'r', 'k'};
	unsigned long trackLength = __builtin_bswap32(_length);
	std::memcpy(&trackData[4], &trackLength, sizeof trackLength);
	ofile.write(trackData, 8);
	ofile.write((char*)_data, _length);
	_length = 0;
}

// rest of functions could be used if/when support for melody tracks is added - not used at the moment
//

std::map<char, Byte> noteValueMap = {  {'c', 0}, {'d', 2},	{'e', 4}, {'f', 5},	{'g', 7}, {'a', 9}, {'b', 11}, {'s', 0xff}  };

void MidiFileData::addMelodyPattern(Byte channel, char instrument, const std::string& melodyPattern) {
	MTrkEvent mtrkEvent;
	std::stack<Byte> activeNotes;
	Byte noteValue;
	int noteLength, silenceLength = 0;
	char octave = 5; // by default

	mtrkEvent.programChange(0, channel, instrument);
	addEvent(mtrkEvent);
	for (unsigned int i = 0; i < melodyPattern.length(); i++) {
		if (melodyPattern.at(i) == 'o')
			octave = melodyPattern.at(i+1) - '0';
		if (getNote(melodyPattern, i, noteValue, noteLength)){
			noteLength *= 24;
			if (noteValue == NOTE_SILENCE)
				silenceLength += noteLength;
			else {
				mtrkEvent.noteOn(silenceLength + _lastSilence, channel, octave*12 + noteValue, 40); addEvent(mtrkEvent);
				mtrkEvent.noteOn(noteLength, channel, octave*12 + noteValue, 0); addEvent(mtrkEvent);
				silenceLength = 0;
				_lastSilence = 0;
			}
		}
	}
	_lastSilence = silenceLength;
}

bool MidiFileData::getNote(const std::string& melodyString, unsigned int& position, Byte& noteValue, int& noteLength) {
	std::map<char, Byte>::iterator it;
	noteLength = 40;
	Byte noteChar = melodyString.at(position);

	it = noteValueMap.find(noteChar);
	if (it != noteValueMap.end()){
		noteValue = noteValueMap.at(noteChar);
		if (melodyString.at(position + 1) == '#'){
			noteValue ++;
			position ++;
		}
		noteLength = melodyString.at(++position) - '0';
		return true;
	}
	return false;
}
