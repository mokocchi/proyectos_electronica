int button = D1;
int ledpin = D2;
int buttonState = 0;
void setup() {  
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  pinMode(button, INPUT);
}
void loop() {

  buttonState = digitalRead(button);
  Serial.println(buttonState);
  if (buttonState == 1)
  {
    digitalWrite(ledpin, HIGH);
    delay(200);
  }
  if (buttonState == 0)
  {
    digitalWrite(ledpin, LOW);
    delay(200);
  }
}
