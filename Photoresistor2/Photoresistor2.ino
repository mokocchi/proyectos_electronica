const int sensorPin = 0;
const int ledPin = 9;

int lightLevel;
int calibratedlightLevel;
int maxThreshold = 0;
int minThreshold = 1023;


void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.print("Leyendo...");
}

String myword = "";

int catch_flash() {
  if (calibratedlightLevel > 165) {
    analogWrite(ledPin, calibratedlightLevel);
    int start = millis();
    int length = 0;
    while (true) {
      lightLevel = analogRead(sensorPin);
      calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
      if (calibratedlightLevel < 165) {
        length = millis() - start;
        break;
      }
    }
    if ((length > 0) && (length < 350)) {
      myword = myword + ".";
    } else {
      myword = myword + "-";
    }
    Serial.print(".");
  } else {
    analogWrite(ledPin, 0);
  }
}

void loop()
{
  catch_flash();
  int start = millis();
  do {
    lightLevel = analogRead(sensorPin);
    calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
  } while (calibratedlightLevel < 165);
  int lenght = millis() - start;
  if ((lenght > 600) && (lenght < 1500)) {
    Serial.println("[" + myword + "]");
  }
  if (lenght > (1500)) {
    Serial.println();
    if (myword != "") {
      Serial.println("La palabra es: [" + myword + "]");
    }
    Serial.print("Leyendo...");
    myword = "";
  }
}
