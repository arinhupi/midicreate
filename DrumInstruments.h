/*
 * DrumInstruments.h
 *
 *  Created on: 4.3.2018
 *      Author: ari
 */

#ifndef DRUMINSTRUMENTS_H_
#define DRUMINSTRUMENTS_H_

#include "MidiConsts.h"
#include <map>

namespace DrumInstruments {
	Byte getInstrument(const std::string& instrName);
	void dispDrumInstruments();
	//std::map<std::string, Byte> _drumMap2;
};

#endif /* DRUMINSTRUMENTS_H_ */
