#include <SoftwareSerial.h>

#define pinRx 3
#define pinTx 2
#define pinState 4
#define pinSensor 5
#define pinLed 6

SoftwareSerial Sensor (pinSensor, 18);
SoftwareSerial Bt (pinTx, pinRx);


void setup() {
  Serial.begin(9600);
  Serial.println("Listo");
  Sensor.begin(9600);
  Bt.begin(9600);

  pinMode(pinLed, OUTPUT);
  pinMode(pinState, INPUT);

  digitalWrite(pinLed, HIGH);
}

void loop() {

  Sensor.listen();
  if (Sensor.available()) {

    digitalWrite(pinLed, LOW);
    
    String dato = Sensor.readString();
    String codigoInv = dato.substring(2, 14);
    String codigo = invertirCadena(codigoInv);
    
    Serial.println(codigoInv);
    Bt.println(codigoInv);
    
    digitalWrite(pinLed, HIGH);
    delay(200);
    digitalWrite(pinLed, LOW);
    delay(200);
    digitalWrite(pinLed, HIGH);
    delay(200);
    digitalWrite(pinLed, LOW);
    delay(200);
    digitalWrite(pinLed, HIGH);

  }

}

String invertirCadena(String s) {
  String temporal = "";
  for (int m = s.length() - 1; m >= 0; m--)
    temporal += s[m];
  return temporal;
}
