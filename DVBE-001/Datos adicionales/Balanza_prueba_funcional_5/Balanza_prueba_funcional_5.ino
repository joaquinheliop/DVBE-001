#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "HX711.h"

const int DOUT_PIN = A4;
const int SCK_PIN = A5;

#define pinState 4
#define pinLed 5

HX711 balanza;
SoftwareSerial BT (2, 3);

float peso;
float pesoAnt;

//Parametros de configuracion
float variacion;
unsigned int conteo;
float escala;
int pesoMin;
float correccion;

void setup() {
  
  Serial.begin(9600);
  BT.begin(9600);
  balanza.begin(DOUT_PIN, SCK_PIN);

  //E/S
  pinMode(pinState, INPUT);
  pinMode(pinLed, OUTPUT);

  digitalWrite(pinLed, HIGH);
  //grabar EEPROM por primera vez con valores por defecto
  int eepr;
  EEPROM.get(12, eepr);

  if (eepr == -1) {
    EEPROM.put(0, 0.05);
    EEPROM.put(4, 50);
    EEPROM.put(6, 1377.0);
    EEPROM.put(10, 30);
    EEPROM.put(12, 1);
    Serial.println("Valores por defecto establecidos");
    BT.println("Valores por defecto establecidos");
  }

  //obtener valores guardados en la EEPROM
  EEPROM.get(0, variacion);
  EEPROM.get(4, conteo);
  EEPROM.get(6, escala);
  EEPROM.get(10, pesoMin);

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

  while (digitalRead(pinState) == 0) {
    digitalWrite(pinLed, HIGH);
    delay(1000);
    digitalWrite(pinLed, LOW);
    delay(1000);
  }

  digitalWrite(pinLed, HIGH);
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

  while (abs(pesoAnt - peso) <= peso * variacion && abs(peso) > pesoMin) {

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
  char dato;
  BT.println("MENU\n\nA. Variacion [%] \nB. Escala \nC. Conteo \nD. Peso minimo estable \nE. Correccion con Kg \nF. Guardar valores en EEPROM \nG. Reestablecer valores por defecto \nS. Salir");

  //Limpiar buffer
  BT.read();
  
  
  dato = BT.read();

  while (!BT.available()) {
  }

  dato = BT.read();

  switch (dato) {
    case 'A':
      BT.println("Variacion: ");
      BT.println(variacion);
      BT.println("Ingrese un valor de 2 a 50");
      variacionF();
      break;

    case 'B':
      BT.println("Escala: ");
      BT.println(escala);
      BT.println("Ingrese un valor de 1 a 10000");
      escalaF();
      break;

    case 'C':
      BT.println("Conteo: ");
      BT.println(conteo);
      BT.println("Ingrese un valor de 1 a 500");
      conteoF();
      break;

    case 'D':
      BT.println("Peso minimo estable: ");
      BT.println(pesoMin);
      BT.println("Ingrese un valor de 5 a 1000");
      pesoMinF();
      break;

    case 'E':
      correccionPeso();
      break;

    case 'F':
      eepromGuardar();
      break;

    case 'G':
      eepromReset();
      break;

    case 'S':
      BT.println("Saliendo...\nListo!");
      loop();
      break;

    default:
      BT.println("Comando incorrecto");
      menu();
      break;
  }
}

void variacionF() {

  String str;
  char salida[1] = {0};
  float variacionAux = 0;

  while (variacionAux == 0) {
    str = BT.readStringUntil('\n');
    variacionAux = str.toInt();
  }

    
  if (variacionAux >= 2  && variacionAux <= 50) {
    variacion = variacionAux / 100;
    BT.println("Cambio correcto\nVariacion: ");
    BT.println(variacion);
    BT.println();
    menu();
  } else {
    BT.println("Valor incorrecto");
    BT.println("Ingrese un valor de 2 a 50");
    variacionF();
  }
}

void escalaF() {

  String str;
  float escalaAux = 0;

  while (escalaAux == 0) {
    str = BT.readStringUntil('\n');
    escalaAux = str.toInt();
  }


  if (escalaAux >= 1  && escalaAux <= 10000) {
    escala = escalaAux;
    balanza.set_scale(escala);
    BT.println("Cambio correcto\nEscala: ");
    BT.println(escala);
    BT.println();
    menu();
  } else {
    BT.println("Valor incorrecto");
    BT.println("Ingrese un valor de 1 a 10000");
    escalaF();
  }
}

void conteoF() {

  String str;
  float conteoAux = 0;

  while (conteoAux == 0) {
    str = BT.readStringUntil('\n');
    conteoAux = str.toInt();
  }


  if (conteoAux >= 1  && conteoAux <= 500) {
    conteo = conteoAux;
    BT.println("Cambio correcto\nConteo: ");
    BT.println(conteo);
    BT.println();
    menu();
  } else {
    BT.println("Valor incorrecto");
    BT.println("Ingrese un valor de 1 a 500");
    conteoF();
  }
}

void pesoMinF() {

  String str;
  float pesoMinAux = 0;

  while (pesoMinAux == 0) {
    str = BT.readStringUntil('\n');
    pesoMinAux = str.toInt();
  }


  if (pesoMinAux >= 10  && pesoMinAux <= 500) {
    pesoMin = pesoMinAux;
    BT.println("Cambio correcto\nPeso minimo estable: ");
    BT.println(pesoMin);
    BT.println();
    menu();
  } else {
    BT.println("Valor incorrecto");
    BT.println("Ingrese un valor de 10 a 500");
    pesoMinF();
  }
}

void correccionPeso() {

  String str;
  float correccionAux = 0;
  int lecturaAux;

  BT.println("Ingrese el valor en Kg corregido");

  while (correccionAux == 0) {
    str = BT.readStringUntil('\n');
    correccionAux = str.toFloat();
  }


  if (correccionAux >= 1 && correccionAux <= 1500) {
    correccion = correccionAux;
    lecturaAux = balanza.read();
    balanza.set_scale(lecturaAux / correccion);
    BT.println("Cambio correcto\nCorreccion aplicada: ");
    BT.print(correccion);
    BT.println(" Kg");
    BT.println();
    menu();
  } else {
    BT.println("Valor incorrecto");
    BT.println("Ingrese un valor de 1 a 1500");
    pesoMinF();
  }
}

void eepromGuardar() {

  char confirmacion;

  BT.println("¿Desea guardar valores en la EEPROM S|N?");
  confirmacion = BT.read();

  while (!BT.available()) {
  }

  confirmacion = BT.read();

  switch (confirmacion) {
    case 'S':
      EEPROM.put(0, variacion);
      EEPROM.put(4, conteo);
      EEPROM.put(6, escala);
      EEPROM.put(10, pesoMin);
      BT.println("Valores guardados");
      delay(1000);
      menu();
      break;

    case 'N':
      BT.println("Valores no guardados");
      menu();
      break;

    default:
      BT.println("Comando incorrecto");
      eepromGuardar();
      break;
  }
}

void eepromReset() {

  char confirmacion;

  BT.println("¿Desea reestablecer valores por defecto S|N?");
  confirmacion = BT.read();

  while (!BT.available()) {
  }

  confirmacion = BT.read();

  switch (confirmacion) {
    case 'S':
      EEPROM.update(0, 0.05);
      EEPROM.update(4, 50);
      EEPROM.update(6, 1377.0);
      EEPROM.update(10, 30);

      EEPROM.get(0, variacion);
      EEPROM.get(4, conteo);
      EEPROM.get(6, escala);
      EEPROM.get(10, pesoMin);

      BT.println("Valores reestablecidos");
      delay(1000);
      menu();
      break;

    case 'N':
      BT.println("Valores no reestablecidos");
      menu();
      break;

    default:
      BT.println("Comando incorrecto");
      eepromReset();
      break;
  }
}
