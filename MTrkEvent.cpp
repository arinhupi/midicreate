/*
 * MTrkEvent.cpp
 *
 *  Created on: 17.1.2018
 *      Author: ari
 */
#include <iostream>
#include <cstring> // std::memcpy
#include "MTrkEvent.h"

MTrkEvent::MTrkEvent() {
    // TODO Auto-generated constructor stub

}

MTrkEvent::~MTrkEvent() {
    // TODO Auto-generated destructor stub
}

void MTrkEvent::noteOn(int interval, int channel, int note, int velocity)
{
    int vlvSize = convertToVLV(interval, 0);
    m_eventData[vlvSize+1] = 0x90 + channel;
    m_eventData[vlvSize+2] = note;
    m_eventData[vlvSize+3] = velocity; // 0...127
    m_eventSize = vlvSize+4;
}

void MTrkEvent::programChange(int interval, int channel, int newProgram)
{
    m_eventData[0] = interval;
    m_eventData[1] = 0xc0 + channel;
    m_eventData[2] = newProgram;
    m_eventSize = 3;
}

void MTrkEvent::trackEnd(int interval)
{
    int vlvSize = convertToVLV(interval, 0);
    m_eventData[vlvSize+1] = 0xff;
    m_eventData[vlvSize+2] = 0x2f;
    m_eventData[vlvSize+3] = 0;
    m_eventSize = vlvSize+4;
}

void MTrkEvent::metaTimeSignature(int numerator, int denominator, int clocksPerTick, int nrOf32thNotesPer24Clocks) {
    m_eventData[0] = 0;
    m_eventData[1] = 0xff;
    m_eventData[2] = 0x58;
    m_eventData[3] = 0x04;
    m_eventData[4] = numerator;
    m_eventData[5] = denominator; // actual denom is this value power to two, e.g. value 3 here means actual 8
    m_eventData[6] = clocksPerTick;
    m_eventData[7] = nrOf32thNotesPer24Clocks;
    m_eventSize = 8;
}

void MTrkEvent::metaKeySignature(int sharpsFlats, int majorMinor)
{
    m_eventData[0] = 0;
    m_eventData[1] = 0xff;
    m_eventData[2] = 0x59;
    m_eventData[3] = 0x02;
    m_eventData[4] = sharpsFlats; // -7=7 flats, +7=7 sharps
    m_eventData[5] = majorMinor; // 0=major, 1=minor
    m_eventSize = 6;
}

void MTrkEvent::metaSetTempo(int bpm)
{
    int32_t microSecsPerQuarter = 60000000/bpm;
    m_eventData[0] = 0;
    m_eventData[1] = 0xff;
    m_eventData[2] = 0x51;
    m_eventData[3] = 0x03;
    // convert 3 lowest bytes to big endian
    std::memcpy(&m_eventData[4], (char*)&microSecsPerQuarter + 2, 1);
    std::memcpy(&m_eventData[5], (char*)&microSecsPerQuarter + 1, 1);
    std::memcpy(&m_eventData[6], (char*)&microSecsPerQuarter, 1);
    m_eventSize = 7;
}

void MTrkEvent::metaTextEvent(std::string text)
{
    m_eventData[0] = 0;
    m_eventData[1] = 0xff;
    m_eventData[2] = 0x01;
    m_eventData[3] = text.length();
    std::memcpy(&m_eventData[4], &text.at(0), text.length());
    m_eventSize = 4 + text.length();
}

int MTrkEvent::convertToVLV(int value, int startIndex) {
    Byte tempVLV[4];
    int vlvSize = 0;

    for (int i = 0; i < 4; i++){
        tempVLV[i] = value & 127;
        if (tempVLV[i] > 0)
            vlvSize = i;
        value = value >> 7;
    }

    for (int i = vlvSize, j = 0; i >= 0; i--, j++)
        m_eventData[startIndex + j] = 128 | tempVLV[i];
    m_eventData[startIndex + vlvSize] &= 127;
    /*
    std::cout << "eventdata " << (int)_eventData[startIndex+vlvSize] << " ";
    std::cout << "eventdata0 " << (int)_eventData[startIndex] ;*/
    return vlvSize;
}
