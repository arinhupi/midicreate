/*
 * MTrkEvent.h
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */

#ifndef MTRKEVENT_H_
#define MTRKEVENT_H_

#include "MidiConsts.h"

class MTrkEvent {
public:
	MTrkEvent();
	virtual ~MTrkEvent();
	void noteOn(int interval, int channel, int note, int velocity);
	void programChange(int interval, int channel, int newProgram);
	void trackEnd(int interval);
	void metaTimeSignature(int numerator, int denominator, int clocksPerTick, int nrOf32thNotesPer24Clocks);
	void metaKeySignature(int sharpsFlats, int majorMinor);
	void metaSetTempo(int bpm);
	void metaTextEvent(std::string text);
	const Byte* getEventData() const {
		return _eventData;
	}

	int getEventSize() const {
		return _eventSize;
	}

private:
	int _eventSize;
	Byte _eventData[50];
	int convertToVLV(int value, int startIndex); // return vlv size in bytes
};

#endif /* MTRKEVENT_H_ */
