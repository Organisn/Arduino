// Note frequencies from public local library
#include "pitches.h"

// Analog readings from lower [0], middle [1] and higher [2] strings
int resistances[3];
unsigned int highestPressedKey = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting reading..");
  // Passive buzzer
  // Must be powered with PWM
  // (Since hw can handle one single tone() instance a time, 
  // it is impossible (without direct timers configuration, maybe, at least) 
  // to play different notes on different speakers simultaneously..)
  pinMode(3, OUTPUT);
}

void loop() {
  for (byte i = 0; i < 1; i = i + 1) { // !!! i < 3 !!!
    resistances[i] = analogRead(i); // IMPLEMENT BUTTON DEBOUNCE? AND SIGNAL STABILIZATION?
    // Each button is put in series with a single unique resistance
    // (Since there are 24 frets on this neckboard, resistances go from 0 to 2^6 
    // every two strings: can't use 24 different resistors since the ADC has a
    // only 10 bit resolution. Can't use even just 12 because signal isn't stable enough
    // to always be quantized in the same interval..)
    // Each analog reading will return a different value, which will tell which note to play
    // When multiple frets are pressed only the higher will generate sound
    // (each combo is identified by a unique sum of powers of 2 and the string groups)
    // (Available note range: A4 (440 hz) - GS6 (1661 hz))
    
    // Since Arduino Uno R3 ADC has a resolution of 10 bits, readings are mapped from 0 to 1023..
    // Following chromatic scale on fretboard, each resistance double the previous one.
    // This will proportionally reflect in digitalization process..
    // EXACTLY?
    resistances[i] = map(resistances[i], 0, 1023, 0, 127);
    // Strings are checked from lower to higher (and so their frets..) and 
    // the highest actually pressed key is redefined when higher keys are found pressed...
    switch (i) {
      case 0:
        Serial.println("From lower strings: " + String(resistances[0]));
        // (Since each adc pin is in series with a resistor connected to ground 
        // (to prevent random floatings when no input is given),
        // part of the resolution is already consumed by its reading and the maximum readable value
        // may not be 1023, depending on the resistor value...)
        /*
        Currently displaced resistors
        A0: 0, 220, 1k, 10k (?)
        AIs ground: 1k(?)
        Current partitioner: 1k (?)
        */
        switch (resistances[0]) {
          // The lower the key, the lower the resistance, the higher the voltage read...
          case 127: // 0
            highestPressedKey = A4;
            break;
          case 126: // 1
            highestPressedKey = AS4; // Highest pressed key is updated only if higher keys are actually pressed..
            break;
          case 125: // 2
          case 124: // 3
            // When multiple keys are pressed, their resistances are summed...
            // As said, only the higher note is played..
            highestPressedKey = B4;
            break;
          case 123: // 4
          case 122: // 5
          case 121: // 6
          case 120: // 7
            highestPressedKey = C5;
            break;
          case 119: // 8
          case 118:
          case 117:
          case 116:
          case 115:
          case 114:
          case 113:
          case 112:
            highestPressedKey = CS5;
            break;
          case 111: // 16
          case 110:
          case 109:
          case 108:
          case 107:
          case 106:
          case 105:
          case 104:
          case 103:
          case 102:
          case 101:
          case 100:
          case 99:
          case 98:
          case 97:
          case 96:
            highestPressedKey = D5;
            break;
          case 95: // 32
          case 94:
          case 93:
          case 92:
          case 91:
          case 90:
          case 89:
          case 88:
          case 87:
          case 86:
          case 85:
          case 84:
          case 83:
          case 82:
          case 81:
          case 80:
          case 79:
          case 78:
          case 77:
          case 76:
          case 75:
          case 74:
          case 73:
          case 72:
          case 71:
          case 70:
          case 69:
          case 68:
          case 67:
          case 66:
          case 65:
          case 64:
            highestPressedKey = DS5;
            break;
          case 63: // 64
          case 62:
          case 61:
          case 60:
          case 59:
          case 58:
          case 57:
          case 56:
          case 55:
          case 54:
          case 53:
          case 52:
          case 51:
          case 50:
          case 49:
          case 48:
          case 47:
          case 46:
          case 45:
          case 44:
          case 43:
          case 42:
          case 41:
          case 40:
          case 39:
          case 38:
          case 37:
          case 36:
          case 35:
          case 34:
          case 33:
          case 32:
          case 31:
          case 30:
          case 29:
          case 28:
          case 27:
          case 26:
          case 25:
          case 24:
          case 23:
          case 22:
          case 21:
          case 20:
          case 19:
          case 18:
          case 17:
          case 16:
          case 15:
          case 14:
          case 13:
          case 12:
          case 11:
          case 10:
          case 9:
          case 8:
          case 7:
          case 6:
          case 5:
          case 4:
          case 3:
          case 2:
          case 1:
            highestPressedKey = E5;
            break;
        }
        break;
      case 1:
        Serial.println("From middle strings: " + String(resistances[1]));
        switch (resistances[1]) {
          case 127:
            highestPressedKey = F5;
            break;
          case 126:
            highestPressedKey = FS5;
            break;
          case 125:
          case 124:
            highestPressedKey = G5;
            break;
          case 123:
          case 122:
          case 121:
          case 120:
            highestPressedKey = GS5;
            break;
          case 119:
          case 118:
          case 117:
          case 116:
          case 115:
          case 114:
          case 113:
          case 112:
            highestPressedKey = A5;
            break;
          case 111:
          case 110:
          case 109:
          case 108:
          case 107:
          case 106:
          case 105:
          case 104:
          case 103:
          case 102:
          case 101:
          case 100:
          case 99:
          case 98:
          case 97:
          case 96:
            highestPressedKey = AS5;
            break;
          case 95:
          case 94:
          case 93:
          case 92:
          case 91:
          case 90:
          case 89:
          case 88:
          case 87:
          case 86:
          case 85:
          case 84:
          case 83:
          case 82:
          case 81:
          case 80:
          case 79:
          case 78:
          case 77:
          case 76:
          case 75:
          case 74:
          case 73:
          case 72:
          case 71:
          case 70:
          case 69:
          case 68:
          case 67:
          case 66:
          case 65:
          case 64:
            highestPressedKey = B5;
            break;
          case 63:
          case 62:
          case 61:
          case 60:
          case 59:
          case 58:
          case 57:
          case 56:
          case 55:
          case 54:
          case 53:
          case 52:
          case 51:
          case 50:
          case 49:
          case 48:
          case 47:
          case 46:
          case 45:
          case 44:
          case 43:
          case 42:
          case 41:
          case 40:
          case 39:
          case 38:
          case 37:
          case 36:
          case 35:
          case 34:
          case 33:
          case 32:
          case 31:
          case 30:
          case 29:
          case 28:
          case 27:
          case 26:
          case 25:
          case 24:
          case 23:
          case 22:
          case 21:
          case 20:
          case 19:
          case 18:
          case 17:
          case 16:
          case 15:
          case 14:
          case 13:
          case 12:
          case 11:
          case 10:
          case 9:
          case 8:
          case 7:
          case 6:
          case 5:
          case 4:
          case 3:
          case 2:
          case 1:
            highestPressedKey = C6;
            break;
        }
        break;
      case 2:
        Serial.println("From higher strings: " + String(resistances[2]));
        switch (resistances[2]) {
          case 127:
            highestPressedKey = CS6;
            break;
          case 126:
            highestPressedKey = D6;
            break;
          case 125:
          case 124:
            highestPressedKey = DS6;
            break;
          case 123:
          case 122:
          case 121:
          case 120:
            highestPressedKey = E6;
            break;
          case 119:
          case 118:
          case 117:
          case 116:
          case 115:
          case 114:
          case 113:
          case 112:
            highestPressedKey = F6;
            break;
          case 111:
          case 110:
          case 109:
          case 108:
          case 107:
          case 106:
          case 105:
          case 104:
          case 103:
          case 102:
          case 101:
          case 100:
          case 99:
          case 98:
          case 97:
          case 96:
            highestPressedKey = FS6;
            break;
          case 95:
          case 94:
          case 93:
          case 92:
          case 91:
          case 90:
          case 89:
          case 88:
          case 87:
          case 86:
          case 85:
          case 84:
          case 83:
          case 82:
          case 81:
          case 80:
          case 79:
          case 78:
          case 77:
          case 76:
          case 75:
          case 74:
          case 73:
          case 72:
          case 71:
          case 70:
          case 69:
          case 68:
          case 67:
          case 66:
          case 65:
          case 64:
            highestPressedKey = G6;
            break;
          case 63:
          case 62:
          case 61:
          case 60:
          case 59:
          case 58:
          case 57:
          case 56:
          case 55:
          case 54:
          case 53:
          case 52:
          case 51:
          case 50:
          case 49:
          case 48:
          case 47:
          case 46:
          case 45:
          case 44:
          case 43:
          case 42:
          case 41:
          case 40:
          case 39:
          case 38:
          case 37:
          case 36:
          case 35:
          case 34:
          case 33:
          case 32:
          case 31:
          case 30:
          case 29:
          case 28:
          case 27:
          case 26:
          case 25:
          case 24:
          case 23:
          case 22:
          case 21:
          case 20:
          case 19:
          case 18:
          case 17:
          case 16:
          case 15:
          case 14:
          case 13:
          case 12:
          case 11:
          case 10:
          case 9:
          case 8:
          case 7:
          case 6:
          case 5:
          case 4:
          case 3:
          case 2:
          case 1:
            highestPressedKey = GS6;
            break;
        }
        break;
    }
  }
  // As said before, analog readings may oscillate due to inconsistent signal..
  if ((resistances[0] == 0) && (resistances[1] == 0) && (resistances[2] == 0))
    // No key pressed...
    // If noTone() isn't called, tone() will eventually continue to work undisturbed
    noTone(3);
  else tone(3, highestPressedKey);
}