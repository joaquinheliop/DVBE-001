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
int pesoMin = 30;

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
  Serial.println("MENU\n\nA. Variacion [%] \nB. Escala \nC. Conteo \nD. Peso minimo estable\nS. Salir");
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
      Serial.println("Conteo: ");
      Serial.println(conteo);
      Serial.println("Ingrese un valor de 1 a 500");
      conteoF();
      break;

    case 'D':
      Serial.println("Peso minimo estable: ");
      Serial.println(pesoMin);
      Serial.println("Ingrese un valor de 5 a 1000");
      pesoMinF();
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

void conteoF() {

  String str;
  float conteoAux = 0;

  while (conteoAux == 0) {
    str = Serial.readStringUntil('\n');
    conteoAux = str.toInt();
  }


  if (conteoAux >= 1  && conteoAux <= 500) {
    conteo = conteoAux;
    Serial.println("Cambio correcto\nConteo: ");
    Serial.println(conteo);
    Serial.println();
    menu();
  } else {
    Serial.println("Valor incorrecto");
    Serial.println("Ingrese un valor de 1 a 500");
    conteoF();
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
