
#include "HX711.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int DOUT_PIN = A1;
const int SCK_PIN = A0;

HX711 balanza;

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Bienvenido!");

  balanza.begin(DOUT_PIN, SCK_PIN);

  pinMode(2, INPUT);

  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando");
  
  delay(600);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Destarando...");
  balanza.set_scale(20346); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");


}

void loop() {
  lcd.clear();
  while (digitalRead(2) == 1) {
    Serial.print("Peso: ");
    float peso = balanza.get_units(10);
    Serial.print(peso, 1);
    Serial.println(" kg");
    
    lcd.setCursor(0, 0);
    lcd.print("Peso:");
    lcd.setCursor(5, 1);
    lcd.print(abs(peso),2);
    lcd.print(" Kg.");
  }
  Serial.println("Destarando");

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Destarando...");

  balanza.tare(20);  //El peso actual es considerado Tara.

  Serial.println("Listo");

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Listo!");
  delay(500);
}
