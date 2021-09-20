#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "HX711.h"

const int DOUT_PIN = A4;//A3,A2 bascula los nietos
const int SCK_PIN = A5;

#define pinState 4
#define pinLed 5

HX711 balanza;
SoftwareSerial BT (2, 3);//3,4 bascula los nietos

float peso, pesoAnt, pesoSig;
float pesoEst;
byte intentos = 1;
byte contador = 0;

//Parametros de configuracion
float variacion;
float escala;
float correccion;
int pesoMin;

void setup() {

  Serial.begin(9600);
  BT.begin(9600);
  balanza.begin(DOUT_PIN, SCK_PIN);

  //Recoleccion de datos EXCEL
  /*Serial.println("CLEARDATA");
  Serial.println("LABEL,HORA,TIEMPO,PESO,DIFERENCIA DE PESO, PESO ESTABLE");
  Serial.println("RESETTIMER");*/

  //E/S
  pinMode(pinState, INPUT);
  pinMode(pinLed, OUTPUT);

  //Configuracion inicial de pinLed
  digitalWrite(pinLed, HIGH);

  //grabar EEPROM por primera vez con valores por defecto
  int eepr;
  EEPROM.get(14, eepr);
  if (eepr == -1) {
    EEPROM.put(0, 1);//variacion
    EEPROM.put(4, 1000);//escala
    EEPROM.put(8, 1);//correccion
    EEPROM.put(12, 50);//peso minimo estable
    EEPROM.put(14, 1);//estado eeprom
  }

  //obtener valores guardados en la EEPROM
  EEPROM.get(0, variacion);
  EEPROM.get(4, escala);
  EEPROM.get(8, correccion);
  EEPROM.get(12, pesoMin);

  //configuraciones iniciales de escala y tara
  balanza.set_scale(escala);
  delay(100);
  balanza.tare(10);

}

void loop() {

  //Recoleccion de datos EXCEL
  /*Serial.print("DATA,TIME,TIMER,");
  Serial.print(peso);
  Serial.print(",");
  Serial.print(peso - pesoAnt);
  Serial.print(",");
  Serial.print("0");
  Serial.println("");*/

  //Pulsar led en el caso de estar buscando conexion BT (Solo para HM-10,HC-05)
   /* while (digitalRead(pinState) == 0) {
      digitalWrite(pinLed, HIGH);
      delay(1000);
      digitalWrite(pinLed, LOW);
      delay(1000);
    }*/
  digitalWrite(pinLed, HIGH);

  pesoAnt = balanza.get_units(2);
  peso = balanza.get_units(2);
  pesoSig = balanza.get_units(2);

  //Imprime datos en el puerto BT
  BT.print("P");
  BT.print(round(pesoSig));
  BT.println("Kg");

  delay(1);
  //Lee datos del puerto BT
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

      case 'm':
        menu();
        break;
    }
  }
  estable();
}

void estable() {


  if (abs(pesoAnt - peso) <= variacion && abs(pesoSig - peso) <= variacion && abs(peso) > pesoMin) {

    if (contador < intentos - 1) {
      contador ++;
      loop();
    }
    contador = 0;

    pesoEst = peso;
    while (peso > pesoMin) {

      //Recoleccion de datos EXCEL
      Serial.print("DATA,TIME,TIMER,");
      Serial.print(peso);
      Serial.print(",");
      Serial.print("0");
      Serial.print(",");
      Serial.print(pesoEst);
      Serial.println(" ,");

      //Imprime datos en puerto BT
      BT.print("E");
      BT.print(round(pesoEst));
      BT.println("Kg");
      delay (1);
      peso = balanza.get_units(5);

      //Lee datos del puerto BT
      if (BT.available()) {
        char lectura = BT.read();
        switch (lectura) {

          case 'T':
            repesado();
            break;

          case 'M':
            menu();
            break;
        }
      }
    }
  }
}

