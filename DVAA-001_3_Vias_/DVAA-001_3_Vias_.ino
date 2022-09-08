#include <SoftwareSerial.h>

SoftwareSerial BT(9, 8);//TX RX
#define btState 4
//led indicador
#define led A4
//pines relays puerta izquierda
#define PIB A3
#define PIA A2
#define sensorPi 2
//pines relays puerta derecha
#define PDB A1
#define PDA A0
#define sensorPd 3



void setup() {

  Serial.begin(9600);
  Serial.println("Serial iniciado");
  BT.begin(38400);
  BT.println("BT iniciado");

  //declaracion salida relays
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  /*
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);*/

  //declaracion de entradas
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  /*pinMode(A2, INPUT);*/

  // configuracion salidas
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, LOW);
  digitalWrite(A5, HIGH);

  /*corrales se numeran a,b,c,d en sentido contrario a las agujas del reloj
    comenzando por corral superior a la izquierda*/

  /*la puerta central el sensor se coloca a la derecha*/
}

void loop() {
  while (!digitalRead(btState)) {
    digitalWrite(led, HIGH);
    delay(3000);
    digitalWrite(led, LOW);
    delay(3000);
  }
  digitalWrite(led, HIGH);
  if (BT.available() || Serial.available()) {
    char dataSerial = Serial.read();
    char dataBT = BT.read();

    if (dataSerial == 'a' || dataBT == 'a') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralA();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (dataSerial == 'b' || dataBT == 'b') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralB();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (dataSerial == 'c' || dataBT == 'c') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralC();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (dataSerial == 'y' || dataBT == 'y') {

      Serial.println("Moviendo puerta derecha");
      BT.println("Moviendo puerta derecha");

      puertaDerecha();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (dataSerial == 'z' || dataBT == 'z') {

      Serial.println("Moviendo puerta izquierda");
      BT.println("Moviendo puerta izquierda");

      puertaIzquierda();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (dataSerial == 'm' || dataBT == 'm') {

      Serial.println("Modo automatico Activado");
      BT.println("Modo automatico Activado");

      modoAutomatico();

      Serial.println("Listo");
      BT.println("Listo");
    }
  }
}

void corralA() {

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(PDB, LOW);
    delay(200);
    digitalWrite(PDB, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(PIA, LOW);
    delay(200);
    digitalWrite(PIA, HIGH);
  }
}

void corralB() {

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(PDB, LOW);
    delay(200);
    digitalWrite(PDB, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPi) == 0) {
    digitalWrite(PIB, LOW);
    delay(200);
    digitalWrite(PIB, HIGH);
  }
}

void corralC() {

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(PIA, LOW);
    delay(200);
    digitalWrite(PIA, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPd) == 1) {
    digitalWrite(PDA, LOW);
    delay(200);
    digitalWrite(PDA, HIGH);
  }
}

void puertaDerecha() {

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(PIA, LOW);
    delay(200);
    digitalWrite(PIA, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPd) == 1) {
    digitalWrite(PDA, LOW);
    delay(200);
    digitalWrite(PDA, HIGH);
  } else {
    digitalWrite(PDB, LOW);
    delay(200);
    digitalWrite(PDB, HIGH);
  }
}

void puertaIzquierda() {

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(PDB, LOW);
    delay(200);
    digitalWrite(PDB, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPi) == 0) {
    digitalWrite(PIB, LOW);
    delay(200);
    digitalWrite(PIB, HIGH);
  } else {
    digitalWrite(PIA, LOW);
    delay(200);
    digitalWrite(PIA, HIGH);
  }
}

void modoAutomatico() {
  char salida = 1;
  while (salida != 's') {
    if (BT.available()) {
      salida = BT.read();
    }
    puertaIzquierda();
    BT.println("Moviendo puerta Izquierda");
     if (BT.available()) {
      salida = BT.read();
    }
    delay(5000);
     if (BT.available()) {
      salida = BT.read();
    }
    puertaDerecha();
    BT.println("Moviendo puerta Derecha");
     if (BT.available()) {
      salida = BT.read();
    }
    delay(5000);
     if (BT.available()) {
      salida = BT.read();
    }
  }
  BT.println("Saliendo de modo automatico");
}
