/*
 * main.cpp
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>

#include "MidiFileHdr.h"
#include "MidiFileData.h"
#include "MidiConsts.h"
#include "MidiSection.h"
#include "DrumPattern.h"
#include "DrumInstruments.h"

typedef struct {
	std::string pattern;
	int nBars;
	int tempo;
	std::string fillPattern;
	int fillFreq;
} ProgramArgs;

void createDrumTest(std::string filename);
void execUserRequest(int, char**);
bool findArguments(int argc, char** argv, std::vector<ProgramArgs>& pArgVec, std::string& midFile);
void usage();


int main(int argc, char** argv){
	execUserRequest(argc, argv);
	// createDrumTest("drumtest.mid");
	return 0;
}

void execUserRequest(int argc, char** argv){
	std::string pattern;
	std::string midFile;
	std::vector<ProgramArgs> pArgsVec;

	if (findArguments(argc, argv, pArgsVec, midFile) == false){
		usage(); return;
	}

	std::ofstream myFile;
	myFile.open((midFile + ".mid").c_str(), std::ios::out | std::ios::binary);

	// midi file hdr
	MidiFileHdr mHdr(FORMAT_SIMULT_TRACKS, 2);
	mHdr.writetoFile(myFile);

	// tempo track
	MidiFileData mData;
	mData.addTempoTrack(pArgsVec.at(0).tempo, "midicreate");
	mData.writeToFileWithTrackEnding(myFile);

	// add drum track sections
	for (auto section : pArgsVec){
		DrumPattern drumPattern(section.pattern);
		MidiSection midiSec(section.tempo, drumPattern, section.nBars);
		if (!section.fillPattern.empty()){
			DrumPattern fillPattern(section.fillPattern);
			midiSec.addToEveryNth(section.fillFreq, fillPattern);
		}
		mData.addDrumSection(DRUM_CHANNEL, midiSec, drumPattern.getPatternLength());
	}
	mData.writeToFileWithTrackEnding(myFile);
	myFile.close();
}

bool findArguments(int argc, char* argv[], std::vector<ProgramArgs>& pArgVec, std::string& midFile){
	std::stringstream fnameStream;
	for (int i = 1; i < argc; i++) {
		ProgramArgs pArgs;
		if (std::string(argv[i]) == "-s"){
			pArgs.pattern = argv[i+1];
			pArgs.nBars = atoi(argv[i+2]);
			pArgs.tempo = atoi(argv[i+3]);
			if (argc > i+5 && isalpha(argv[i+4][0]) && isdigit(argv[i+5][0])) {
				pArgs.fillPattern = argv[i+4];
				pArgs.fillFreq = atoi(argv[i+5]);
			}
			pArgVec.push_back(pArgs);
			if (pArgVec.size() > 1)
				fnameStream << "-";
			fnameStream << pArgs.pattern << "-" << pArgs.nBars << "-" << pArgs.tempo;
		}
		if (std::string(argv[i]) == "-d")
			DrumInstruments::dispDrumInstruments();
	}
	midFile = fnameStream.str();
	return pArgVec.size() ? true : false;
}

void usage(){
	std::cout << "midicreate - create a midi file containing a drum track\n\n";
	std::cout << "Usage: \n";
	std::cout << "midicreate [-s patternName nBars tempo [fillPatternName fillPatternFrequency]] [-s ...] [-d]\n\n";
	std::cout << "-s, starts section parameters\n";
	std::cout << "    patternName (str), must be found from patterns.conf file\n";
	std::cout << "    nBars (num), number of patterns (i.e. bars) per section\n";
	std::cout << "    tempo (num), tempo as bpm, e.g. 120\n";
	std::cout << "    fillPatternName (str, optional), must be found from patterns.conf file\n";
	std::cout << "    fillPatternFrequency (num), must be present if fillPattern is given\n";
	std::cout << "-d, optional; display drum instrument names which can be used in pattern.conf\n";
	std::cout << "Program generates the output (midi) file name based on pattern name\n\n";
	std::cout << "Example: Create a drum track consisting of two sections, both 12 bars long.\n";
	std::cout << "        Tempo is 110 bpm. Using fill pattern on every fourth bar.\n";
	std::cout << "        midicreate -s beat2 12 110 fill1 4 -s rock2 12 110 fill2 4\n\n";
}

void createDrumTest(std::string filename) {
	MidiFileHdr mHdr(FORMAT_SINGLE_TRACK, 1, 96);
	MidiFileData mData;
	MTrkEvent mtEvent;

	std::ofstream myFile;
	myFile.open (filename.c_str(), std::ios::out | std::ios::binary);
	mHdr.writetoFile(myFile);

	// data, only 1 track
	for (int instr = 31; instr <= 82; instr++) {
		mtEvent.noteOn(50, DRUM_CHANNEL, instr, VOLUME_DEFAULT); mData.addEvent(mtEvent);
		mtEvent.noteOn(96, DRUM_CHANNEL, instr, 0); mData.addEvent(mtEvent);
	}

	mData.writeToFileWithTrackEnding(myFile);
	myFile.close();
}
