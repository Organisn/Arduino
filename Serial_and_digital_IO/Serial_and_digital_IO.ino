/* To achieve right to write on ttyACM0
 sudo chmod a+rw /dev/ttyACM0*/
unsigned long lastTimeHigh = millis();
unsigned long currentTime;
String blink = "Blink at ";
String blinkState = "Speed: ";
// Blink frequency selection state
// It goes from 1 to 5
uint8_t speed = 1;
// Blink interval
unsigned int blinkInterval = 2000;

// Reset built-in func
void(* resetFunc) (void) = 0;
// put your setup code here, to run once
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Digital input pins for buttons pressure sense
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
  Serial.println(blinkState + speed);
}
// put your main code here, to run repeatedly
void loop() {
  /* Check increase button pressure
  or 'u' serial monitor input
  to eventually increase blink frequency */
  if (digitalRead(3) == HIGH || Serial.read() == 'u') {
    // Button stabilization delay
    delay(300);
    if (speed < 4) {
      speed++;
      blinkInterval /= 2;
      Serial.println(blinkState + speed);
    }
    else Serial.println("Blink frequency already highest: " + speed);
  }
  /* Check decrease button pressure
  or 'd' serial monitor input
  to eventually decrease blink frequency */
  if (digitalRead(2) == HIGH || Serial.read() == 'd') {
    // Button stabilization delay
    delay(300);
    if (speed > 1) {
      speed--;
      blinkInterval *= 2;
      Serial.println(blinkState + speed);
    }
    else Serial.println("Blink frequency already lowest: " + speed);
  }
  currentTime = millis();
  // LED on
  if (currentTime - lastTimeHigh > blinkInterval) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(blink + currentTime + "ms");
    lastTimeHigh = currentTime;
  }
  // LED off
  if(currentTime - lastTimeHigh > blinkInterval / 2) digitalWrite(LED_BUILTIN, LOW);
  // Reset instruction after 60s
  if (currentTime > 60000) {
    Serial.println("Resetting board...");
    delay(500); // resetFunc()'s faster than println() display phase...
    resetFunc();
  }
}
