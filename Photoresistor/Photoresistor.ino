const int pin_sensor = A0;

int umbral_minimo = 0;
int umbral_maximo = 1023;
int luz_ambiente = 0;
int duracion_parpadeo = 100;
int tolerancia_parpadeo = 150;
String punto = ".";
String raya = "-";
String palabra = "";

void fijar_luz_ambiente() {
  int luz_sensor = analogRead(pin_sensor);
  luz_ambiente = map(luz_sensor, 0, 1023, 0, 255);
}

bool esperar_luz(int inicio_oscuridad, boolean por_siempre) {
  int luz_calibrada = 0;
  do {
    int luz_sensor = analogRead(pin_sensor);
    luz_calibrada = map(luz_sensor, 0, 1023, 0, 255);
  } while ((luz_calibrada <= luz_ambiente + 10) && (por_siempre || (millis() - inicio_oscuridad < duracion_parpadeo * 7)));
  return luz_calibrada <= luz_ambiente + 10; //se acabo el tiempo
}

void esperar_oscuridad() {
  int luz_calibrada = 0;
  do {
    int luz_sensor = analogRead(pin_sensor);
    luz_calibrada = map(luz_sensor, 0, 1023, 0, 255);    
  } while (luz_calibrada > luz_ambiente);
}

void leer_luz() {
  // Serial.println("LUZ");
  int duracion_luz = 0;
  int luz_calibrada = 0;
  int inicio_luz = millis();
  do {
    int luz_sensor = analogRead(pin_sensor);
    luz_calibrada = map(luz_sensor, 0, 1023, 0, 255);
  } while (luz_calibrada > luz_ambiente);
  duracion_luz = millis() - inicio_luz;
  if (duracion_luz < duracion_parpadeo + tolerancia_parpadeo) {
    palabra = palabra + punto;
  } else {
    palabra = palabra + raya;
  }
  Serial.print(".");
}

void setup() {
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
