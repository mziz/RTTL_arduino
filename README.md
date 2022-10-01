# RTTTL_arduino
RTTTL format parser for Arduino

Author: Milda Zizyte

Developed for Lab 4 (Clocks, timers, and watchdogs) in CSCI 1600: Embedded and real-time software at Brown University (more information [here](https://cs.brown.edu/courses/info/csci1600/).)

If you use or modify this library for developing educational materials, please drop me a line. Credit for using this code for any project is appreciated.

## Overview

This repository includes code to parse an RTTTL string into buffers of note frequencies and durations. It can then be used in conjunction with e.g. the [Tone library](https://github.com/daniel-centore/arduino-tone-library).

Examples of RTTTL-formatted songs can be found [here](http://www.cellringtones.com/)

## Usage

You can download the files as-is and run `rtttl_parser_test` on an Arduino with the Serial Monitor open. This code runs some unit tests for the library, and you should see that the tests pass.

To use these files for your own Arduino project, copy `rtttl_parser` and `pitches.h` into your project folder. Then, you can parse an RTTTL string using the `rtttl_to_buffers` function.

Example usage:

```
#include "pitches.h"

...

const String song = "spooky:d=4,o=6,b=127:8c,f,8a,f,8c,b5,2g,8f,e,8g,e,8e5,a5,2f,8c,f,8a,f,8c,b5,2g,8f,e,8c,d,8e,1f,8c,8d,8e,8f,1p,8d,8e,8f_,8g,1p,8d,8e,8f_,8g,p,8d,8e,8f_,8g,p,c,8e,1f";
int note_frequencies[100];
int note_durations[100];
int song_len;

song_len = rtttl_to_buffers(song, note_frequencies, note_durations);
if (song_len == -1) {
    Serial.println("ERROR PARSING SONG!");
    while(true);
}
// note_frequencies and note_durations now contains the frequencies and durations for the song, in sequence
```
  
