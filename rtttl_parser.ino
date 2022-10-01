/*
 * Code for converting RTTTL format to frequency/duration
 * Developed as a helper library for Lab 4 (Clocks, Timers, and Watchdogs)
 * in CSCI 1600 (Real-time and embedded software) at Brown University
 *    (https://cs.brown.edu/courses/info/csci1600/)
 * Author: Milda Zizyte
 */
 
void print_error(int location, char c, String msg) {
  Serial.print("Character (indexed on spaceless string) ");
  Serial.print(location);
  Serial.print(": ");
  Serial.print(msg);
  Serial.print(" [received ");
  Serial.print(c);
  Serial.println("]");
}

/*
 * Convert an RTTTL string to sequential lists of their frequencies (Hz) and durations (ms)
 * A frequency of "0" indicates that the note is actually a rest
 * Inputs:
 *   rtttl: RTTTL music format string
 *      (we allow for notes B0-D#8 inclusive, and for flexibility in dot position)
 *      e.g. if default duration = 4 and octave = 4, a dotted quarter C4 note could be expressed as:
 *      c., c.4, c4., 4.c, 4c., 4.c4, 4c.4, 4c4.
 *   notes: buffer in which to put the frequencies
 *   durs: buffer in which to put the durations
 * Outputs:
 *   length of the song (in notes/rests). A return value of -1 indicates an error.
 */

