/*
 * DrumInstruments.h
 *
 *  Created on: 4.3.2018
 *      Author: ari
 */

#ifndef DRUMINSTRUMENTS_H_
#define DRUMINSTRUMENTS_H_

#include "MidiConsts.h"

namespace DrumInstruments
{
    Byte getInstrument(const std::string& instrName);
    void dispDrumInstruments();
};

#endif /* DRUMINSTRUMENTS_H_ */
