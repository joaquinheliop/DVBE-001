#include <SoftwareSerial.h>
#include "HX711.h"

const int DOUT_PIN = A4;
const int SCK_PIN = A5;

HX711 balanza;
SoftwareSerial BT (2,3);

void setup() {
  
  Serial.begin(9600);

  balanza.begin(DOUT_PIN, SCK_PIN);

  pinMode(2, INPUT);

  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando");

  balanza.set_scale(1377); //La escala por defecto es 1   1269//156.25c//
  balanza.tare(20);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");


}

void loop() {

 
    Serial.print("Peso: ");
    float peso = balanza.get_units(10);
    Serial.print(peso, 1);
    Serial.println(" kg");
  delay(500);
  
  
}
