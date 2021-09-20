#include <SoftwareSerial.h>
#include "HX711.h"

const int DOUT_PIN = A4;
const int SCK_PIN = A5;

HX711 balanza;
SoftwareSerial BT (2, 3);

float peso = 0;
float pesoAnt;

//Parametros de configuracion
float variacion = 0.05;
unsigned int conteo = 50;
float escala = 1377;

void setup() {

  Serial.begin(9600);
  BT.begin(9600);
  balanza.begin(DOUT_PIN, SCK_PIN);

  Serial.print("Lectura del valor del crudo:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun objeto sobre la balanza");
  Serial.println("Destarando");

  balanza.set_scale(escala); //La escala por defecto es 1   1269//156.25c//
  delay(300);
  balanza.tare(10);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");

}

void loop() {


  pesoAnt = peso;
  peso = balanza.get_units(10);

  Serial.print("Peso: ");
  Serial.print(peso, 1);
  Serial.println(" kg");

  BT.print("P");
  BT.print(abs(peso), 1);
  BT.println("$");

  delay(100);


  if (BT.available()) {
    char lectura = BT.read();
    switch (lectura) {
      case 'T':
        Serial.println("Destarando");
        balanza.tare(10);
        break;

      case 'M':
        menu();
        break;
    }
  }

  estable();


}

void estable() {
  unsigned int conteo1 = conteo;

  while (abs(pesoAnt - peso) <= peso * variacion && abs(peso) > 30) {

    conteo1 = conteo1 - 1;

    if (conteo1 == 0) {
      Serial.print("Peso: ");
      Serial.print(peso, 1);
      Serial.println(" kg");

      Serial.println("Peso estable");
      BT.print("E");
      BT.print(peso, 1);
      BT.println("$");
      delay (1500);

      conteo1 = conteo;
      peso = balanza.get_units(10);
    }
  }
}

void menu() {
  BT.println("MENU\n\nA. Variacion \nB. Escala \nC. Conteo \nD. Peso minimo estable");
}
