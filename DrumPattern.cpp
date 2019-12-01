/*
 * DrumPattern.cpp
 *
 *  Created on: 29.1.2018
 *      Author: ari
 */

#include "DrumPattern.h"
#include "DrumInstruments.h"
#include <algorithm> //sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cctype>

DrumPattern::DrumPattern() {
}

DrumPattern::DrumPattern(std::string patternName)
{
    if (!findFromConfigFile(patternName))
        throw std::runtime_error("Configuration file reading error");
}

// in case another way to construct is needed in the future...
DrumPattern::DrumPattern(const std::vector<DrumHit>& pattern)
{
    m_drumHitVec = pattern;
    setVolume(DRUM_HIHAT_CLOSED, VOLUME_DEFAULT);
    setVolume(DRUM_BASS1, VOLUME_DEFAULT);
    setVolume(DRUM_SNARE1, VOLUME_DEFAULT);
}

DrumPattern::~DrumPattern()
{
    // TODO Auto-generated destructor stub
}

void DrumPattern::setVolume(int instrument, int newVolume)
{
    if (newVolume > 127) return;

    for (DrumHit& hit: m_drumHitVec) {
        if (hit.drumInstrument == instrument)
            hit.volume = newVolume;
    }
}

void DrumPattern::addToPattern(DrumPattern& subPattern)
{
    for (int i = 0; i < subPattern.getSize(); i++)
        m_drumHitVec.push_back(subPattern.getHitAt(i));

    std::sort(m_drumHitVec.begin(), m_drumHitVec.end(), [ ]( const DrumHit& hit1, const DrumHit& hit2 )
            { return hit1.timeFromStart < hit2.timeFromStart; });
}

void DrumPattern::replacePattern(int timeFromBarStart, DrumPattern& subPattern)
{
    for (unsigned int i = 0; i < m_drumHitVec.size(); i++){
        if (m_drumHitVec.at(i).timeFromStart >= timeFromBarStart)
            m_drumHitVec.erase(m_drumHitVec.begin() + i);
    }
    addToPattern(subPattern);
}

void DrumPattern::addInstrumentToPattern(Byte instrument, const std::vector<int>& timing, Byte volume)
{
    DrumHit drumHit;
    for (int hitTime: timing){
        drumHit.timeFromStart = hitTime;
        drumHit.drumInstrument = instrument;
        drumHit.volume = volume;
        m_drumHitVec.push_back(drumHit);
    }
    std::sort(m_drumHitVec.begin(), m_drumHitVec.end(), [ ]( const DrumHit& hit1, const DrumHit& hit2 )
            { return hit1.timeFromStart < hit2.timeFromStart; });
}

DrumHit& DrumPattern::getHitAt(int i)
{
    return m_drumHitVec.at(i);
}

bool DrumPattern::findFromConfigFile(std::string myPattern)
{
    std::ifstream myFile;
    bool patternFound = false;

    myFile.open("patterns.conf", std::ios::in);
    if (myFile.is_open() == false){
        std::cout << "Pattern configuration file open failure\n";
        return false;
    }

    std::string line;
    while (std::getline(myFile, line)){
        std::istringstream iss(line);
        if (iss.peek() == '#')
            continue;
        std::string patternLabel, patternName, ticksString;
        if ((iss >> patternLabel >> patternName >> ticksString) && (patternLabel == "pattern" && patternName == myPattern)){
            patternFound = true;
            m_patternLength = atoi(ticksString.c_str());
            getPatternInstruments(myFile);
            break;
        }
    }
    myFile.close();

    if (patternFound)
        return true;
    else{
        std::cout << "Pattern " << myPattern << " not found from configuration file.\n";
        return false;
    }
}

void DrumPattern::getPatternInstruments(std::ifstream& ifile)
{
    std::string line, instrName, volLabel, comma;
    int hitTime, volume;
    std::vector<int> timeVec;

    while (std::getline(ifile, line)){
        if (line.empty() || line.compare(0, 3, "end") == 0)
            break;
        std::istringstream iss(line);
        std::getline(iss, instrName, ' ');
        while (iss >> hitTime){
            timeVec.push_back(hitTime);
            if (iss.peek() == ',')
                iss >> comma;
        }
        if (!(iss >> volLabel >> volume && volLabel == "vol" && volume < 128))
            volume = VOLUME_DEFAULT;
        addInstrumentToPattern(DrumInstruments::getInstrument(instrName), timeVec, volume);
        timeVec.clear();
    }
}
