# midicreate
Creates a midi file for the drum/percussion track.

Drum track consists of 1 or more sections.
Each section contains 1 or more rhythm patterns (=bars) and tempo information.
Pattern contains the instrument-time-volume information. Default patterns are defined in configuration file patterns.conf. Users are encouraged to add new patters.

Use midi player (e.g. timidity) to play the file.
Use sequencer program (e.g. Rosegarden or Qtracktor) to use the created midi track in a music composition.

Program Arguments
-s patternName nBars tempo [fillPatternName n]
-d list all percussion instrument names which can be used in patterns.conf

Usage example, create track consisting of three sections. First one is
a 2 bar intro section, then 8 bar section of pattern 'beat' and the last
one is 12 bars of 'rock2' pattern. Tempo in all sections is 110 bpm.

midicreate -s intro 2 110 -s beat 8 110 -s rock2 12 110 fill1 4

