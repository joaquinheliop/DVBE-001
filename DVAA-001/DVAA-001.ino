#include <SoftwareSerial.h>

SoftwareSerial BT(9, 8);

//pines relays puerta central
#define pc1 2
#define pc2 3
#define sensorPc A0
//pines relays puerta izquierda
#define pi1 4
#define pi2 5
#define sensorPi A2
//pines relays puerta derecha
#define pd1 6
#define pd2 7
#define sensorPd A1



void setup() {

  Serial.begin(9600);
  Serial.println("Serial iniciado");
  BT.begin(9600);
  BT.println("BT iniciado");

  //declaracion salida relays
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  /*
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);*/

  //declaracion de entradas
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  // configuracion salidas
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  /*corrales se numeran a,b,c,d en sentido contrario a las agujas del reloj
    comenzando por corral superior a la izquierda*/

  /*la puerta central el sensor se coloca a la derecha*/
}

void loop() {

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

    if (dataSerial == 'd' || dataBT == 'd') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralD();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (dataSerial == 'x' || dataBT == 'x') {

      Serial.println("Moviendo puerta central");
      BT.println("Moviendo puerta central");

      puertaCentral();

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
  }
}

void corralA() {

  if (digitalRead(sensorPc) == 0) {
    digitalWrite(pc1, LOW);
    delay(200);
    digitalWrite(pc1, HIGH);
  }

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(pd1, LOW);
    delay(200);
    digitalWrite(pd1, HIGH);
  }

    delay(1000);

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(pi1, LOW);
    delay(200);
    digitalWrite(pi1, HIGH);
  }
}

void corralB() {

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(pd1, LOW);
    delay(200);
    digitalWrite(pd1, HIGH);
  }

    delay(1000);

  if (digitalRead(sensorPi) == 0) {
    digitalWrite(pi2, LOW);
    delay(200);
    digitalWrite(pi2, HIGH);
  }
}

void corralC() {

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(pi1, LOW);
    delay(200);
    digitalWrite(pi1, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPd) == 1) {
    digitalWrite(pd2, LOW);
    delay(200);
    digitalWrite(pd2, HIGH);
  }
}

void corralD() {

  if (digitalRead(sensorPc) == 1) {
    digitalWrite(pc2, LOW);
    delay(200);
    digitalWrite(pc2, HIGH);
  }

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(pd1, LOW);
    delay(200);
    digitalWrite(pd1, HIGH);
  }

    delay(1000);

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(pi1, LOW);
    delay(200);
    digitalWrite(pi1, HIGH);
  }
}

void puertaCentral() {

  if (digitalRead(sensorPc) == 0) {
    digitalWrite(pc1, LOW);
    delay(200);
    digitalWrite(pc1, HIGH);
  } else {
    digitalWrite(pc2, LOW);
    delay(200);
    digitalWrite(pc2, HIGH);
  }
}

void puertaDerecha() {

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(pi1, LOW);
    delay(200);
    digitalWrite(pi1, HIGH);
  }
  
  delay(1000);

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(pd1, LOW);
    delay(200);
    digitalWrite(pd1, HIGH);
  } else {
    digitalWrite(pd2, LOW);
    delay(200);
    digitalWrite(pd2, HIGH);
  }
}

void puertaIzquierda() {

  if (digitalRead(sensorPd) == 0) {
    digitalWrite(pd1, LOW);
    delay(200);
    digitalWrite(pd1, HIGH);
  }

  delay(1000);

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(pi1, LOW);
    delay(200);
    digitalWrite(pi1, HIGH);
  } else {
    digitalWrite(pi2, LOW);
    delay(200);
    digitalWrite(pi2, HIGH);
  }
}
