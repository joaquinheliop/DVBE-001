#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial BT (9, 8);

//pines relays puerta central
#define pc1 2
#define pc2 3
#define sensorPc A0
//pines relays puerta izquierda
#define pi1 4
#define pi2 5
#define sensorPi A1
//pines relays puerta derecha
#define pd1 6
#define pd2 7
#define sensorPd A2


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

    if (val == 'a' || valBt == 'a') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralA();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (val == 'b' || valBt == 'b') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralB();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (val == 'c' || valBt == 'c') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralC();

      Serial.println("Listo");
      BT.println("Listo");
    }

    if (val == 'd' || valBt == 'd') {

      Serial.println("Moviendo puertas");
      BT.println("Moviendo puertas");

      corralD();

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

  if (digitalRead(sensorPi) == 1) {
    digitalWrite(pi1, LOW);
    delay(200);
    digitalWrite(pi1, HIGH);
  }
}
