#include "rtttl_parser.h"

/**
 * Unit tests for RTTTL parser (code for converting RTTTL format to frequency/duration)
 * Developed as a helper library for Lab 4 (Clocks, Timers, and Watchdogs)
 * in CSCI 1600 (Real-time and embedded software) at Brown University
 *    (https://cs.brown.edu/courses/info/csci1600/)
 * Author: Milda Zizyte
 */

void print_error(String msg, String varname, int expected, int actual) {
  Serial.println(msg);
  Serial.print("Expected ");
  Serial.print(varname);
  Serial.print(": ");
  Serial.print(expected);
  Serial.print(", actual: ");
  Serial.println(actual);
  while(true);
}

int f[10];
int d[10];
int l;
  
void run_test_single_note(int test_no, String rtttl, int exp_f, int exp_d) {
  char itoa_buf[12];
  itoa(test_no, itoa_buf, 10);
  Serial.print("Running test ");
  Serial.println(itoa_buf);
  l = rtttl_to_buffers(rtttl, f, d);
  String e_str = "Test " + (String) itoa_buf + (String) " failed";
  if (l != 1) {
    print_error(e_str, "length", 1, l);
  }
  if (f[0] != exp_f) {
    print_error(e_str, "frequency", exp_f, f[0]);
  }
  if (d[0] != exp_d) {
    print_error(e_str, "duration", exp_d, d[0]);
  }
  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  while(!Serial);

  int test_no = 1;
  /**
   * Test 1: Single note
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4c4", NOTE_C4, 1000);  
  test_no++;

  /**
   * Test 2: Single note, default octave
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4c", NOTE_C4, 1000);  
  test_no++;

  /**
   * Test 3: Single note, different octave
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4c6", 1047, 1000);  
  test_no++;

  /**
   * Test 4: Single note, default duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c4", NOTE_C4, 1000);  
  test_no++;

  /**
   * Test 5: Single note, half duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:8c4", NOTE_C4, 500);  
  test_no++;

  /**
   * Test 6: Single note, double duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:2c4", NOTE_C4, 2000);  
  test_no++;

  /**
   * Test 7: Single note, dot duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4c4.", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 8: Single note, fourth duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:16c4", NOTE_C4, 250);  
  test_no++;

  /**
   * Test 9: Single note, eighth duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:32c4", NOTE_C4, 125);  
  test_no++;

  /**
   * Test 10: Single note, quadruple duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:1c4", NOTE_C4, 4000);  
  test_no++;

  /**
   * Test 11: Single note, eighth default
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=8:c4", NOTE_C4, 1000);  
  test_no++;

  /**
   * Test 12: Single note, eighth default w/ duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=8:8c4", NOTE_C4, 1000);  
  test_no++;

  /**
   * Test 13: Single note, eighth default, fourth duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=8:4c4", NOTE_C4, 2000);  
  test_no++;

  /**
   * Test 14: Single note, eighth default, sixteenth duration
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=8:16c4", NOTE_C4, 500);  
  test_no++;
  
  /**
   * Test 15: C
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c", NOTE_C4, 1000);  
  test_no++;

  /**
   * Test 16: C#
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c_", NOTE_CS4, 1000);  
  test_no++;

  /**
   * Test 17: D
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:d", NOTE_D4, 1000);  
  test_no++;

  /**
   * Test 18: D#
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:d_", NOTE_DS4, 1000);  
  test_no++;

  /**
   * Test 19: E
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:e", NOTE_E4, 1000);  
  test_no++;

  /**
   * Test 20: E# (F)
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:e_", NOTE_F4, 1000);  
  test_no++;

  /**
   * Test 21: F
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:f", NOTE_F4, 1000);  
  test_no++;

  /**
   * Test 22: F#
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:f_", NOTE_FS4, 1000);  
  test_no++;

  /**
   * Test 23: G
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:g", NOTE_G4, 1000);  
  test_no++;

  /**
   * Test 24: G#
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:g_", NOTE_GS4, 1000);  
  test_no++;

  /**
   * Test 25: A
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:a", NOTE_A4, 1000);  
  test_no++;

  /**
   * Test 26: A#
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:a_", NOTE_AS4, 1000);  
  test_no++;

  /**
   * Test 27: B
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:b", NOTE_B4, 1000);  
  test_no++;

  /**
   * Test 28: B# (C one octave up)
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:b_", NOTE_C5, 1000);  
  test_no++;

  /**
   * Test 29: dot variation 1
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4.c4", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 30: dot variation 2
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4c.4", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 31: dot variation 3
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4c.", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 32: dot variation 4
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:4.c", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 33: dot variation 5
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c.4", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 34: dot variation 6
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c4.", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 35: dot variation 7
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c.", NOTE_C4, 1500);  
  test_no++;

  /**
   * Test 36: dot variation 8
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c_.4", NOTE_CS4, 1500);  
  test_no++;

  /**
   * Test 37: dot variation 9
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c_4.", NOTE_CS4, 1500);  
  test_no++;

  /**
   * Test 38: dot variation 10
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:c_.", NOTE_CS4, 1500);  
  test_no++;

  /**
   * Test 39: dot with eighth note
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:8c.", NOTE_C4, 750);  
  test_no++;

  /**
   * Test 40: rest
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:p", 0, 1000);  
  test_no++;

  /**
   * Test 41: eighth note rest
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:8p", 0, 500);  
  test_no++;

  /**
   * Test 42: dotted rest
   */
  run_test_single_note(test_no, "a:b=60,o=4,d=4:p.", 0, 1500);
  test_no++;

  /**
   * Test 43: multiple notes
   */
  Serial.println("Running test 43");
  l = rtttl_to_buffers("a:b=60,o=4,d=4:b5,a,8c,8f_,2g7,p.,b_", f, d);
  const int song_len = 7;
  int expected_f[song_len] = {NOTE_B5, NOTE_A4, NOTE_C4, NOTE_FS4, NOTE_G7, 0, NOTE_C5};
  int expected_d[song_len] = {1000, 1000, 500, 500, 2000, 1500, 1000};
  String e_str = "Test 43 failed";
  if (l != song_len) {
    print_error(e_str, "length", song_len, l);
  }
  for(int i = 0; i < song_len; i++) {
    if (f[i] != expected_f[i]) {
      print_error(e_str, (String) "f[" + (String)(char)(i + (int) '0') + (String) "]", expected_f[i], f[i]);
    }
    if (d[i] != expected_d[i]) {
      print_error(e_str, (String) "d[" + (String)(char)(i + (int) '0') + (String) "]", expected_d[i], d[i]);
    }
  }

  Serial.println("");
  Serial.println("All tests passed!");
}

void loop() {

}
