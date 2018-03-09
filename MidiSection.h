/*
 * MidiSection.h
 *
 *  Created on: 30.1.2018
 *      Author: ari
 */

#ifndef MIDISECTION_H_
#define MIDISECTION_H_

#include "DrumPattern.h"

class MidiSection {
public:
	MidiSection();
	virtual ~MidiSection();
	MidiSection(int tempo, DrumPattern pattern, int lengthBars);
	MidiSection(int tempo, std::string pattern, int lengthBars);
	void addBarToPattern(int bar, DrumPattern& subPattern);
	void replaceBarInPattern(int bar, DrumPattern& subPattern);
	void addTogether(MidiSection& section2);
	void addToEveryNth(int nth, DrumPattern& addPattern);
	void replaceInEveryNth(int nth, DrumPattern& repPattern);
	DrumPattern& getPatternAt(int i);
	void setVolume(Byte instrument, Byte newVolume);
	int getSize() const { return _patternVec.size(); }

	const std::vector<DrumPattern>& getPatternVec() const {
		return _patternVec;
	}

	void setTempo(int tempo) {
		_tempo = tempo;
	}

	int getTempo() const {
		return _tempo;
	}

private:
	int _tempo;
	std::vector<DrumPattern> _patternVec;
};

#endif /* MIDISECTION_H_ */
