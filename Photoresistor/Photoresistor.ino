const int pin_sensor = A0;
const int largo_punto = 300;
const int tiempo_maximo =  1000;
const int largo_silencio = 750;
const String punto = ".";
const String raya = "-";
String palabra = "";
int luz_ambiente;

void fijar_luz_ambiente(){
  luz_ambiente = analogRead(pin_sensor);
}

void esperar_luz(bool con_tiempo) {
  int luz_sensor;
  int inicio = millis();
  do {
    luz_sensor = analogRead(pin_sensor);
    if(con_tiempo && (millis() > inicio + tiempo_maximo)) {
      return;
    }
  } while (luz_sensor < luz_ambiente + 10);
}

void esperar_oscuridad() {
  int luz_sensor;
  do {
    luz_sensor = analogRead(pin_sensor);
  } while (luz_sensor > luz_ambiente);
}

void setup() {
  Serial.begin(9600);
  pinMode(pin_sensor, INPUT);
  fijar_luz_ambiente();
  Serial.print("Luz ambiente: ");
  Serial.println(luz_ambiente);
}

void loop() {
  esperar_luz(false);  
  const int inicio_luz = millis();
  esperar_oscuridad();
  int duracion_luz = millis() - inicio_luz;
  if(duracion_luz > largo_punto) {
    palabra = palabra + raya;
  } else {
    palabra = palabra + punto;
  }
  Serial.print(".");
  const int inicio_oscuridad = millis();
  esperar_luz(true);
  int duracion_oscuridad = millis() - inicio_oscuridad;
  if(duracion_oscuridad >= largo_silencio) {
    Serial.println();
    Serial.print("La palabra es:");
    Serial.print("[");
    Serial.print(palabra);
    Serial.println("]");
    palabra = "";    
  }
}