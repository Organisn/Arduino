// Liquid crystal display control built-in library import
#include <LiquidCrystal.h>
// Initialize library
// No need to set mode for display interface pins
LiquidCrystal lcd(7, 8, 5, 4, 3, 2); // Interface pins: RS, E, D4, D5, D6, D7)
// Motor speed from powerometer
// Analog signal goes from 0 to 5 volts mapped on 1024 bits (= 5/1024 ≃ 0,005 volts per bit)
// Must proportionally reduce before write (PWM supports values from 0 to 255 [=1023/4])
int speed = analogRead(5) / 4;
// Memorize motor driver chip enabling state
// By default motor is stopped
bool driverEnabled = 0;
// Memorize motor rotation state (clockwise/anti-clockwise)
bool rotationState = 0;
// Buttons pressure relevation variables
// Rotation pin status
int rotationReading;
// Enabling pin status
int enablingReading;
// Memorize last buttons status: pressure, release or debounce
int lastRotationButtonState = LOW; // LOW or HIGH as reading
int lastEnablingButtonState = LOW; // LOW or HIGH as reading
// Memorize definitive buttons pressures or releases
int rotationButtonState = LOW;
int enablingButtonState = LOW;
// Last time reading
unsigned long lastTime;
// Memorize each button pressure or debounce time
unsigned long lastRotationDebounceTime = 0; // ms
unsigned long lastEnablingDebounceTime = 0;
// Debounce lapse
unsigned long debounceDelay = 50;
// Speed display variables
unsigned long lastSpeedDisplay = 0;
unsigned long speedDisplayDelay = 250; // ms
// Motor state display messages
// PC serial monitor
String enablingUpdate = "Driver ";
String rotationUpdate = "Spin sense: ";
String speedUpdate = "Speed: ";
// LCD
String lcdRotationUpdate = "Sense:";
String lcdSpeedUpdate = "   v:";
// Reset built-in func
void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(enablingUpdate + "disabled");
  Serial.println(rotationUpdate + 0);
  Serial.println(speedUpdate + speed);
  // Does not need to set A0 as input pin 'cause that's analog pins default mode
  // Driver enabling button pin
  pinMode(13, INPUT);
  // Motor spin sense button pin
  pinMode(12, INPUT);
  // Motor power giver pin for the first sense
  pinMode(11, OUTPUT);
  // Motor power giver pin for the second sense
  pinMode(10, OUTPUT);
  // Driver enabling pin
  pinMode(9, OUTPUT);
  // LCD display constrast set pin (only because I don't have a second potentiometer, which is better. 
  // Another way to define contrast is by directly plugging lcd to board 5V voltage supply)
  pinMode(6, OUTPUT);
  lcd.begin(16, 2); // 16 columns, 2 rows of char slots
  // (LCD is called 1602 exactly because of columns and rows quantity)
  // Set LCD constrast with PWM (instead of analog signal..)
  // Got just a single poweromoter in the kit...
  analogWrite(6, 0);
  // Cursor goes from top-left to bottom-right
  lcd.setCursor(0, 0); // Column on the left, upper row
  // Display on lcd chip, spin and speed status labels and initial values
  // Until cleared or overwritten, chars will remain memorized and displayed by lcd
  // So labels will never be reprinted..
  lcd.print(enablingUpdate + "disabled");
  lcd.setCursor(0, 1); // Column on the left, bottom row
  lcd.print(lcdRotationUpdate + rotationState + lcdSpeedUpdate + "  " + String(speed)); // speed must be converted to String when equal to 0
  // otherwise it won't be printed on lcd
}

void loop() {
  analogWrite(6, 0);
  // Move motor
  activateDCM(driverEnabled, rotationState, speed);
  // Read current time
  lastTime = millis();
  // Reset board after 2 minutes
  if (lastTime > 120000) {
    Serial.println("Resetting board...");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset!");
    delay(1000); // resetFunc()'s faster than println() display phase...
    resetFunc();
  }
  // Motor spin sense changes on right button pressure
  rotationReading = digitalRead(12);
  if (rotationReading != lastRotationButtonState) lastRotationDebounceTime = lastTime;  // Button pressure, release or debounce!
  if ((lastTime - lastRotationDebounceTime) > debounceDelay && rotationReading != rotationButtonState) { // Button pressure or release!
    rotationButtonState = rotationReading;
    if (rotationButtonState == HIGH){
      rotationState = !rotationState; // Invert motor spin sense after button pressure
      Serial.println(rotationUpdate + rotationState);
      // Update rotation status value on lcd
      lcd.setCursor(6, 1);
      lcd.print(rotationState);
    }
  }
  lastRotationButtonState = rotationReading;
  // Motor ceases/start to spin over left button pressure
  enablingReading = digitalRead(13);
  if (enablingReading != lastEnablingButtonState) lastEnablingDebounceTime = lastTime;  // Button pressure, release or debounce!
  if ((lastTime - lastEnablingDebounceTime) > debounceDelay && enablingReading != enablingButtonState){ // Button pressure or release!
    enablingButtonState = enablingReading;
    if (enablingButtonState == HIGH){
      driverEnabled = !driverEnabled; // Dis/enable motor driver after button pressure
      Serial.println(enablingUpdate + (driverEnabled ? "enabled" : "disabled"));
      lcd.setCursor(7, 0);
      // Update only those lcd slots which change.. 
      lcd.print(driverEnabled ? " en" : "dis");
    }
  }
  lastEnablingButtonState = enablingReading;
  // Update motor speed
  speed = analogRead(5) / 4;
  // Display speed status each quarter of second
  if ((lastTime - lastSpeedDisplay) > speedDisplayDelay){
    Serial.println(speedUpdate + speed);
    lcd.setCursor(12, 1);
    // Must clear slots for tens and/or hundreds units 
    // when having to show speed values made of just units or tens units 
    // When speed value goes over 9 or 99 and then goes back down,
    // Lcd slots used for tens and hundreds units remain occupied
    // Must clear 'em (overwriting with black spaces...)
    if (speed < 10) lcd.print("  " + String(speed));
    else lcd.print(speed < 100 ? " " + String(speed) : String(speed));
    lastSpeedDisplay = lastTime;
  }
}

// Gives power to motor and set its rotation sense
void activateDCM(bool driverEnabled, bool rotationState, int speed) {
  if (driverEnabled){
    digitalWrite(9, HIGH);
    // Give power only if driver's enabled (otherwise is just wasted current)
    if (rotationState){
        // Switch off one activator pin
        analogWrite(11, 0);
        // On the other
        analogWrite(10, speed);
    }
    else {
        analogWrite(10, 0);
        analogWrite(11, speed);
    }
  }
  else digitalWrite(9, LOW); // This will prevent the motor to start even under power supply
}
