// Note frequencies from public local library
#include "pitches.h"

// Pins status...
// from strings..
uint8_t EReading;
byte AReading;
byte DReading;
byte GReading;
byte BReading;
byte eReading;
// and frets (starting from A from lower E string)
byte fifthReading;
byte sixthReading;
byte seventhReading;
byte eighthReading;
// Memorize last buttons status (pressure, release or bounce)
byte lastEButtonState = LOW;
byte lastAButtonState = LOW;
byte lastDButtonState = LOW;
byte lastGButtonState = LOW;
byte lastBButtonState = LOW;
byte lasteButtonState = LOW;
byte lastFifthButtonState = LOW;
byte lastSixthButtonState = LOW;
byte lastSeventhButtonState = LOW;
byte lastEighthButtonState = LOW;
// Memorize definitive buttons pressures or releases
byte EButtonState = LOW;
byte AButtonState = LOW;
byte DButtonState = LOW;
byte GButtonState = LOW;
byte BButtonState = LOW;
byte eButtonState = LOW;
byte fifthButtonState = LOW;
byte sixthButtonState = LOW;
byte seventhButtonState = LOW;
byte eighthButtonState = LOW;
// Memorize each button each pressure or bounce time
unsigned long lastEBounceTime = 0;
unsigned long lastABounceTime = 0;
unsigned long lastDBounceTime = 0;
unsigned long lastGBounceTime = 0;
unsigned long lastBBounceTime = 0;
unsigned long lasteBounceTime = 0;
unsigned long lastFifthBounceTime = 0;
unsigned long lastSixthBounceTime = 0;
unsigned long lastSeventhBounceTime = 0;
unsigned long lastEighthBounceTime = 0;
// Debounce lapse
unsigned long debounceDelay = 50;
// Timestamp
unsigned long lastTime;
// Since multiple buttons can be simultaneously pressed,
// every program cycle must be state which one is the highest-pitched
unsigned int highestPressedKey = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting playing..");
  // Buttons grid
  // Strings (from lower to higher)
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  // Frets (from lower to higher)
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  // Buzzer
  pinMode(10, OUTPUT);
}

