// Note freqs
#define c3 130.8
#define e 164.8
#define f 174.6
#define g 196
#define a 220

void setup() {
  // put your setup code here, to run once:
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(13) == HIGH) {
    delay(50);
    tone(2, (unsigned int)(c3 + 0.5f));
  }
  if (digitalRead(12) == HIGH) {
    delay(50);
    tone(2, (unsigned int)(e + 0.5f));
  }
  if (digitalRead(6) == HIGH) {
    delay(50);
    tone(2, (unsigned int)(f + 0.5f));
  }
  if (digitalRead(5) == HIGH) {
    delay(50);
    tone(2, (unsigned int)(g + 0.5f));
  }
  if (digitalRead(4) == HIGH) {
    delay(50);
    tone(2, (unsigned int)(a + 0.5f));
  }
  delay(10);
  noTone(2);
}
