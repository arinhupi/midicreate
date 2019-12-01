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
#include <cstdint>


MidiFileData::MidiFileData()
{
    m_data = new Byte[50000];
    m_bufSize = 50000;
    m_length = 0;
    m_lastSilence = 0;
}

MidiFileData::MidiFileData(int maxSize)
{
    m_data = new Byte[maxSize];
    m_bufSize = maxSize;
    m_length = 0;
    m_lastSilence = 0;
}

MidiFileData::~MidiFileData()
{
    delete []m_data;
}

void MidiFileData::addEvent(MTrkEvent& mtrkEvent)
{
    static int fileOverflow = 0;
    int eventSize = mtrkEvent.getEventSize();
    if (m_length + eventSize + 10 < m_bufSize){
        std::memcpy(&m_data[m_length], mtrkEvent.getEventData(), eventSize);
        m_length += eventSize;
    }
    else if (++fileOverflow == 1 )
        std::cout << "Incomplete output file due to buffer overflow in MidiFileData::addEvent.\n";
}

void MidiFileData::addDrumPattern(DrumPattern& drumPattern, int patternLength)
{
    MTrkEvent mtrkEvent;
    std::stack<Byte> activeNotes;
    DrumHit drumHit;
    Byte prevTime = 0;
    int delay;
    for (int i = 0; i < drumPattern.getSize(); i++){
        drumHit = drumPattern.getHitAt(i);
        delay = (drumHit.timeFromStart - prevTime + m_lastSilence) * 4; // 1/4 note len is 96 ticks
        if (drumHit.timeFromStart > prevTime){
            if (setActiveNotesOff(DRUM_CHANNEL, activeNotes, delay))
                delay = 0;
        }
        mtrkEvent.noteOn(delay, DRUM_CHANNEL, drumHit.drumInstrument, drumHit.volume);
        addEvent(mtrkEvent);
        activeNotes.push(drumHit.drumInstrument);
        prevTime = drumHit.timeFromStart;
        m_lastSilence = 0;
    }
    m_lastSilence = patternLength - drumHit.timeFromStart; //delay for the next pattern;
}

void MidiFileData::addDrumSection(unsigned char channel, MidiSection& section, int patternLength)
{
    MTrkEvent mtrkEvent;
    mtrkEvent.metaSetTempo(section.getTempo());
    addEvent(mtrkEvent);

    std::vector<DrumPattern> patternVec = section.getPatternVec();
    for (unsigned int i = 0; i < patternVec.size(); i++){
        addDrumPattern(patternVec.at(i), patternLength);
    }
}

void MidiFileData::addTempoTrack(int tempo, std::string text)
{
    m_length = 0;
    MTrkEvent event;
    event.metaTextEvent(text);
    addEvent(event);
    event.metaSetTempo(tempo);
    addEvent(event);
    event.metaTimeSignature(4, 2, 0x30, 0x08);
    addEvent(event);
    event.metaKeySignature(KEY_C, KEY_MAJOR); // doesn't matter with drum track
    addEvent(event);
}

bool MidiFileData::setActiveNotesOff(Byte channel, std::stack<Byte>& activeNotes, int delay)
{
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

void MidiFileData::writeToFileWithTrackEnding(std::ostream& ofile)
{
    // add first the track end event
    MTrkEvent event;
    event.trackEnd(10); addEvent(event);
    // then write track hdr and data to file
    char trackData[8] = {'M', 'T', 'r', 'k'};
    uint32_t trackLength = __builtin_bswap32(m_length);
    std::memcpy(&trackData[4], &trackLength, 4);
    ofile.write(trackData, 8);
    ofile.write((char*)m_data, m_length);
    m_length = 0;
}

// rest of functions could be used if/when support for melody tracks is added - not used at the moment
//

std::map<char, Byte> noteValueMap = { {'c', 0}, {'d', 2}, {'e', 4}, {'f', 5}, {'g', 7}, {'a', 9}, {'b', 11}, {'s', 0xff} };

void MidiFileData::addMelodyPattern(Byte channel, char instrument, const std::string& melodyPattern)
{
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
                mtrkEvent.noteOn(silenceLength + m_lastSilence, channel, octave*12 + noteValue, 40); addEvent(mtrkEvent);
                mtrkEvent.noteOn(noteLength, channel, octave*12 + noteValue, 0); addEvent(mtrkEvent);
                silenceLength = 0;
                m_lastSilence = 0;
            }
        }
    }
    m_lastSilence = silenceLength;
}

bool MidiFileData::getNote(const std::string& melodyString, unsigned int& position, Byte& noteValue, int& noteLength)
{
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
