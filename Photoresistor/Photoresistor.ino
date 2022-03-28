const int pin_sensor = A0;
int luz_ambiente = 0;
int duracion_parpadeo = 100;
int tolerancia_parpadeo = 150;
int largo_silencio = 7;
String punto = ".";
String raya = "-";
String palabra = "";

void fijar_luz_ambiente() {
  int max_luz = 0;
  for (int i = 0; i < 100; i++) {
    int luz_sensor = analogRead(pin_sensor);
    if (luz_sensor > max_luz) {
      max_luz = luz_sensor;
    }
  }
  luz_ambiente = max_luz;
}

bool esperar_luz(int inicio_oscuridad, bool por_siempre) {
  int luz_sensor = 0;
  do {
    luz_sensor = analogRead(pin_sensor);    
    delay(10);
  } while ((luz_sensor <= luz_ambiente) && (por_siempre || (millis() - inicio_oscuridad < duracion_parpadeo * largo_silencio)));
  return luz_sensor <= luz_ambiente;  //se acabo el tiempo
}

void esperar_oscuridad() {
  int luz_sensor = 0;
  do {
    luz_sensor = analogRead(pin_sensor);
    delay(10);
  } while (luz_sensor > luz_ambiente);
}

void leer_luz() {
  int duracion_luz = 0;
  int luz_sensor = 0;
  int inicio_luz = millis();
  esperar_oscuridad();
  duracion_luz = millis() - inicio_luz;
  if (duracion_luz < duracion_parpadeo + tolerancia_parpadeo) {
    palabra = palabra + punto;
  } else {
    palabra = palabra + raya;
  }
  Serial.print(".");
}

void setup() {
  pinMode(pin_sensor, INPUT);
  Serial.begin(9600);
  delay(1000);
  fijar_luz_ambiente();
  char buffer[16];
  sprintf(buffer, "Luz ambiente: %d", luz_ambiente);
  Serial.println(buffer);
  Serial.print("Esperando...");
}

void loop() {
  esperar_luz(0, true);
  leer_luz();
  esperar_oscuridad();
  int inicio_oscuridad = millis();
  if (esperar_luz(inicio_oscuridad, false)) {
    Serial.println();
    Serial.println("La palabra es: [" + palabra + "]");
    Serial.print("Esperando...");
    palabra = "";
  }
}