void loop() {
  // Read current time
  lastTime = millis();
  // Check each button pin status variation to say which buttons
  // are actually pressed at each moment...
  // E string
  EReading = digitalRead(0);
  // No need to wait debounce lapse to start making noise
  // (we aren't affecting dangerous actuators pushing these buttons..)
  if (EReading == HIGH) EButtonState = HIGH;
  // But we must wait debounce lapse to say if button is definitely pressed or released..
  if (EReading != lastEButtonState) lastEBounceTime = lastTime; // Button pressure, release or debounce!
  if ((lastTime - lastEBounceTime) > debounceDelay) // Button definitely pressed or released!
  	EButtonState = EReading;
  lastEButtonState = EReading;
  // A string
  AReading = digitalRead(1);
  if (AReading == HIGH) AButtonState = HIGH;
  if (AReading != lastAButtonState) lastABounceTime = lastTime;
  if ((lastTime - lastABounceTime) > debounceDelay)
  	AButtonState = AReading;
  lastAButtonState = AReading;
  // D string
  DReading = digitalRead(2);
  if (DReading == HIGH) DButtonState = HIGH;
  if (DReading != lastDButtonState) lastDBounceTime = lastTime;
  if ((lastTime - lastDBounceTime) > debounceDelay)
  	DButtonState = DReading;
  lastDButtonState = DReading;
  // G string
  GReading = digitalRead(3);
  if (GReading == HIGH) GButtonState = HIGH;
  if (GReading != lastGButtonState) lastGBounceTime = lastTime;
  if ((lastTime - lastGBounceTime) > debounceDelay)
  	GButtonState = GReading;
  lastGButtonState = GReading;
  // B string
  BReading = digitalRead(4);
  if (BReading == HIGH) BButtonState = HIGH;
  if (BReading != lastBButtonState) lastBBounceTime = lastTime;
  if ((lastTime - lastBBounceTime) > debounceDelay)
  	BButtonState = BReading;
  lastBButtonState = BReading;
  // e string
  eReading = digitalRead(5);
  if (eReading == HIGH) eButtonState = HIGH;
  if (eReading != lasteButtonState) lasteBounceTime = lastTime;
  if ((lastTime - lasteBounceTime) > debounceDelay)
  	eButtonState = eReading;
  lasteButtonState = eReading;
  // Fifth fret
  fifthReading = digitalRead(6);
  if (fifthReading == HIGH) fifthButtonState = HIGH;
  if (fifthReading != lastFifthButtonState) lastFifthBounceTime = lastTime;
  if ((lastTime - lastFifthBounceTime) > debounceDelay)
  	fifthButtonState = fifthReading;
  lastFifthButtonState = fifthReading;
  // Sixth fret
  sixthReading = digitalRead(7);
  if (sixthReading == HIGH) sixthButtonState = HIGH;
  if (sixthReading != lastSixthButtonState) lastSixthBounceTime = lastTime;
  if ((lastTime - lastSixthBounceTime) > debounceDelay)
  	sixthButtonState = sixthReading;
  lastSixthButtonState = sixthReading;
  // Seventh fret
  seventhReading = digitalRead(8);
  if (seventhReading == HIGH) seventhButtonState = HIGH;
  if (seventhReading != lastSeventhButtonState) lastSeventhBounceTime = lastTime;
  if ((lastTime - lastSeventhBounceTime) > debounceDelay)
  	seventhButtonState = seventhReading;
  lastSeventhButtonState = seventhReading;
  // Eighth fret
  eighthReading = digitalRead(9);
  if (eighthReading == HIGH) eighthButtonState = HIGH;
  if (eighthReading != lastEighthButtonState) lastEighthBounceTime = lastTime;
  if ((lastTime - lastEighthBounceTime) > debounceDelay)
  	eighthButtonState = eighthReading;
  lastEighthButtonState = eighthReading;
  
  // Checking each string and fret from lower to higher let us update
  // the highest key pressed when multiple keys are simultaneously pressed
  // (and oc when a single higher key is pressed too)
  if (EButtonState == HIGH && fifthButtonState == HIGH) // check if brackets necessary
  	highestPressedKey = 440;
  if (EButtonState == HIGH && sixthButtonState == HIGH)
    highestPressedKey = 466;
  if (EButtonState == HIGH && seventhButtonState == HIGH)
    highestPressedKey = 494;
  if (EButtonState == HIGH && eighthButtonState == HIGH)
    highestPressedKey = 523;
  if (AButtonState == HIGH && fifthButtonState == HIGH)
    highestPressedKey = 554;
  if (AButtonState == HIGH && sixthButtonState == HIGH)
    highestPressedKey = 587;
  if (AButtonState == HIGH && seventhButtonState == HIGH)
    highestPressedKey = 622;
  if (AButtonState == HIGH && eighthButtonState == HIGH)
    highestPressedKey = 659;
  if (DButtonState == HIGH && fifthButtonState == HIGH)
    highestPressedKey = 698;
  if (DButtonState == HIGH && sixthButtonState == HIGH)
    highestPressedKey = 740;
  if (DButtonState == HIGH && seventhButtonState == HIGH)
    highestPressedKey = 784;
  if (DButtonState == HIGH && eighthButtonState == HIGH)
    highestPressedKey = 831;
  if (GButtonState == HIGH && fifthButtonState == HIGH)
    highestPressedKey = 880;
  if (GButtonState == HIGH && sixthButtonState == HIGH)
    highestPressedKey = 932;
  if (GButtonState == HIGH && seventhButtonState == HIGH)
    highestPressedKey = 988;
  if (GButtonState == HIGH && eighthButtonState == HIGH)
    highestPressedKey = 1047;
  if (BButtonState == HIGH && fifthButtonState == HIGH)
    highestPressedKey = 1109;
  if (BButtonState == HIGH && sixthButtonState == HIGH)
    highestPressedKey = 1175;
  if (BButtonState == HIGH && seventhButtonState == HIGH)
    highestPressedKey = 1245;
  if (BButtonState == HIGH && eighthButtonState == HIGH)
    highestPressedKey = 1319;
  if (eButtonState == HIGH && fifthButtonState == HIGH)
    highestPressedKey = 1397;
  if (eButtonState == HIGH && sixthButtonState == HIGH)
    highestPressedKey = 1480;
  if (eButtonState == HIGH && seventhButtonState == HIGH)
    highestPressedKey = 1568;
  if (eButtonState == HIGH && eighthButtonState == HIGH)
    highestPressedKey = 1661;
          
  // Stop sound emission when no key pressed..
  if ((EButtonState == LOW) && 
      (AButtonState == LOW) && 
      (DButtonState == LOW) && 
      (GButtonState == LOW) && 
      (BButtonState == LOW) && 
      (eButtonState == LOW) &&
      (fifthButtonState == LOW) &&
      (sixthButtonState == LOW) &&
      (seventhButtonState == LOW) &&
      (eighthButtonState == LOW))
    // If noTone() isn't called, tone() will eventually continue to work undisturbed
    noTone(10);
  else tone(10, highestPressedKey);
}