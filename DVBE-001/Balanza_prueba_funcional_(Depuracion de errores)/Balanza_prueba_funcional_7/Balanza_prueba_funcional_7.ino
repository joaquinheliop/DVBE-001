#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "HX711.h"

const int DOUT_PIN = A4;
const int SCK_PIN = A5;

#define pinState 4
#define pinLed 5

HX711 balanza;
//SoftwareSerial Serial (2, 3);

int peso;
int pesoAnt;

//Parametros de configuracion
float variacion;
float escala;
int pesoMin;
float correccion;

void setup() {

  Serial.begin(9600);
  Serial.begin(9600);
  balanza.begin(DOUT_PIN, SCK_PIN);

  //E/S
  pinMode(pinState, INPUT);
  pinMode(pinLed, OUTPUT);

  digitalWrite(pinLed, HIGH);
  
  //grabar EEPROM por primera vez con valores por defecto
  int eepr;
  EEPROM.get(10, eepr);
 
  if (eepr == -1) {
    EEPROM.put(0, 0.05);
    EEPROM.put(4, 1.0);
    EEPROM.put(8, 20);
    EEPROM.put(10, 1);
    Serial.println("Valores por defecto establecidos");
    Serial.println("Valores por defecto establecidos");
  }

  //obtener valores guardados en la EEPROM
  EEPROM.get(0, variacion);
  EEPROM.get(4, escala);
  EEPROM.get(8, pesoMin);

  Serial.print("Lectura del valor del crudo:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun objeto sobre la balanza");
  Serial.println("Destarando");

  balanza.set_scale(escala); //La escala por defecto es 1
  delay(300);
  balanza.tare(10);  //El peso actual es considerado Tara.

  Serial.println("Listo para pesar");

}

