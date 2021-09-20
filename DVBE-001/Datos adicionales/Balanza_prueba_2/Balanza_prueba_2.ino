#include <SoftwareSerial.h>
#include "HX711.h"

const int DOUT_PIN = A4;
const int SCK_PIN = A5;

HX711 balanza;
SoftwareSerial BT (2,3);

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  
  balanza.begin(DOUT_PIN, SCK_PIN);

  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando");

  balanza.set_scale(1); //La escala por defecto es 1
  balanza.tare(10);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");
}

void loop() {
  
    char orden = Serial.read();
    
      Serial.print("Peso: ");
      
      double dato = balanza.get_units(10);
      Serial.print(dato);
      Serial.println(" kg");
      
      BT.print(balanza.get_units(10), 1);
      //BT.print(" kg");
      //BT.println("$");
      
      delay(500);

   if (BT.available() > 0) {
    orden = BT.read();
    Serial.print(orden);
   
    if (orden == 'T') {
      Serial.println("Destarando");
      balanza.tare(20);  //El peso actual es considerado Tara.
      Serial.println("Listo");
    } else {
      Serial.println("Comando incorrecto");
    }
 }
}
