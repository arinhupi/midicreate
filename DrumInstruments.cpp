/*
 * DrumInstruments.cpp
 *
 *  Created on: 4.3.2018
 *      Author: ari
 */

#include "DrumInstruments.h"
#include <iostream>


std::map<std::string, Byte> _drumMap2 = {
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


namespace DrumInstruments{

Byte getInstrument(const std::string& instrName) {
	Byte drumValue;
	try {
		drumValue = _drumMap2.at(instrName);
	}
	catch (...) {
		std::cout << "Drum instrument " << instrName << " not found\n";
		throw;
	};
	return drumValue;
}

void dispDrumInstruments() {
	std::cout << "Available percussion instruments based on the General Midi standard.\n";
	std::cout << "Use these when adding new drum patterns to patterns.conf file.\n\n";
	for (auto i : _drumMap2)
		std::cout << i.first << "  ";

	std::cout << "\n\n";
}

}


