// Note frequencies from public local library
#include "pitches.h"
// C, E, F, G, A pins status
int cReading;
int eReading;
int fReading;
int gReading;
int aReading;
// Memorize last buttons status: pressure, release or debounce
int lastCButtonState = LOW;
int lastEButtonState = LOW;
int lastFButtonState = LOW;
int lastGButtonState = LOW;
int lastAButtonState = LOW;
// Memorize definitive buttons pressures or releases
int cButtonState = LOW;
int eButtonState = LOW;
int fButtonState = LOW;
int gButtonState = LOW;
int aButtonState = LOW;
// Memorize each button pressure or debounce time
unsigned long lastCDebounceTime = 0;
unsigned long lastEDebounceTime = 0;
unsigned long lastFDebounceTime = 0;
unsigned long lastGDebounceTime = 0;
unsigned long lastADebounceTime = 0;
// Debounce lapse
unsigned long debounceDelay = 50;
// Memorize time from program launch
unsigned long lastTime;
// Reset built-in func
void(* resetFunc) (void) = 0;
// Shift registers as 74HC595 turn out useful to save board IO pins (5, to be exact)
// as when needed to turn on 7-segment (+ dot) displays
// 74HC595 can store 8 bits at a time 
// Each bit is dedicated to a specific display segment, as connected in the scheme:
//     Q7
//     _
// Q2 |_| Q6
//    Q1
// Q3 |_| Q5 .Q0
//     Q4
// It is helpful to previously memorize each LEDs configuration,
// keeping in mind pins representation into shifting byte:
// (from most significative bit) Q7 Q6 Q5 Q4 Q3 Q2 Q1 Q0 (ex: 11111100 (=252) lights as 0)
const uint8_t c = 0b10011100;
const uint8_t e = 0b10011110;
const uint8_t f = 0b10001110;
const uint8_t g = 0b10111110;
const uint8_t a = 0b11101110;
const uint8_t r = 0b00001010; // 'r' for the board reset announce
const uint8_t off = 0b00000000;
// (Binary bytes can be memorized also as decimal integers)
uint8_t playingNote = off;

void setup() {
  pinMode(13, OUTPUT); // Buzzer
  pinMode(12, INPUT); // C button
  pinMode(11, INPUT); // E button
  pinMode(10, INPUT); // F button
  pinMode(9, INPUT); // G button
  pinMode(8, INPUT); // A button
  // 74HC595 pins
  // DS: serial data output pin
  pinMode(4, OUTPUT);
  // ST_CP (storage register clock pin): data transmission enabler pin
  pinMode(3, OUTPUT);
  // SH_CP (shift register clock pin): serial data transmission clock pin
  pinMode(2, OUTPUT);
  // Buzzer and display check
  /*
  digitalWrite(3, LOW);
  shiftOut(4, 2, MSBFIRST, a);
  digitalWrite(3, HIGH);
  tone(13, A4);
  delay(500);
  noTone(13);
  */
}

