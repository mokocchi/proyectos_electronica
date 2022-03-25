const int sensorPin = 0;
const int ledPin = 9;

int lightLevel;
int calibratedlightLevel;
int maxThreshold = 0;
int minThreshold = 1023;
int envLight = 0;

void setEnvLight() {
  lightLevel = analogRead(sensorPin);
  envLight = map(lightLevel, 0, 1023, 0, 255);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  setEnvLight();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  char buffer[16];
  sprintf(buffer, "Env light: %d", envLight);
  Serial.println(buffer);
  Serial.print("Esperando...");
}

String myword = "";

int catch_flash() {
  int start = millis();
  int mylenght = 0;
  while (true) {
    lightLevel = analogRead(sensorPin);
    calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
    if (calibratedlightLevel <= envLight) {
      mylenght = millis() - start;
      break;
    }
  }
  Serial.println(mylenght);
  if ((mylenght > 0) && (mylenght < 350)) {
    Serial.println("PUNTO");
    myword = myword + ".";
  } else {
    Serial.println("RAYA");
    myword = myword + "-";
  }
  Serial.print(".");
}

void loop() {
  do {
    lightLevel = analogRead(sensorPin);
    calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
  } while (calibratedlightLevel <= envLight);
  analogWrite(LED_BUILTIN, calibratedlightLevel);
  do {
    catch_flash();
  } while (calibratedlightLevel > envLight);
  analogWrite(LED_BUILTIN, 0);
  Serial.write("Calculando silencio");
  int start = millis();
  do {
    lightLevel = analogRead(sensorPin);
    calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
    Serial.print(".");
  } while (calibratedlightLevel <= envLight);
  int mylenght2 = millis() - start;
  if (mylenght2 > 800) {
    Serial.println(mylenght2);
    Serial.println();
    Serial.println("Espacio largo");
    Serial.println("La palabra es: [" + myword + "]");
    Serial.print("Leyendo...");
    myword = "";
  } else {
    Serial.println("Espacio corto");
  }
}
