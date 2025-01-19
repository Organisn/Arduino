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
char playingNote = ' ';
// Reset built-in func
void(* resetFunc) (void) = 0;

void setup() {
  pinMode(13, INPUT); // C button
  pinMode(11, INPUT); // E button
  pinMode(9, INPUT); // F button
  pinMode(7, INPUT); // G button
  pinMode(5, INPUT); // A button
  pinMode(2, OUTPUT); // Buzzer
}

void loop() {
  // Read current time
  lastTime = millis();
  // Reset board after 2 minutes
  if (lastTime > 120000) resetFunc();
  // Check a button pin status variation only if buzzer's free to play or is already playing the note it is about
  // (Otherwise it would be only a waste of time and energy, since buzzer can only play single notes per time...)
  if (playingNote == ' ' || playingNote == 'c') {
    cReading = digitalRead(13);
    if (cReading != lastCButtonState) lastCDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastCDebounceTime) > debounceDelay) { // Button pressure or release!
      if (cReading != cButtonState) cButtonState = cReading;
      // Emit tone and set buzzer as not available when button's pressed
      if (cButtonState == HIGH) {
        tone(2, C3, 10);
        playingNote = 'c';
      }
      // Otherwise, free buzzer..
      else {
        noTone(2);
        playingNote = ' ';
      }
    }
  }
  // Do the same done with C button to others... 
  if (playingNote == ' ' || playingNote == 'e') {
    eReading = digitalRead(11);
    if (eReading != lastEButtonState) lastEDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastEDebounceTime) > debounceDelay) { // Button pressure or release!
      if (eReading != eButtonState) eButtonState = eReading;
      // Emit tone and set buzzer as not available when button's pressed
      if (eButtonState == HIGH) {
        tone(2, E3, 10);
        playingNote = 'e';
      }
      // Otherwise, free buzzer..
      else {
        noTone(2);
        playingNote = ' ';
      }
    }
  }
  if (playingNote == ' ' || playingNote == 'f') {
    fReading = digitalRead(9);
    if (fReading != lastFButtonState) lastFDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastFDebounceTime) > debounceDelay) { // Button pressure or release!
      if (fReading != fButtonState) fButtonState = fReading;
      // Emit tone and set buzzer as not available when button's pressed
      if (fButtonState == HIGH) {
        tone(2, F3, 10);
        playingNote = 'f';
      }
      // Otherwise, free buzzer..
      else {
        noTone(2);
        playingNote = ' ';
      }
    }
  }
  if (playingNote == ' ' || playingNote == 'g') {
    gReading = digitalRead(7);
    if (gReading != lastGButtonState) lastGDebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastGDebounceTime) > debounceDelay) { // Button pressure or release!
      if (gReading != gButtonState) gButtonState = gReading;
      // Emit tone and set buzzer as not available when button's pressed
      if (gButtonState == HIGH) {
        tone(2, G3, 10);
        playingNote = 'g';
      }
      // Otherwise, free buzzer..
      else {
        noTone(2);
        playingNote = ' ';
      }
    }
  }
  if (playingNote == ' ' || playingNote == 'a') {
    aReading = digitalRead(5);
    if (aReading != lastAButtonState) lastADebounceTime = lastTime; // Button pressure, release or debounce!
    if ((lastTime - lastADebounceTime) > debounceDelay) { // Button pressure or release!
      if (aReading != aButtonState) aButtonState = aReading;
      // Emit tone and set buzzer as not available when button's pressed
      if (aButtonState == HIGH) {
        tone(2, A3, 10);
        playingNote = 'a';
      }
      // Otherwise, free buzzer..
      else {
        noTone(2);
        playingNote = ' ';
      }
    }
  }
}