unsigned long lastTimeHigh = millis();
unsigned long currentTime;
String blink = "Blink at ";
String blinkState = "Speed: ";
// Blink frequency selection state
// It goes from 1 to 5
uint8_t speed = 1;
// Blink interval
unsigned int blinkInterval = 2000;
// Last serial monitor message memorization
// (Use 'U' or 'D' to increase o decrease LED blink frequency)
char lastSerialMessage;
// Increase freq pin status
int increaseReading;
// Decrease freq pin status
int decreaseReading;
// Memorize last buttons status: pressure, release or debounce
int lastIncreaseButtonState = LOW; // LOW or HIGH as reading
int lastDecreaseButtonState = LOW; // LOW or HIGH as reading
// Memorize definitive button pressures or releases
int increaseButtonState = LOW;
int decreaseButtonState = LOW;
// Memorize each button pressure or debounce time
unsigned long lastIncreaseDebounceTime = 0; // ms
unsigned long lastDecreaseDebounceTime = 0;
// Button debounce lapse
unsigned long debounceDelay = 50;
// Memorize time when actually buttons are pressed
unsigned long lastIncreaseButtonPressureTime;
unsigned long lastDecreaseButtonPressureTime;
// Speed in/decrease delay over buttons pressure
// They will change after buttons pressure
unsigned int increaseButtonPressureDelay;
unsigned int decreaseButtonPressureDelay;
// Reset built-in func
void(* resetFunc) (void) = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // 13
  // Digital input pins for buttons pressure sense
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  Serial.begin(9600);
  Serial.println(blinkState + speed);
}

void loop() {
  currentTime = millis();
  // Check new messages from serial monitor
  lastSerialMessage = Serial.read();
  if (lastSerialMessage == 'u') { // Stands for 'up'
    // Eventually increase blink freq
    if (speed < 5) {
      speed++;
      blinkInterval /= 2;
      Serial.println(blinkState + speed);
    }
    else Serial.println("Blink frequency already highest: " + String(speed));
  }
  if (lastSerialMessage == 'd') { // Stands for 'down'
    // Eventually decrease blink freq
    if (speed > 1) {
      speed --;
      blinkInterval *= 2;
      Serial.println(blinkState + speed);
    }
    else Serial.println("Blink frequency already lowest: " + String(speed));
  }
  /* Check increase/decrease button pressure
  to eventually increase/decrease blink frequency */
  increaseReading = digitalRead(12);
  if (increaseReading != lastIncreaseButtonState) lastIncreaseDebounceTime = currentTime;  // Button pressure, release or debounce!
  if ((currentTime - lastIncreaseDebounceTime) > debounceDelay) { // Button stabilized 
    if (increaseReading != increaseButtonState) {
      increaseButtonState = increaseReading; // Button pressure or release!
      // Add some delay over pressure to avoid to over-in/decrease speed (program loop is faster than human finger button pressure)
      // This will let user sensibly check how speed value change during pressure time
      // A single speed level in/decrement requires only 30ms pressure to be applied
      increaseButtonPressureDelay = 30;
      // So, first, start counting time since pressure
      if (increaseButtonState == HIGH) lastIncreaseButtonPressureTime = currentTime;
    } 
    // Increase speed when pressure delay is exceeded and keep increase delay over delay if button keep being pressed
    if (increaseButtonState == HIGH && (currentTime - lastIncreaseButtonPressureTime) > increaseButtonPressureDelay) {
      if (speed < 5) {
        speed++;
        blinkInterval /= 2;
        Serial.println(blinkState + speed);
      }
      else Serial.println("Blink frequency already highest: " + String(speed));
      // If button keep being pushed it will require 1s for next speed level increment
      increaseButtonPressureDelay = 1000; 
      lastIncreaseButtonPressureTime = currentTime;
    }
  }
  lastIncreaseButtonState = increaseReading;

  decreaseReading = digitalRead(11);
  if (decreaseReading != lastDecreaseButtonState) lastDecreaseDebounceTime = currentTime;  // Button pressure, release or debounce!
  if ((currentTime - lastDecreaseDebounceTime) > debounceDelay) { // Button stabilized 
    if (decreaseReading != decreaseButtonState) {
      decreaseButtonState = decreaseReading; // Button pressure or release!
      decreaseButtonPressureDelay = 30;
      if (decreaseButtonState == HIGH) lastDecreaseButtonPressureTime = currentTime;
    }
    // // As for speed increasing mechanism, decrease speed when pressure delay is exceeded and keep decrease delay over delay if button keep being pressed
    if (decreaseButtonState == HIGH && (currentTime - lastDecreaseButtonPressureTime) > decreaseButtonPressureDelay) {
      if (speed > 1) {
        speed--;
        blinkInterval *= 2;
        Serial.println(blinkState + speed);
      }
      else Serial.println("Blink frequency already lowest: " + String(speed));
      // If button keep being pushed it will require 1s for next speed level decrement
      decreaseButtonPressureDelay = 1000;
      lastDecreaseButtonPressureTime = currentTime;
    }
  }
  lastDecreaseButtonState = decreaseReading;
  // LED on
  if (currentTime - lastTimeHigh > blinkInterval) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(blink + currentTime + "ms");
    lastTimeHigh = currentTime;
  }
  // LED off
  if(currentTime - lastTimeHigh > blinkInterval / 2) digitalWrite(LED_BUILTIN, LOW);
  // Reset instruction after 2 minutes
  if (currentTime > 120000) {
    Serial.println("Resetting board...");
    delay(500); // resetFunc()'s faster than println() display phase...
    resetFunc();
  }
}
