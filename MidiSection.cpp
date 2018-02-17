/*
 * MidiSection.cpp
 *
 *  Created on: 30.1.2018
 *      Author: ari
 */

#include "MidiSection.h"
#include <iostream>

MidiSection::MidiSection() {
}

MidiSection::~MidiSection() {
	// TODO Auto-generated destructor stub
}

MidiSection::MidiSection(int tempo, DrumPattern pattern, int lengthBars) {
	_tempo = tempo;
	for (int i=0; i<lengthBars; i++)
		_patternVec.push_back(pattern);
}

void MidiSection::addBarToPattern(int bar,DrumPattern& subPattern) {
	_patternVec.at(bar).addToPattern(subPattern);
}

void MidiSection::replaceBarInPattern(int bar, int timeFromBarStart, DrumPattern& subPattern) {
	_patternVec.at(bar).replacePattern(timeFromBarStart, subPattern);
}

void MidiSection::addTogether(MidiSection& section2) {
	if (_patternVec.size() != section2.getPatternVec().size()){
		std::cout << "Different sizes in MidiSection::addTogether\n";
		exit(1);
	}
	for (unsigned int i = 0; i < _patternVec.size(); i++)
		addBarToPattern(i, section2.getPatternAt(i));
}

DrumPattern& MidiSection::getPatternAt(int i) {
	return _patternVec.at(i);
}

void MidiSection::addToEveryNth(int nth, DrumPattern& addPattern) {
	for (unsigned int i = 0; i < _patternVec.size(); i++){
		if ((i+1) % nth == 0)
			addBarToPattern(i, addPattern);
	}
}

void MidiSection::setVolume(unsigned char instrument, unsigned char newVolume) {
	for (DrumPattern& pattern: _patternVec)
		pattern.setVolume(instrument, newVolume);
}