void loop() {

  /*while (digitalRead(pinState) == 0) {
    digitalWrite(pinLed, HIGH);
    delay(1000);
    digitalWrite(pinLed, LOW);
    delay(1000);
  }*/

  digitalWrite(pinLed, HIGH);
  pesoAnt = peso;
  peso = balanza.get_units(10);

  Serial.print("Peso 2: ");
  Serial.print(peso);
  Serial.println(" kg");

  /*Serial.print("P");
  //Serial.print(abs(peso));
  Serial.print(peso);
  Serial.println("$");*/
  

  delay(100);


  if (Serial.available()) {
    char lectura = Serial.read();
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
 // estable();
}

void estable() {


  while (abs(pesoAnt - peso) <= peso * variacion && abs(peso) > pesoMin) {

    Serial.print("Peso: ");
    Serial.print(peso);
    Serial.println(" kg");
    Serial.println("Peso estable");

    Serial.print("E");
    Serial.print(peso);
    Serial.println("$");
    delay (1500);

    peso = balanza.get_units(10);
  }
}

void menu() {
  char dato;
  Serial.println("MENU\n\nA. Variacion [%] \nB. Escala \nC. Peso minimo estable \nD. Correccion con Kg \nE. Guardar valores en EEPROM  \nF. Reestablecer valores por defecto \nS. Salir");

  //Limpiar buffer
  Serial.read();


  dato = Serial.read();

  while (!Serial.available()) {
  }

  dato = Serial.read();

  switch (dato) {
    case 'A':
      Serial.println("Variacion: ");
      Serial.println(variacion);
      Serial.println("Ingrese un valor de 2 a 50");
      variacionF();
      break;

    case 'B':
      Serial.println("Escala: ");
      Serial.println(escala);
      Serial.println("Ingrese un valor de 1 a 10000");
      escalaF();
      break;

    case 'C':
      Serial.println("Peso minimo estable: ");
      Serial.println(pesoMin);
      Serial.println("Ingrese un valor de 5 a 1000");
      pesoMinF();
      break;

    case 'D':
      correccionPeso();
      break;

    case 'E':
      eepromGuardar();
      break;

    case 'F':
      eepromReset();
      break;

    case 'S':
      Serial.println("Saliendo...\nListo!");
      loop();
      break;

    default:
      Serial.println("Comando incorrecto");
      menu();
      break;
  }
}

void variacionF() {

  String str;
  float variacionAux = 0;

  while (variacionAux == 0) {
    str = Serial.readStringUntil('\n');
    variacionAux = str.toInt();
  }


  if (variacionAux >= 2  && variacionAux <= 50) {
    variacion = variacionAux / 100;
    Serial.println("Cambio correcto\nVariacion: ");
    Serial.println(variacion);
    Serial.println();
    menu();
  } else {
    Serial.println("Valor incorrecto");
    Serial.println("Ingrese un valor de 2 a 50");
    variacionF();
  }
}

void escalaF() {

  String str;
  float escalaAux = 0;

  while (escalaAux == 0) {
    str = Serial.readStringUntil('\n');
    escalaAux = str.toInt();
  }


  if (escalaAux >= 1  && escalaAux <= 10000) {
    escala = escalaAux;
    balanza.set_scale(escala);
    Serial.println("Cambio correcto\nEscala: ");
    Serial.println(escala);
    Serial.println();
    menu();
  } else {
    Serial.println("Valor incorrecto");
    Serial.println("Ingrese un valor de 1 a 10000");
    escalaF();
  }
}

void pesoMinF() {

  String str;
  float pesoMinAux = 0;

  while (pesoMinAux == 0) {
    str = Serial.readStringUntil('\n');
    pesoMinAux = str.toInt();
  }


  if (pesoMinAux >= 10  && pesoMinAux <= 500) {
    pesoMin = pesoMinAux;
    Serial.println("Cambio correcto\nPeso minimo estable: ");
    Serial.println(pesoMin);
    Serial.println();
    menu();
  } else {
    Serial.println("Valor incorrecto");
    Serial.println("Ingrese un valor de 10 a 500");
    pesoMinF();
  }
}

void correccionPeso() {

  String str;
  float correccionAux = 0;
  float lecturaAux;
  float promedio = 0;

  Serial.println("Ingrese el valor en Kg corregido");

  while (correccionAux == 0) {
    str = Serial.readStringUntil('\n');
    correccionAux = str.toFloat();
  }


  if (correccionAux >= 1 && correccionAux <= 1500) {

    correccion = correccionAux;

    balanza.set_scale(1);
    delay(300);
    lecturaAux = balanza.get_units(10);

    float nuevaEscala = lecturaAux / correccion;

    balanza.set_scale(nuevaEscala);
    escala = nuevaEscala;

    Serial.print("Nueva escala establecida: ");
    Serial.println(nuevaEscala);
    delay(1500);
    Serial.println();
    menu();
  } else {
    Serial.println("Valor incorrecto");
    Serial.println("Ingrese un valor de 1 a 1500");
    pesoMinF();
  }
}

void eepromGuardar() {

  char confirmacion;

  Serial.println("¿Desea guardar valores en la EEPROM S|N?");
  confirmacion = Serial.read();

  while (!Serial.available()) {
  }

  confirmacion = Serial.read();

  switch (confirmacion) {
    case 'S':

      EEPROM.put(0, variacion);
      EEPROM.put(4, escala);
      EEPROM.put(8, pesoMin);
      
      Serial.println("Valores guardados");
      delay(1000);
      menu();
      break;

    case 'N':
      Serial.println("Valores no guardados");
      menu();
      break;

    default:
      Serial.println("Comando incorrecto");
      eepromGuardar();
      break;
  }
}

void eepromReset() {

  char confirmacion;

  Serial.println("¿Desea reestablecer valores por defecto S|N?");
  confirmacion = Serial.read();

  while (!Serial.available()) {
  }

  confirmacion = Serial.read();

  switch (confirmacion) {
    case 'S':
      EEPROM.put(0, 0.05);
      EEPROM.put(4, 1.0);
      EEPROM.put(8, 20);

      EEPROM.get(0, variacion);
      EEPROM.get(4, escala);
      EEPROM.get(8, pesoMin);

      Serial.println("Valores reestablecidos");
      delay(1000);
      menu();
      break;

    case 'N':
      Serial.println("Valores no reestablecidos");
      menu();
      break;

    default:
      Serial.println("Comando incorrecto");
      eepromReset();
      break;
  }
}
