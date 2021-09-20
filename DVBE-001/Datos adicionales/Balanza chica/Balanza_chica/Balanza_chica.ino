
#include "HX711.h"

const int DOUT_PIN = A1;
const int SCK_PIN = A0;

HX711 balanza;

void setup() {
  Serial.begin(9600);

  balanza.begin(DOUT_PIN, SCK_PIN);

  pinMode(2, INPUT);

  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando");

  balanza.set_scale(20346); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");


}

void loop() {

  while (digitalRead(2) == 1) {
    Serial.print("Peso: ");
    float peso = balanza.get_units(10);
    Serial.print(peso, 1);
    Serial.println(" kg");
  
  }
  Serial.println("Destarando");
  balanza.tare(20);  //El peso actual es considerado Tara.
  Serial.println("Listo");

}
