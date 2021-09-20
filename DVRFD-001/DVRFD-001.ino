#include <SoftwareSerial.h>

#define pinRx 3
#define pinTx 4
#define pinState 5
#define pinSensor 6
#define pinBuzzer 2

SoftwareSerial RfBt (pinSensor, pinTx); // RX, TX;


void setup() {
  Serial.begin(9600);
  Serial.println("Listo");
  RfBt.begin(9600);

  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinState, INPUT);

  digitalWrite(pinBuzzer, LOW);
  delay (1000);
  digitalWrite(pinBuzzer, HIGH);
}

void loop() {
  byte i = 0;
  char lectura[32], codAnimal[10], codPais[4] ;
  if (RfBt.available()) {

    while (RfBt.available()) {
      lectura[i] = RfBt.read();
      i++;
      delay(5);
    }

    //Codigo animal HEX
    for ( int j = 10; j >= 1; j--) {
      codAnimal[10 - j] = lectura[j];
    }
    //Codigo animal en DEC
    long codigoAnimalLong = StrToDec(codAnimal);

    //Codigo pais en HEX
    for ( int j = 14; j >= 11; j--) {
      codPais[14 - j] = lectura[j];
    }
    //Codigo Pais en DEC
    long codigoPaisLong = StrToDec(codPais);

    //DEC completo Bluetooth

    RfBt.print("0" + (String)codigoPaisLong);
    RfBt.println(codigoAnimalLong);
    digitalWrite(pinBuzzer, LOW);
    delay(1000);
    digitalWrite(pinBuzzer, HIGH);
  }
}

//Convierte un string HEX a un string DEC
long StrToDec(char str[]) {
  return (long) strtol(str, 0, 16);
}