int rtttl_to_buffers(String rtttl, int notes[], int durs[]) {
  int si = 0;

  // convert rtttl string to lowercase and remove spaces
  String rtttl_nosp = "";
  for (int i = 0; i < rtttl.length(); i++) {
    if (rtttl[i] >= 'A' and rtttl[i] <= 'Z') {
      rtttl_nosp.concat(rtttl[i] - 'A' + 'a');
    } else if (rtttl[i] != ' ') {
      rtttl_nosp.concat(rtttl[i]);
    }
  }
  if (rtttl_nosp.length() == 0) {
    Serial.println("Cannot parse empty string");
    return -1;
  }
  
  // RTTTL format example
  // spooky:d=4,o=6,b=127:8c,f,8a,f,8c,b5,2g,8f,e,8g,e,8e5,a5,2f,8c,f,8a,f,8c,b5,2g,8f,e,8c,d,8e,1f,8c,8d,8e,8f,1p,8d,8e,8f_
  
  // find starting index (assumes non-empty song name terminated in : character)
  while(rtttl_nosp[si] != ':') {
    si += 1;
    if (si >= rtttl_nosp.length()) {
      print_error(si, '-', "Expected colon after song name");
      return -1;
    }
  }

  // starting index is 1 past index of :
  si += 1;
  // need beat, octave, and duration defined
  if (si >= rtttl_nosp.length()) {
    print_error(si, '-', "Expected beat, octave, duration");
    return -1;
  }

  // Since b,o,d can appear in any order, we create flags to indicate if they were all found in the string
  bool b_found = false;
  bool o_found = false;
  bool d_found = false;

   // store beat, order, duration when found
  int bod[3];

  while (!b_found or !o_found or !d_found) {
    int bod_index;
    // check for next character being b, o, or d, and store the corresponding index in bod_index
    // (b: index 0, o: index 1, d: index 2)
    if (rtttl_nosp[si] == 'b') {
      if (b_found) {
        print_error(si, 'b', "b defined twice");
        return -1;
      }
      bod_index = 0;
      b_found = true;
    } else if (rtttl_nosp[si] == 'o') {
      if (o_found) {
        print_error(si, 'o', "o defined twice");
        return -1;
      }
      bod_index = 1;
      o_found = true;
    } else if (rtttl_nosp[si] == 'd') {
      if (d_found) {
        print_error(si, 'd', "d defined twice");
        return -1;
      }
      bod_index = 2;
      d_found = true;
    } else {
      print_error(si, rtttl_nosp[si], "Expected b, o, or d");
      return -1;
    }
    // next character expected is =
    si += 1;
    if (si >= rtttl_nosp.length() or rtttl_nosp[si] != '=') {
      print_error(si, rtttl_nosp[si], "Expected beat, octave, duration");
      return -1;
    }
    // next character(s) expected are numeric
    si += 1;
    if (si >= rtttl_nosp.length() or rtttl_nosp[si] < '0' or rtttl_nosp[si] > '9') {
      print_error(si, rtttl_nosp[si], "Expected number for b,o,d");
      return -1;
    }
    // convert the next numeric symbols to a decimal number
    bod[bod_index] = (int) rtttl_nosp[si] - (int) '0';
    si += 1;
    if (si >= rtttl_nosp.length()) {
       print_error(si - 1, rtttl_nosp[si - 1], "Expected beat, order, duration or : after character");
       return -1;
    }
    while (rtttl_nosp[si] >= '0' and rtttl_nosp[si] <= '9') {
      bod[bod_index] *= 10;
      bod[bod_index] += (int) rtttl_nosp[si] - (int) '0';
      si += 1;
      if (si >= rtttl_nosp.length()) {
        print_error(si - 1, rtttl_nosp[si - 1], "Expected beat, octave, duration or : after character");
        return -1;
      }
    }

    // next character (after e.g. b=40) is expected to be comma or colon
    if (rtttl_nosp[si] != ',' and rtttl_nosp[si] != ':') {
      print_error(si, rtttl_nosp[si], "Expected comma or colon");
      return -1;
    }

    // but we only expect a colon after all of b,o,d are defined
    if (rtttl_nosp[si] == ':' and (!b_found or !o_found or !d_found)) {
      print_error(si, ';', "Colon before all of b,o,d defined");
      return -1;
    }

    si += 1;
    if (si >= rtttl_nosp.length()) {
      print_error(si, rtttl_nosp[si - 1], "Expected beat, octave, duration or :");
      return -1;
    }
  }
  
  // after we've stored bod and gotten past the :, time to parse the notes
  // From wikipedia:
  // Each note is separated by a comma and includes, in sequence:
  //    a duration specifier (1 = whole note, 2 = half note, etc)
  //    a standard music note, either a, b, c, d, e, f or g*
  //    an octave specifier, as in scientific pitch notation
  // If no duration or octave specifier are present, the default applies.
  //
  // * A # or _ after the note letter indicates a sharp
  // A . after the note indicates dotted notation 
  // We allow for the . to come after the duration, the note letter, or the octave
  
  int note_ind = 0;

  bool prev_comma_found = true;
  while (si < rtttl_nosp.length()) {
    if (! prev_comma_found) {
      print_error(si, rtttl_nosp[si], "Expected commas to separate notes!");
      return -1;
    }
    prev_comma_found = false;
    // If the next symbols are numeric, then we get the duration
    int note_dur = 0;
    bool dur_found = false;
    while (rtttl_nosp[si] >= '0' and rtttl_nosp[si] <= '9') {
      dur_found = true;
      note_dur *= 10;
      note_dur += (int) rtttl_nosp[si] - (int) '0';
      si += 1;
      if (si >= rtttl_nosp.length()) {
        print_error(si-1, rtttl_nosp[si - 1], "Expected note name");
        return -1;
      } 
    }
    // Compute the duration of the note
    // 1 beat = 1 default duration
    // 1000 * 60 / beat = 1000 * 60 / bod[0] is the duration of one default duration beat in ms
    // default_duration * 1000 * 60 / (beat * duration) = bod[2] * 1000 * 60 / (bod[0] * note_dur) is the duration of one beat in ms
    // (for example, if default_duration = 4 and node_dur = 8, then the duration is 1/2 of the default
    if (dur_found) { 
      durs[note_ind] = bod[2] * 1000 * 60 / (bod[0] * note_dur);
      // check for dotted notation, which multiplies duration by 1.5
      if (rtttl_nosp[si] == '.') {
        durs[note_ind] += durs[note_ind] / 2;
        si += 1;
        if (si >= rtttl_nosp.length()) {
          print_error(si-1, rtttl_nosp[si-1], "Expected note name");
        }
      }
    } else{
      durs[note_ind] = 1000 * 60 / bod[0];
    }

    // Next, parse the note. Pull in the default octave as a starting point
    int note_octave = bod[1];
    
    // p indicates rest
    if(rtttl_nosp[si] == 'p') {
      notes[note_ind] = 0;
      si += 1;
      // rests can be dotted too
      if (si < rtttl_nosp.length() and rtttl_nosp[si] == '.') {
        durs[note_ind] += durs[note_ind]/2;
        si += 1;
      }
    } else {
      // get the base of the note (a-g)
      if (rtttl_nosp[si] < 'a' or rtttl_nosp[si] > 'g') {
        print_error(si, rtttl_nosp[si], "Expected a-g or p");
        return -1;
      }
      char note_base = rtttl_nosp[si];
      si += 1;
      bool sharp = false;
      if (si < rtttl_nosp.length()) {
        // detect a sharp
        if (rtttl_nosp[si] == '#' or rtttl_nosp[si] == '_') {
          sharp = true;
          si += 1;
        }
        // check for dotted notation
        if (si < rtttl_nosp.length() and rtttl_nosp[si] == '.') {
          durs[note_ind] += durs[note_ind]/2;
          si += 1;
        }
        // check for octave modifier
        // technically RTTTL only allows for octaves 4-7, but we have more notes defined than that so we might
        // as well allow for some flexibility
        if (si < rtttl_nosp.length() and rtttl_nosp[si] >= '0' and rtttl_nosp[si] <= '8') {
          note_octave = (int) rtttl_nosp[si] - (int) '0';
          si += 1;
          // check once more for dotted notation
          if (si < rtttl_nosp.length() and rtttl_nosp[si] == '.') {
            durs[note_ind] += durs[note_ind]/2;
            si += 1;
          }
        }
      }

      // first note in all_notes is B0 (index of B0 = 0), so
      // "Index" of C0 : - 11
      // Since there are 12 notes in an octave, we compute the index of C[octave] with the following formula:
      int note_def_location = -11 + 12 * note_octave;
      
      // compute offset from c
      // notes go:
      // c, c#, d, d#, e, f, f#, g, g#, a, a#, b
      // 0   1  2   3  4  5   6  7   8  9  10  11
      if (note_base == 'd') {
        note_def_location += 2;
      } else if (note_base == 'e') {
        note_def_location += 4;
      } else if (note_base == 'f') {
        note_def_location += 5;
      } else if (note_base == 'g') {
        note_def_location += 7;
      } else if (note_base == 'a') {
        note_def_location += 9;
      } else if (note_base == 'b') {
        note_def_location += 11;
      }
    
      // this will correctly find the index of a sharp note
      // (even for e# and b#, since e + 1 = f and b + 1 = c in the next octave)
      if (sharp) {
        note_def_location += 1;
      }

      if (note_def_location < 0 or note_def_location >= 89) {
        print_error(si, rtttl_nosp[si], "Unsupported note frequency");
        return -1;
      }
      
      // look up the note in the all_notes array
      notes[note_ind] = all_notes[note_def_location];
    }
 
    // detect comma for error checking at start of loop
    // (this takes care of the fencepost of no comma needed at the end of a song)
    if (si < rtttl_nosp.length() and rtttl_nosp[si] == ',') {
      prev_comma_found = true;
      si += 1;
    }

    note_ind += 1;
  }

  return note_ind;
}
