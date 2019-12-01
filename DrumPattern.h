/*
 * DrumPattern.h
 *
 *  Created on: 29.1.2018
 *      Author: ari
 */

#ifndef DRUMPATTERN_H_
#define DRUMPATTERN_H_

#include <vector>
#include <map>
#include "MidiConsts.h"


typedef struct {
    Byte drumInstrument;
    int timeFromStart;
    int volume;
} DrumHit;

class DrumPattern {
public:
    DrumPattern();
    DrumPattern(std::string patternName);
    DrumPattern(const std::vector<DrumHit>& pattern);
    virtual ~DrumPattern();
    void addToPattern(DrumPattern& subPattern);
    void replacePattern(int timeFromBarStart, DrumPattern& subPattern);
    void addInstrumentToPattern(Byte instrument, const std::vector<int>& timing, Byte volume);
    void setVolume(int instrument, int newVolume);
    DrumHit& getHitAt(int i);
    int getSize(){return m_drumHitVec.size();};

    int getPatternLength() const {
        return m_patternLength;
    }

private:
    bool findFromConfigFile(std::string pattern);
    void getPatternInstruments(std::ifstream& ifile);
    std::vector<DrumHit> m_drumHitVec;
    int m_patternLength;
};

#endif /* DRUMPATTERN_H_ */

