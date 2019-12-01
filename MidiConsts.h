/*
 * MidiConsts.h
 *
 *  Created on: 19.1.2018
 *      Author: ari
 */

#include <map>

#ifndef MIDICONSTS_H_
#define MIDICONSTS_H_

#define Byte unsigned char

#define OCT_3_NOTE_C 0x24
#define OCT_4_NOTE_C 0x30
#define OCT_5_NOTE_C 0x3c

// general midi percussion instruments
#define DRUM_BASS1 35
#define DRUM_BASS2 36
#define DRUM_SIDE_STICK 37
#define DRUM_SNARE1 38
#define DRUM_HAND_CLAP 39
#define DRUM_SNARE2 40
#define DRUM_TOM_LOW_FLOOR 41
#define DRUM_HIHAT_CLOSED 42
#define DRUM_TOM_HIGH_FLOOR 43
#define DRUM_HIHAT_PEDAL 44
#define DRUM_TOM_LOW 45
#define DRUM_HIHAT_OPEN 46
#define DRUM_TOM_LOW_MID 47
#define DRUM_TOM_HI_MID 48
#define DRUM_CYMBAL_CRASH 49
#define DRUM_TOM_HIGH 50
#define DRUM_CYMBAL_RIDE 51
#define DRUM_CYMBAL_CHINESE 52
#define DRUM_RIDE_BELL 53
#define DRUM_TAMBOURINE 54
#define DRUM_CYMBAL_SPLASH 55
#define DRUM_COWBELL 56
#define DRUM_CYMBAL_CRASH2 57
#define DRUM_VIBRASLAP 58
#define DRUM_CYMBAL_RIDE2 59
#define DRUM_BONGO_HI 60
#define DRUM_BONGO_LOW 61
#define DRUM_CONGA_MUTE_HI 62
#define DRUM_CONGA_OPEN_HI 63
#define DRUM_CONGA_LOW 64
#define DRUM_TIMBALE_HIGH 65
#define DRUM_TIMBALE_LOW 66
#define DRUM_AGOGO_HIGH 67
#define DRUM_AGOGO_LOW 68
#define DRUM_CABASE 69
#define DRUM_MARACAS 70
#define DRUM_SHORT_WHISTLE 71
#define DRUM_LONG_WHISTLE 72
#define DRUM_GUIRO_SHORT 73
#define DRUM_GUIRO_LONG 74
#define DRUM_CLAVES 75
#define DRUM_WOOD_BLOCK_HI 76
#define DRUM_WOOD_BLOCK_LOW 77
#define DRUM_CUICA_MUTE 78
#define DRUM_CUICA_OPEN 79
#define DRUM_TRIANGLE_MUTE 80
#define DRUM_TRIANGLE_OPEN 81


#define KEY_MAJOR 0
#define KEY_MINOR 1
#define KEY_C 0

#define TICKS_PER_QN 96

#define NOTE_SILENCE 0xff
#define DRUM_CHANNEL 9
#define VOLUME_DEFAULT 60

class MidiConsts {
public:
    MidiConsts();
    virtual ~MidiConsts();
    char getNoteNumber(int octave, std::string noteName);
};

#endif /* MIDICONSTS_H_ */

