/*
 * DrumPattern.cpp
 *
 *  Created on: 29.1.2018
 *      Author: ari
 */

#include "DrumPattern.h"
#include <algorithm> //sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cctype>

DrumPattern::DrumPattern() {
}

DrumPattern::DrumPattern(std::string patternName) {
	if (!findFromConfigFile(patternName))
		throw std::runtime_error("Configuration file reading error");
}

// in case another way to construct is needed in the future...
DrumPattern::DrumPattern(const std::vector<DrumHit>& pattern) {
	_drumHitVec = pattern;
	setVolume(DRUM_HIHAT_CLOSED, VOLUME_DEFAULT);
	setVolume(DRUM_BASS1, VOLUME_DEFAULT);
	setVolume(DRUM_SNARE1, VOLUME_DEFAULT);
}

DrumPattern::~DrumPattern() {
	// TODO Auto-generated destructor stub
}

void DrumPattern::setVolume(int instrument, int newVolume) {
	if (newVolume > 127) return;

	for (DrumHit& hit: _drumHitVec) {
		if (hit.drumInstrument == instrument)
			hit.volume = newVolume;
	}
}

void DrumPattern::addToPattern(DrumPattern& subPattern) {
	for (int i = 0; i < subPattern.getSize(); i++)
		_drumHitVec.push_back(subPattern.getHitAt(i));

	std::sort(_drumHitVec.begin(), _drumHitVec.end(), [ ]( const DrumHit& hit1, const DrumHit& hit2 )
			{ return hit1.timeFromStart < hit2.timeFromStart; });
}

void DrumPattern::replacePattern(int timeFromBarStart, DrumPattern& subPattern) {
	for (unsigned int i = 0; i < _drumHitVec.size(); i++){
		if (_drumHitVec.at(i).timeFromStart >= timeFromBarStart)
			_drumHitVec.erase(_drumHitVec.begin() + i);
	}
	addToPattern(subPattern);
}

void DrumPattern::addInstrumentToPattern(Byte instrument, const std::vector<int>& timing, Byte volume) {
	DrumHit drumHit;
	for (int hitTime: timing){
		drumHit.timeFromStart = hitTime;
		drumHit.drumInstrument = instrument;
		drumHit.volume = volume;
		_drumHitVec.push_back(drumHit);
	}
	std::sort(_drumHitVec.begin(), _drumHitVec.end(), [ ]( const DrumHit& hit1, const DrumHit& hit2 )
			{ return hit1.timeFromStart < hit2.timeFromStart; });
}

DrumHit& DrumPattern::getHitAt(int i) {
	return _drumHitVec.at(i);
}

bool DrumPattern::findFromConfigFile(std::string myPattern) {
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
			_patternLength = atoi(ticksString.c_str());
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

void DrumPattern::getPatternInstruments(std::ifstream& ifile) {
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
		addInstrumentToPattern(getInstrument(instrName), timeVec, volume);
		timeVec.clear();
	}
}

unsigned char DrumPattern::getInstrument(const std::string& instrName) {
	Byte drumValue;
	try{
		drumValue = _drumMap.at(instrName);
	}
	catch (...){
		std::cout << "Drum instrument " << instrName << " not found\n";
		throw;
	};
	return drumValue;
}

std::map<std::string, Byte> DrumPattern::_drumMap = {
		{"bass", DRUM_BASS1},
		{"bass2", DRUM_BASS2},
		{"side_stick", DRUM_SIDE_STICK},
		{"snare", DRUM_SNARE1},
		{"hand_clap", DRUM_HAND_CLAP},
		{"snare2", 	DRUM_SNARE2},
		{"tom_low_floor", DRUM_TOM_LOW_FLOOR},
		{"hihat_closed", DRUM_HIHAT_CLOSED},
		{"tom_high_flow", DRUM_TOM_HIGH_FLOOR},
		{"hihat_pedal", DRUM_HIHAT_PEDAL},
		{"tom_low", DRUM_TOM_LOW},
		{"hihat_open", DRUM_HIHAT_OPEN},
		{"tom_low_mid", DRUM_TOM_LOW_MID},
		{"tom_hi_mid", DRUM_TOM_HI_MID},
		{"cymbal_crash", DRUM_CYMBAL_CRASH},
		{"tom_high", DRUM_TOM_HIGH},
		{"cymbal_ride", DRUM_CYMBAL_RIDE},
		{"cymbal_chinese", DRUM_CYMBAL_CHINESE},
		{"ride_bell", DRUM_RIDE_BELL},
		{"tambourine", DRUM_TAMBOURINE},
		{"cymbal_splash", DRUM_CYMBAL_SPLASH},
		{"cowbell", DRUM_COWBELL},
		{"cymbal_crash2", DRUM_CYMBAL_CRASH2},
		{"vibraslap", DRUM_VIBRASLAP},
		{"cymbal_ride2", DRUM_CYMBAL_RIDE2},
		{"bongo_hi", DRUM_BONGO_HI},
		{"bongo_low", DRUM_BONGO_LOW},
		{"conga_mute_hi", DRUM_CONGA_MUTE_HI},
		{"conga_open_hi", DRUM_CONGA_OPEN_HI},
		{"conga_low", DRUM_CONGA_LOW},
		{"timabale_high", DRUM_TIMBALE_HIGH},
		{"timbale_low", DRUM_TIMBALE_LOW},
		{"agogo_high", DRUM_AGOGO_HIGH},
		{"agogo_low", DRUM_AGOGO_LOW},
		{"cabase", DRUM_CABASE},
		{"maracas", DRUM_MARACAS},
		{"short_whistle", DRUM_SHORT_WHISTLE},
		{"long_whistle", DRUM_LONG_WHISTLE},
		{"guiro_short", DRUM_GUIRO_SHORT},
		{"guiro_long", DRUM_GUIRO_LONG},
		{"claves", DRUM_CLAVES},
		{"wood_block_hi", DRUM_WOOD_BLOCK_HI},
		{"wood_block_low", DRUM_WOOD_BLOCK_LOW},
		{"cuica_mute", DRUM_CUICA_MUTE},
		{"cuica_open", DRUM_CUICA_OPEN},
		{"triangle_mute", DRUM_TRIANGLE_MUTE},
		{"triangle_open", DRUM_TRIANGLE_OPEN}
};

void DrumPattern::dispDrumInstruments() {
	std::cout << "Available percussion instruments based on the General Midi standard.\n";
	std::cout << "Use these when adding new drum patterns to patterns.conf file.\n\n";
	for (auto i : _drumMap)
		std::cout << i.first << "  ";

	std::cout << "\n\n";
}
