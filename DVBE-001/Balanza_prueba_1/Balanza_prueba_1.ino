#include "HX711.h"

const int DOUT_PIN = A0;
const int SCK_PIN = A1;

HX711 balanza;

void setup() {
  Serial.begin(9600);
  balanza.begin(DOUT_PIN, SCK_PIN);

  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando");

  balanza.set_scale(5169.66); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");
}

void loop() {
  //if (Serial.available() > 0) {
    char orden = Serial.read();

    //if (orden == 'p') {
      Serial.print("Peso: ");

      Serial.print(balanza.get_units(10), 1);
      Serial.println(" kg");
      delay(1000);
   // }
    if (orden == 't') {
      Serial.println("Destarando");
      balanza.tare(20);  //El peso actual es considerado Tara.
      Serial.println("Listo");
    }
//  }
}