void menu() {
  char dato;

  BT.println("MENU\n\nA. Variacion [Kg] \nB. Peso minimo estable \nC. Establecer escala \nG. Guardar valores en EEPROM  \nR. Reestablecer valores por defecto \nS. Salir");

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
      BT.println("Ingrese un valor de 0,1 a 5 o 'S' para salir");
      variacionF();
      break;

    case 'B':
      BT.println("Peso minimo estable: ");
      BT.println(pesoMin);
      BT.println("Ingrese un valor de 5 a 500 o 'S' para salir");
      pesoMinF();
      break;

    case 'C':
      BT.print("Escala: ");
      BT.println(escala);
      BT.println("Ingrese el peso conocido en Kg 'S' para salir");
      correccionPesoF();
      break;

    case 'G':
      eepromGuardar();
      break;

    case 'R':
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

//funcion creeada para que cuando recibe varias taras en la reestabilizacion del peso no me haga un a tara a cero se logro quitando la capacidad de lectura mientras hace el repesaje
void repesado() {

  //Recoleccion de datos EXCEL
  Serial.print("DATA,TIME,TIMER,");
  Serial.print(peso);
  Serial.print(",");
  Serial.print(peso - pesoAnt);
  Serial.print(",");
  Serial.print("0");
  Serial.println("");

  pesoAnt = balanza.get_units(2);
  peso = balanza.get_units(2);
  pesoSig = balanza.get_units(2);

  //Imprime datos en el puerto BT
  BT.print("P");
  BT.print(round(pesoSig));
  BT.println("Kg");

  delay(1);

  while(BT.available()){
  BT.read();
  }
}

void variacionF() {

  String str;
  float variacionAux = 0;

  //limpiar buffer
  BT.read();

  while (!BT.available()) {
  }

  str = BT.readStringUntil('\n');
  char strAux = str.charAt(0);
  variacionAux = str.toFloat();

  if (strAux == 'S' || strAux == 's') {
    menu();
  } else if (variacionAux >= 0.1  && variacionAux <= 5) {
    variacion = variacionAux;
    BT.println("Cambio correcto\nVariacion: ");
    BT.println(variacion);
    BT.println();
    menu();
  } else {
    BT.println("Valor incorrecto");
    BT.println("Ingrese un valor de 0,1 a 5");
    variacionF();
  }
}

void pesoMinF() {

  String str;
  float pesoMinAux = 0;

  //limpiar buffer
  BT.read();

  while (!BT.available()) {
  }

  str = BT.readStringUntil('\n');
  char strAux = str.charAt(0);
  pesoMinAux = str.toInt();

  if (strAux == 'S' || strAux == 's') {
    menu();
  } else if (pesoMinAux >= 10  && pesoMinAux <= 500) {
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

void correccionPesoF() {

  String str;
  float correccionAux = 0;
  float lecturaAux;
  float promedio = 0;

  //limpiar buffer
  BT.read();

  while (!BT.available()) {
  }

  str = BT.readStringUntil('\n');
  char strAux = str.charAt(0);
  correccionAux = str.toFloat();

  if (strAux == 'S' || strAux == 's') {
    menu();
  } else if (correccionAux >= 1 && correccionAux <= 1500) {

    correccion = correccionAux;

    balanza.set_scale(1);
    delay(300);
    lecturaAux = balanza.get_units(10);

    float nuevaEscala = lecturaAux / correccion;

    balanza.set_scale(nuevaEscala);
    escala = nuevaEscala;

    BT.print("Nueva escala establecida: ");
    BT.println(nuevaEscala);
    delay(1500);
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
      EEPROM.put(4, escala);
      EEPROM.put(8, correccion);
      EEPROM.put(12, pesoMin);

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
      EEPROM.put(0, 1);//variacion
      EEPROM.put(4, 1000);//escala
      EEPROM.put(8, 1);//correccion
      EEPROM.put(12, 50);//peso minimo estable

      EEPROM.get(0, variacion);
      EEPROM.get(4, escala);
      EEPROM.get(8, correccion);
      EEPROM.get(12, pesoMin);


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