void loop() {
  // Read current time
  lastTime = millis();
  // Reset board after 2 minutes
  if (lastTime > 120000) {
    // Display an 'r' for 2s..
    digitalWrite(3, LOW);
    shiftOut(4, 2, MSBFIRST, r);
    digitalWrite(3, HIGH);
    delay(2000);
    resetFunc();
  }
  // Check a button pin status variation only if buzzer's free to play or is already playing the note it is about
  // (Otherwise it would be only a waste of time and energy, since buzzer can only play single notes per time...)
  if (playingNote == off || playingNote == c) {
    cReading = digitalRead(12);
    if (cReading == HIGH) cButtonState = HIGH; // No need to wait debounce lapse to start making noise
    // (we aren't affecting dangerous actuators pushing these buttons..)
    // Emit tone and set buzzer as not available for other notes when button's pressed
    if (cButtonState == HIGH) {
      // Display note
      digitalWrite(3, LOW); //ground ST_CP and hold low for as long as you are transmitting
      shiftOut(4, 2, MSBFIRST, c); // Shift out data mode: Most Significant Bit FIRST
      // shiftOut() can transmit only 8 bits per step
      // (since we need not more than a byte to light up 8 LEDs, this time it is not required to call it two times..)
      // Return the latch pin high to signal chip that it no longer needs to listen for information
      // This will let 74HC595 to move data from shift to memory register (actually saving it..)
      digitalWrite(3, HIGH);
      tone(13, C5);
      delay(20);
      noTone(13);
      playingNote = c;
    }
    if (cReading != lastCButtonState) lastCDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastCDebounceTime) > debounceDelay) { // Button definitely pressed or released!
      if (cReading != cButtonState) cButtonState = cReading;
      // Free buzzer and display when button is released...
      if (cButtonState == LOW) {
        digitalWrite(3, LOW);
        shiftOut(4, 2, MSBFIRST, 0);
        digitalWrite(3, HIGH);
        playingNote = off;
      }
    }
    lastCButtonState = cReading;
  }
  // Do the same done with C button to others... 
  if (playingNote == off || playingNote == e) {
    eReading = digitalRead(11);
    if (eReading == HIGH) eButtonState = HIGH;
    // Emit tone and set buzzer as not available when button's pressed
    if (eButtonState == HIGH) {
      digitalWrite(3, LOW);
      shiftOut(4, 2, MSBFIRST, e);
      digitalWrite(3, HIGH);
      tone(13, E5);
      delay(15);
      noTone(13);
      playingNote = e;
    }
    if (eReading != lastEButtonState) lastEDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastEDebounceTime) > debounceDelay) { // Button pressure or release!
      if (eReading != eButtonState) eButtonState = eReading;
      // Free buzzer and display when button is released...
      if (eButtonState == LOW) {
        digitalWrite(3, LOW);
        shiftOut(4, 2, MSBFIRST, 0);
        digitalWrite(3, HIGH);
        playingNote = off;
      }
    }
    lastEButtonState = eReading;
  }
  if (playingNote == off || playingNote == f) {
    fReading = digitalRead(10);
    if (fReading == HIGH) fButtonState = HIGH;
    // Emit tone and set buzzer as not available when button's pressed
    if (fButtonState == HIGH) {
      digitalWrite(3, LOW);
      shiftOut(4, 2, MSBFIRST, f);
      digitalWrite(3, HIGH);
      tone(13, F5);
      delay(10);
      noTone(13);
      playingNote = f;
    }
    if (fReading != lastFButtonState) lastFDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastFDebounceTime) > debounceDelay) { // Button pressure or release!
      if (fReading != fButtonState) fButtonState = fReading;
      // Free buzzer and display when button is released...
      if (fButtonState == LOW) {
        digitalWrite(3, LOW);
        shiftOut(4, 2, MSBFIRST, 0);
        digitalWrite(3, HIGH);
        playingNote = off;
      }
    }
    lastFButtonState = fReading;
  }
  if (playingNote == off || playingNote == g) {
    gReading = digitalRead(9);
    if (gReading == HIGH) gButtonState = HIGH;
    // Emit tone and set buzzer as not available when button's pressed
    if (gButtonState == HIGH) {
      digitalWrite(3, LOW);
      shiftOut(4, 2, MSBFIRST, g);
      digitalWrite(3, HIGH);
      tone(13, G5);
      delay(25);
      noTone(13);
      playingNote = g;
    }
    if (gReading != lastGButtonState) lastGDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastGDebounceTime) > debounceDelay) { // Button pressure or release!
      if (gReading != gButtonState) gButtonState = gReading;
      // Free buzzer and display when button is released...
      if (gButtonState == LOW) {
        digitalWrite(3, LOW);
        shiftOut(4, 2, MSBFIRST, 0);
        digitalWrite(3, HIGH);
        playingNote = off;
      }
    }
    lastGButtonState = gReading;
  }
  if (playingNote == off || playingNote == a) {
    aReading = digitalRead(8);
    if (aReading == HIGH) aButtonState = HIGH;
    // Emit tone and set buzzer as not available when button's pressed
    if (aButtonState == HIGH) {
      digitalWrite(3, LOW);
      shiftOut(4, 2, MSBFIRST, a);
      digitalWrite(3, HIGH);
      tone(13, A5);
      delay(25);
      noTone(13);
      playingNote = a;
    }
    if (aReading != lastAButtonState) lastADebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastADebounceTime) > debounceDelay) { // Button pressure or release!
      if (aReading != aButtonState) aButtonState = aReading;
      // Free buzzer and display when button is released...
      if (aButtonState == LOW) {
        digitalWrite(3, LOW);
        shiftOut(4, 2, MSBFIRST, 0);
        digitalWrite(3, HIGH);
        playingNote = off;
      }
    }
    lastAButtonState = aReading;
  }
}